#include "gtest/gtest.h"
#include <iostream>
#include <stdexcept>
#include <string>

#include "data_extractor.hpp"
#include "data_sender.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "mcu_server.hpp"
#include "mcu_server_fixture.hpp"

using namespace mcu_server;
using namespace mcu_server_uts;
using namespace mcu_server_utl;

using GpioId = typename McuServerFixture::GpioId;
using RawData = typename McuServer<GpioId>::RawData;

class TestExtractor: public DataExtractor<RawData> {
public:
	
};

class TestSender: public DataSender<RawData> {
public:
	TestSender(const RawData& header, const RawData& tail): m_header(header), m_tail(tail) {

	}
	TestSender(const TestSender& other) = default;
	TestSender& operator=(const TestSender& other) = default;

	void send(const RawData& data) const override {
		std::cout << "sending data: " << data << std::endl;
	}
	DataSender<RawData> *clone() const override {
		return new TestSender(*this);
	}
private:
	RawData m_header;
	RawData m_tail;
};

TEST_F(McuServerFixture, ctor_dtor_sanity) {
	// WHEN
	McuServer<GpioId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new McuServer<GpioId>(
				mcu_engine(),
				TestSender("aadd", "ssgg"),
				TestExtractor("aadd", "ssgg"),
				JsonDataParser(),
				JsonDataSerializer()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}