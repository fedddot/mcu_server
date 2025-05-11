#include <cstdlib>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"

#include "request_json_transformer.hpp"

using namespace ipc;
using namespace manager;

using AxisControllerConfig = std::string;

static Json::Value cfg2json(const AxisControllerConfig& cfg);
static AxisControllerConfig json2cfg(const Json::Value& cfg);
static bool doubles_equal(const double one, const double other, const double tolerance);

TEST(ut_request_json_transformer, ctor_dtor_sanity) {
	// WHEN
	RequestJsonTransformer<AxisControllerConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new RequestJsonTransformer<AxisControllerConfig>(cfg2json, json2cfg));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

inline Json::Value cfg2json(const AxisControllerConfig& cfg) {
	throw std::runtime_error("NOT IMPLEMENTED");
}

inline AxisControllerConfig json2cfg(const Json::Value& cfg) {
	throw std::runtime_error("NOT IMPLEMENTED");
}

inline bool doubles_equal(const double one, const double other, const double tolerance) {
	return std::abs(one - other) <= std::abs(tolerance);
}