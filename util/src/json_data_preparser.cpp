#include "json_data_preparser.hpp"

using namespace mcu_server_utl;
using namespace mcu_server;

const std::map<std::string, std::string> mcu_server_utl::JsonDataPreParser::s_replacements {
	{"0", "ctor_id"},
	{"1", "gpio_id"},
	{"2", "gpio_dir"},
	{"3", "gpio_state"},
	{"4", "delay_ms"},
	{"5", "tasks"},
	{"6", "result"}
};