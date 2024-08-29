#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include <exception>
#include <memory>

#include "gtest/gtest.h"
#include <stdexcept>

#include "custom_creator.hpp"
#include "data.hpp"
#include "server.hpp"

namespace server_uts {
	class ServerFixture: public testing::Test {
	public:
		using ServerTask = typename server::Server::ServerTask;
		using TaskFactory = typename server::Server::TaskFactory;
		using FailureReportCreator = typename server::Server::FailureReportCreator;
		
		ServerFixture() = default;
		ServerFixture(const ServerFixture& other) = delete;
		ServerFixture& operator=(const ServerFixture& other) = delete;

		void SetUp() override {
			using namespace server;
			using namespace server_utl;
			m_fail_report_creator = std::unique_ptr<FailureReportCreator>(
				new CustomCreator<Data *(const std::exception&)>(
					[](const std::exception& e)-> Data * {
						throw std::runtime_error("NOT IMPLEMENTED");
					}
				)
			);
			m_factory = std::unique_ptr<TaskFactory>(
				new CustomCreator<ServerTask *(const Data&)>(
					[](const Data& data)-> ServerTask * {
						throw std::runtime_error("NOT IMPLEMENTED");
					}
				)
			);
		}
	
		const TaskFactory& factory() const {
			return *m_factory;
		}

		const FailureReportCreator& fail_report_creator() const {
			return *m_fail_report_creator;
		}
	private:
		std::unique_ptr<TaskFactory> m_factory;
		std::unique_ptr<FailureReportCreator> m_fail_report_creator;
	};
}

#endif