#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include <thread>
#include <vector>

#include "data.hpp"
#include "object.hpp"
#include "integer.hpp"
#include "string.hpp"

#include "functional_connection.hpp"
#include "functional_creator.hpp"
#include "functional_parser.hpp"
#include "json_data_parser.hpp"
#include "functional_serializer.hpp"
#include "json_data_serializer.hpp"
#include "functional_task.hpp"

#include "server.hpp"

using namespace server;
using namespace engine;
using namespace engine_utl;
using namespace mcu_control_utl;
using namespace mcu_control_utl;

using RawData = std::string;
using TestTime = int;

using TestServer = Server<RawData, TestTime>;
using EngineTask = typename TestServer::Engine::EngineTask;


TEST(ut_server, ctor_dtor_sanity) {
	// GIVEN
	FunctionalCreator<EngineTask *(const Data&)> task_creator(
		[](const Data& data)-> EngineTask * {
			return new FunctionalTask<Data *>(
				[](void)-> Data * {
					throw std::runtime_error("not needed");
				}
			);
		}
	);
	FunctionalCreator<Data *(const std::exception&)> fail_report_creator(
		[](const std::exception& e)-> Data * {
			(void)e;
			throw std::runtime_error("not needed");
		}
	);
	FunctionalParser<Data *(const RawData&)> parser(
		[](const RawData& data) -> Data * {
			(void)data;
			throw std::runtime_error("not needed");
		}
	);
	FunctionalSerializer<RawData(const Data&)> serializer(
		[](const Data& data) -> RawData {
			(void)data;
			throw std::runtime_error("not needed");
		}
	);
	FunctionalConnection<RawData, TestTime> connection(
		[](const RawData& data) {
			throw std::runtime_error("not needed");
		},
		[](const TestTime& delay)-> bool {
			throw std::runtime_error("not needed");
		},
		[]()-> RawData {
			throw std::runtime_error("not needed");
		}
	);

	// WHEN
	TestServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestServer(
				&connection,
				10,
				task_creator,
				fail_report_creator,
				parser,
				serializer
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}



TEST(ut_server, run_stop_sanity_negative) {
	// GIVEN
	const int fail_code(-100);
	const int success_code(0);

	FunctionalCreator<EngineTask *(const Data&)> task_creator(
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

	std::mutex data_mux;
	std::vector<RawData> datas;

	for (auto test_case: test_cases) {
		// WHEN
		FunctionalConnection<RawData, TestTime> connection(
			[parser, test_case](const RawData& data) {
				std::cout << "received report: " << std::endl << data << std::endl;
				auto parsed_result = Data::cast<Integer>(Data::cast<Object>(*std::unique_ptr<Data>(parser.parse(data))).access("result")).get();
				ASSERT_EQ(parsed_result, test_case.second);
			},
			[&data_mux, &datas](const TestTime& delay)-> bool {
				sleep(delay);
				std::lock_guard lock(data_mux);
				return !datas.empty();
			},
			[&data_mux, &datas]()-> RawData {
				std::lock_guard lock(data_mux);
				if (datas.empty()) {
					throw std::runtime_error("data buff is empty");
				}
				RawData data(*datas.begin());
				datas.erase(datas.begin());
				return data;
			}
		);
		TestServer instance(&connection, 10, task_creator, fail_report_creator, parser, serializer);

		RawData report("");
		std::thread data_sending_thread(
			[&data_mux, &datas, test_case, &instance](){
				sleep(1);
				datas.push_back(test_case.first);
				sleep(2);
				instance.stop();
			}
		);
		// THEN
		std::cout << "running TC with input:" << std::endl << test_case.first << std::endl;

		ASSERT_NO_THROW(instance.run());

	}
}