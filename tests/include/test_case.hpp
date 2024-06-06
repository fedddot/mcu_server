#ifndef	TEST_CASE_HPP
#define	TEST_CASE_HPP

#include "data.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_creator.hpp"
#include "server_parser.hpp"
#include "server_serializer.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace server_ut {
	class TestCase {
	public:
		TestCase(const std::string& title, const std::string& raw_data, int expected_result): m_title(title), m_raw_data(raw_data), m_expected_result(expected_result) {

		}

		void run() const {
			// WHEN
			RawData report("");

			// THEN
			std::cout << "TC: " << m_title << std::endl;
			std::cout << "sending data:" << std::endl << m_raw_data << std::endl;
			ASSERT_NO_THROW(report = s_instance->run(m_raw_data));
			std::cout << "received report:" << std::endl << report << std::endl;
			ASSERT_EQ(m_expected_result, server::Data::cast<server::Integer>(server::Data::cast<server::Object>(*std::unique_ptr<server::Data>(s_raw_data_parser.parse(report))).access("result")).get());
			std::cout << std::endl;
		}
		static void reset_instance();
	private:
		using RawData = std::string;
		using GpioId = int;
		
		std::string m_title;
		std::string m_raw_data;
		int m_expected_result;
		
		static server_utl::ServerParser<server::Data *(const RawData&)> s_raw_data_parser;
		static server_utl::ServerSerializer<RawData(const server::Data&)> s_raw_data_serializer;
		static server_utl::ServerCreator<server::Gpio *(const GpioId&, const server::Gpio::Direction&)> s_gpio_creator;
		static std::unique_ptr<server::Server<RawData, GpioId>> s_instance;
	};

	inline void TestCase::reset_instance() {
		s_instance = std::unique_ptr<server::Server<RawData, GpioId>>(
			new server::Server<RawData, GpioId>(
				s_raw_data_parser,
				s_raw_data_serializer,
				s_gpio_creator
			)
		);
	}
}
#endif // TEST_CASE_HPP