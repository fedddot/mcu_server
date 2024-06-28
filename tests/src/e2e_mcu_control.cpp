#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

#include "client.hpp"
#include "connection.hpp"
#include "data.hpp"
#include "functional_parser.hpp"
#include "functional_serializer.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_task_engine.hpp"
#include "string.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

using namespace client;
using namespace server;
using namespace engine;
using namespace engine_utl;
using namespace mcu_control;

using RawData = std::string;
using GpioId = int;

FunctionalParser<ServerTaskId(const Data&)> s_task_id_parser(
	[](const engine::Data& data) {
		return ServerTaskId(Data::cast<String>(Data::cast<Object>(data).access("task_id")).get());
	}
);

FunctionalCreator<Data *(const std::exception&)> s_failure_report_creator(
	[](const std::exception& e) {
		Object report;
		report.add("result", Integer(-1));
		report.add("what", String(std::string(e.what())));
		return report.clone();
	}
);

FunctionalParser<Data *(const RawData&)> s_raw_data_parser(
	[](const RawData& data)-> Data * {
		return JsonDataParser().parse(data).clone();
	}
);

FunctionalSerializer<RawData(const Data&)> s_raw_data_serializer(
	[](const Data& data)-> RawData {
		return JsonDataSerializer().serialize(Data::cast<Object>(data));
	}
);

FunctionalCreator<Gpio *(const GpioId&, const Gpio::Direction&)> s_gpio_creator(
	[](const GpioId& id, const Gpio::Direction& dir)-> Gpio * {
		switch (dir) {
		case Gpio::Direction::IN:
			return new server_uts::TestGpi();
		case Gpio::Direction::OUT:
			return new server_uts::TestGpo();
		default:
			throw std::invalid_argument("unsupported GPIO direction");
		}
	}
);

FunctionalParser<GpioId(const Data&)> s_gpio_id_parser(
	[](const Data& data)-> GpioId {
		return static_cast<GpioId>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_id")).get());
	}
);

FunctionalParser<Gpio::Direction(const Data&)> s_gpio_dir_parser(
	[](const Data& data) {
		return static_cast<Gpio::Direction>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_dir")).get());
	}
);

FunctionalParser<Gpio::State(const Data&)> s_gpio_state_parser(
	[](const Data& data) {
		return static_cast<Gpio::State>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_state")).get());
	}
);

class TestConnection: public Connection<RawData> {
public:
	using SendDataAction = std::function<void(const RawData&)>;
	TestConnection(int timeout_ms, const SendDataAction& send_data): m_timeout_ms(timeout_ms), m_send_data(send_data) {

	}
	void send_data(const RawData& data) const override {
		m_send_data(data);
	}
	bool is_readable() const override {
		std::unique_lock lock(m_mutex);
		if (!m_datas.empty()) {
			return true;
		}
		auto wait_result = m_cond.wait_for(lock, std::chrono::milliseconds(m_timeout_ms));
		return !m_datas.empty();
	}
	RawData read_data() const override {
		auto iter = m_datas.begin();
		RawData data(*iter);
		m_datas.erase(iter);
		return data;
	}
	void feed_data(const RawData& data) {
		std::unique_lock lock(m_mutex);
		m_datas.push_back(data);
		m_cond.notify_all();
	}
private:
	int m_timeout_ms;
	SendDataAction m_send_data;
	mutable std::vector<RawData> m_datas;
	mutable std::mutex m_mutex;
	mutable std::condition_variable m_cond;
};

TEST(e2e_mcu_control, flow_sanity) {
	// GIVEN
	const int receiving_timeout(10);
	std::unique_ptr<TestConnection> client_connection(nullptr);
	std::unique_ptr<TestConnection> server_connection(nullptr);

	client_connection = std::make_unique<TestConnection>(
		10,
		[&server_connection](const RawData& data){
			std::cout << "client sends to server data: " << data << std::endl;
			server_connection->feed_data(data);
		}
	);
	server_connection = std::make_unique<TestConnection>(
		10,
		[&client_connection](const RawData& data){
			std::cout << "server sends to client data: " << data << std::endl;
			client_connection->feed_data(data);
		}
	);
	
	Object create_gpio_task_data;
	create_gpio_task_data.add("task_id", String("create_gpio"));
	create_gpio_task_data.add("gpio_dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
	create_gpio_task_data.add("gpio_id", Integer(10));

	Object set_gpio_task_data;
	set_gpio_task_data.add("task_id", String("set_gpio"));
	set_gpio_task_data.add("gpio_state", Integer(static_cast<int>(Gpio::State::HIGH)));
	set_gpio_task_data.add("gpio_id", Integer(10));

	Object read_gpio_task_data;
	read_gpio_task_data.add("task_id", String("get_gpio"));
	read_gpio_task_data.add("gpio_id", Integer(10));

	Object delete_gpio_task_data;
	delete_gpio_task_data.add("task_id", String("delete_gpio"));
	delete_gpio_task_data.add("gpio_id", Integer(10));

	Object shutdown_task_data;
	shutdown_task_data.add("task_id", String("shutdown"));

	const std::vector<RawData> test_cases {
		s_raw_data_serializer.serialize(create_gpio_task_data),
		s_raw_data_serializer.serialize(set_gpio_task_data),
		s_raw_data_serializer.serialize(read_gpio_task_data),
		s_raw_data_serializer.serialize(delete_gpio_task_data),
		s_raw_data_serializer.serialize(shutdown_task_data)
	};

	// WHEN
	Client<RawData> client(client_connection.get());
	Server<RawData, GpioId> server(
		server_connection.get(),
		s_task_id_parser,
		s_failure_report_creator,
		s_raw_data_parser,
		s_raw_data_serializer,
		s_gpio_id_parser,
		s_gpio_dir_parser,
		s_gpio_state_parser,
		s_gpio_creator
	);

	// THEN
	std::thread server_thread(
		[](Server<RawData, GpioId> *server_ptr) {
			server_ptr->run();
		},
		&server
	);

	for (auto test_case: test_cases) {
		auto report = client.run(test_case);
		std::unique_ptr<Data> report_parsed(s_raw_data_parser.parse(report));
		ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_parsed).access("result")).get());
	}
	server_thread.join();
}