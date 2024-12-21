#include <stdexcept>

#include "gtest/gtest.h"

#include "cnc_server.hpp"
#include "custom_data_reader.hpp"
#include "custom_data_writer.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_types.hpp"

using namespace server;
using namespace cnc_server;
using namespace manager;
using namespace ipc;
using namespace vendor;

TEST(ut_cnc_server, ctor_dtor_sanity) {
    // GIVEN
    CustomDataReader<Request> reader(
        []() {
            return Request(Request::Method::CREATE, {}, Body());
        },
        []() {
            return false;
        }
    );
    CustomDataWriter<Response> writer(
        [](const Response&) {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    CncServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new CncServer(
            &reader,
            &writer,
            [](const Data&)-> Gpio * {
                throw std::runtime_error("NOT IMPLEMENTED");
            },
            [](const double) {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        )
    );
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
}