#include <condition_variable>
#include <mutex>
#include <string>

#include "gtest/gtest.h"

#include "http_ipc_connection.hpp"
#include "integer.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_types.hpp"

using namespace server;
using namespace server_utl;

TEST(ut_http_ipc_connection, sanity) {
	// GIVEN:
	const unsigned int timeout_s = 10;
	const std::string uri = "http://127.0.0.1:5000";
	const std::string subs_id = "subscriber";

	std::mutex mux;
	std::condition_variable cond;

	auto callback = [&mux, &cond](const Request& request) {
		std::unique_lock lock(mux);
		std::cout << "received request" << std::endl;
		cond.notify_one(); 
	};
	
	// WHEN:
	HttpIpcConnection<std::string> *instance_ptr(nullptr);

	// THEN:
	ASSERT_NO_THROW(instance_ptr = new HttpIpcConnection<std::string>(uri, timeout_s));
	ASSERT_NO_THROW(instance_ptr->subscribe(subs_id, callback));
	
	std::unique_lock lock(mux);
	cond.wait(lock);

	Body response_body;
	response_body.add("val", Integer(15));
	instance_ptr->send(Response(ResponseCode::OK, response_body));

	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
}