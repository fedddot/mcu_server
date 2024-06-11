#include <iostream>

#include "gtest/gtest.h"

#include "functional_connection.hpp"

#include "client.hpp"

using namespace client;
using namespace mcu_control_utl;
using namespace mcu_control_utl;

using RawData = std::string;
using SubscriberId = int;

using TestClient = Client<RawData, SubscriberId>;

static FunctionalConnection<int, RawData> s_connection(
	[](const RawData& data) {
		std::cout << "sending data: " << data << std::endl;
	},
	[]()-> int {
		static int s_last_id(0);
		return s_last_id++;
	}
);

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

// TEST(ut_client, run_stop_sanity_negative) {
// 	// GIVEN
// 	const int fail_code(-100);
// 	const int success_code(0);

// 	static FunctionalCreator<EngineTask *(const Data&)> task_creator(
// 		[fail_code, success_code](const Data& data)-> EngineTask * {
// 			auto cfg_obj = Data::cast<Object>(data);
// 			auto cfg_code = Data::cast<Integer>(cfg_obj.access("cfg_code")).get();
// 			return new FunctionalTask<Data *>(
// 				[cfg_code, fail_code, success_code](void)-> Data * {
// 					if (fail_code == cfg_code) {
// 						throw std::runtime_error("fail code received");
// 					}
// 					Object report;
// 					report.add("result", Integer(success_code));
// 					return report.clone();
// 				}
// 			);
// 		}
// 	);

// 	FunctionalCreator<Data *(const std::exception&)> fail_report_creator(
// 		[fail_code](const std::exception& e)-> Data * {
// 			Object report;
// 			report.add("result", Integer(fail_code));
// 			report.add("what", String(std::string(e.what())));
// 			return report.clone();
// 		}
// 	);

// 	FunctionalParser<Data *(const RawData&)> parser(
// 		[](const RawData& data) -> Data * {
// 			return JsonDataParser().parse(data).clone();
// 		}
// 	);

// 	FunctionalSerializer<RawData(const Data&)> serializer(
// 		[](const Data& data) -> RawData {
// 			return JsonDataSerializer().serialize(Data::cast<Object>(data));
// 		}
// 	);

// 	const std::map<RawData, int> test_cases {
// 		{"{\"cfg_code\": " + std::to_string(fail_code) + "}", fail_code},
// 		{"{\"cfg_code\": " + std::to_string(success_code) + "}", success_code},
// 		{"{\"cfg_code_gaga\": " + std::to_string(success_code) + "}", fail_code},
// 		{"{\"cfg_code\": \"" + std::to_string(success_code) + "\"}", fail_code}
// 	};

// 	for (auto test_case: test_cases) {
// 		// WHEN
// 		FunctionalConnection<int, RawData> connection(
// 			[parser, test_case](const RawData& data) {
// 				std::cout << "received report: " << std::endl << data << std::endl;
// 				auto parsed_result = Data::cast<Integer>(Data::cast<Object>(*std::unique_ptr<Data>(parser.parse(data))).access("result")).get();
// 				ASSERT_EQ(parsed_result, test_case.second);
// 			},
// 			[]()-> int {
// 				static int s_last_id(0);
// 				return s_last_id++;
// 			}
// 		);
// 		TestClient instance(task_creator, fail_report_creator, parser, serializer, &connection);
// 		RawData report("");

// 		// THEN
// 		std::cout << "running TC with input:" << std::endl << test_case.first << std::endl;
// 		ASSERT_NO_THROW(instance.run());
// 		ASSERT_TRUE(instance.is_running());
// 		ASSERT_NO_THROW(connection.post_data(test_case.first));
// 		ASSERT_NO_THROW(instance.stop());
// 	}
// }