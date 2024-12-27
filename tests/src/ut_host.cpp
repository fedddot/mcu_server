#include <string>

#include "gtest/gtest.h"

#include "host.hpp"

using namespace app;

using Request = std::string;
using Response = std::string;
using ManagerCfg = std::string;
using AppCfg = std::string;

using TestHost = Host<Request, Response, ManagerCfg, AppCfg>;

TEST(ut_host, sanity) {
	// WHEN:
	TestHost *instance(nullptr);
}