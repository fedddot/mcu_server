#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include <thread>
#include <unistd.h>

#include "client.hpp"
#include "functional_connection.hpp"

using namespace client;
using namespace mcu_control_utl;

using RawData = std::string;
using TestClient = Client<RawData>;


TEST(ut_client, ctor_dtor_sanity) {
	// GIVEN
	FunctionalConnection<RawData> test_connection(
		[](const RawData& data) {
			(void)(data);
		},
		[]() {
			return false;
		},
		[]() {
			return RawData("");
		} 
	);
	
	// WHEN
	TestClient *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestClient(&test_connection)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}

TEST(ut_client, run_sanity) {
	// GIVEN
	bool is_readable(false);
	std::mutex mux;
	std::condition_variable cond;
	const RawData expected_report("{\"result\": 0}");
	const RawData test_data("{\"data\": \"data\"}");
	FunctionalConnection<RawData> test_connection(
		[&](const RawData& data) {
			std::cout << "client sends data: " << data << std::endl;
		},
		[&]() {
			while (true) {
				std::unique_lock lock(mux);
				if (is_readable) {
					return true;
				}
				cond.wait(lock);
			}
			return is_readable;
		},
		[&]() {
			std::unique_lock lock(mux);
			if (!is_readable) {
				throw std::runtime_error("is not readable");
			}
			std::cout << "client reads data: " << expected_report << std::endl;
			return expected_report;
		}
	);

	// WHEN
	TestClient instance(&test_connection);
	RawData report("");

	// THEN
	std::thread run_thread(
		[&](TestClient *client) {
			ASSERT_NO_THROW(report = client->run(test_data));
		},
		&instance
	);
	std::unique_lock lock(mux);
	is_readable = true;
	cond.notify_one();
	lock.unlock();
	run_thread.join();
	ASSERT_EQ(expected_report, report);
}