#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "data.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server_creator.hpp"
#include "server_parser.hpp"
#include "server_serializer.hpp"

#include "server.hpp"
#include "server_task.hpp"

using namespace server;
using namespace server_utl;

using InputData = std::string;
using ReportData = std::string;
using TestServer = Server<InputData, ReportData>;

TEST(ut_server, ctor_dtor_sanity) {
	// GIVEN
	ServerCreator<TestServer::ServerTask *(const Data& data)> factory(
		[](const Data& data)-> TestServer::ServerTask * {
			throw std::runtime_error("not implemented");
		}
	);
	ServerParser<Data *(const InputData&)> raw_data_parser(
		[](const InputData& data)-> Data * {
			throw std::runtime_error("not implemented");
		}
	);
	ServerSerializer<InputData(const Data&)> raw_data_serializer(
		[](const Data& data)-> InputData {
			throw std::runtime_error("not implemented");
		}
	);

	// WHEN
	TestServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestServer(
				raw_data_parser,
				raw_data_serializer,
				factory
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}

TEST(ut_server, run_task_sanity) {
	// GIVEN
	ServerCreator<TestServer::ServerTask *(const Data& data)> factory(
		[](const Data& data)-> TestServer::ServerTask * {
			return new ServerTask<Data *>(
				[&data]()-> Data * {
					Object report;
					report.add("result", Integer(Data::cast<Integer>(Data::cast<Object>(data).access("report_val"))));
					return report.clone();
				}
			);
		}
	);
	ServerParser<Data *(const InputData&)> raw_data_parser(
		[](const InputData& data)-> Data * {
			return JsonDataParser().parse(data).clone();
		}
	);
	
	ServerSerializer<InputData(const Data&)> raw_data_serializer(
		[](const Data& data)-> InputData {
			return JsonDataSerializer().serialize(Data::cast<Object>(data));
		}
	);
	auto test_cases = std::vector<std::pair<int, InputData>> {
		{0, "{\"report_val\":0}"},
		{1, "{\"report_val\":1}"},
		{2, "{\"report_val\":2}"}
	};

	// WHEN
	TestServer instance(
		raw_data_parser,
		raw_data_serializer,
		factory
	);

	for (auto test_case: test_cases) {
		// WHEN
		ReportData report("");

		// THEN
		ASSERT_NO_THROW(report = instance.run(test_case.second));
		std::unique_ptr<Data> report_parsed(raw_data_parser.parse(report));
		ASSERT_EQ(test_case.first, Integer(Data::cast<Integer>(Data::cast<Object>(*report_parsed).access("result"))).get());
	}
}