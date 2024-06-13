#include <iostream>

#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>
#include <thread>

#include "connection.hpp"
#include "data.hpp"
#include "functional_connection.hpp"
#include "functional_creator.hpp"
#include "functional_parser.hpp"
#include "functional_serializer.hpp"
#include "functional_task.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server.hpp"

#include "client.hpp"
#include "string.hpp"

using namespace client;
using namespace server;
using namespace engine;
using namespace mcu_control;
using namespace mcu_control_utl;

using RawData = std::string;
using SubscriberId = int;

using TestClient = Client<RawData, SubscriberId>;



TEST(ut_client, ctor_dtor_sanity) {
	// GIVEN
	const int server_response_timeout_ms(10);
	
	// WHEN
	TestClient *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestClient(
				&s_connection,
				server_response_timeout_ms
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}

TEST(ut_client, run_sanity_negative) {
	// GIVEN
	const int fail_code(-100);
	const int success_code(0);

	using TestServer = Server<RawData, SubscriberId>;
	using EngineTask = typename TestServer::Engine::EngineTask;

	static FunctionalCreator<EngineTask *(const Data&)> task_creator(
		[fail_code, success_code](const Data& data)-> EngineTask * {
			auto cfg_obj = Data::cast<Object>(data);
			auto cfg_code = Data::cast<Integer>(cfg_obj.access("cfg_code")).get();
			return new FunctionalTask<Data *>(
				[cfg_code, fail_code, success_code](void)-> Data * {
					if (fail_code == cfg_code) {
						throw std::runtime_error("fail code received");
					}
					Object report;
					report.add("result", Integer(success_code));
					return report.clone();
				}
			);
		}
	);

	FunctionalCreator<Data *(const std::exception&)> fail_report_creator(
		[fail_code](const std::exception& e)-> Data * {
			Object report;
			report.add("result", Integer(fail_code));
			report.add("what", String(std::string(e.what())));
			return report.clone();
		}
	);

	FunctionalParser<Data *(const RawData&)> parser(
		[](const RawData& data) -> Data * {
			return JsonDataParser().parse(data).clone();
		}
	);

	FunctionalSerializer<RawData(const Data&)> serializer(
		[](const Data& data) -> RawData {
			return JsonDataSerializer().serialize(Data::cast<Object>(data));
		}
	);

	const std::map<RawData, int> test_cases {
		{"{\"cfg_code\": " + std::to_string(fail_code) + "}", fail_code},
		{"{\"cfg_code\": " + std::to_string(success_code) + "}", success_code},
		{"{\"cfg_code_gaga\": " + std::to_string(success_code) + "}", fail_code},
		{"{\"cfg_code\": \"" + std::to_string(success_code) + "\"}", fail_code}
	};

	for (auto test_case: test_cases) {
		// WHEN
		std::shared_ptr<FunctionalConnection<int, RawData>> server_connection(nullptr);
		std::shared_ptr<FunctionalConnection<int, RawData>> client_connection(nullptr);
		server_connection = std::make_shared<FunctionalConnection<int, RawData>>(
			[client_connection](const RawData& data) {
				std::cout << "server2client: report: " << std::endl << data << std::endl;
				client_connection->post_data(data);
			},
			[]()-> int {
				static int s_last_id(0);
				return s_last_id++;
			}
		);
		client_connection = std::make_shared<FunctionalConnection<int, RawData>>(
			[server_connection](const RawData& data) {
				std::cout << "client2server: sending data: " << std::endl << data << std::endl;
				server_connection->post_data(data);
			},
			[]()-> int {
				static int s_last_id(0);
				return s_last_id++;
			}
		);

		TestServer server_instance(task_creator, fail_report_creator, parser, serializer, server_connection.get());
		TestClient client_instance(client_connection.get(), 100);
		RawData report("");

		// THEN
		std::cout << "running TC with input:" << std::endl << test_case.first << std::endl;
		server_instance.run();

		ASSERT_NO_THROW(report = client_instance.run(test_case.first));
		// ASSERT_TRUE();
		// ASSERT_NO_THROW(client_connection.post_data(test_case.first));
		// ASSERT_NO_THROW(instance.stop());

		// TEAR_DOWN
		server_thread.join();
		server_instance.stop();
	}
}