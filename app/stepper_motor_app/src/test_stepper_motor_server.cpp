#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"

#include "cpprest/http_msg.h"
#include "http_ipc_server.hpp"
#include "pplx/pplxtasks.h"

#include "host.hpp"
#include "ipc_server_factory.hpp"
#include "manager.hpp"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"
#include "test_stepper_motor_creator.hpp"
#include "test_stepper_motor_delay_generator.hpp"

using namespace manager;
using namespace host;
using namespace ipc;

using ProvidersConfig = std::string;
using StepperCreateCfg = std::string;

using StepperMotorHost = Host<StepperMotorRequest<StepperCreateCfg>, StepperMotorResponse, StepperMotorManagerConfig>;

static Manager<StepperMotorRequest<StepperCreateCfg>, StepperMotorResponse> *create_manager(const StepperMotorManagerConfig& cfg);
static StepperMotorRequest<StepperCreateCfg> transform_to_request(const web::http::http_request& http_request);
static web::http::http_response transform_to_response(const StepperMotorResponse& response);

int main(void) {
    auto ipc_config = HttoIpcServerConfig<StepperMotorRequest<StepperCreateCfg>, StepperMotorResponse>();
    ipc_config.uri = "http://127.0.0.1:5555";
    ipc_config.polling_timeout_s = 1;
    ipc_config.response_timeout_s = 60;
    ipc_config.to_request = transform_to_request;
    ipc_config.to_response = transform_to_response;
    
    auto host = StepperMotorHost(
        IpcServerFactory<StepperMotorRequest<StepperCreateCfg>, StepperMotorResponse>(),
        ipc_config,
        create_manager,
        "",
        [](const std::exception& e) {
            auto response = StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
            response.set_message(std::string(e.what()));
            return response;
        }
    );

    host.run();

    return 0;
}

static inline StepperMotorId retrieve_motor_id(const Json::Value& root) {
    const auto value_ptr = root.find("motor_id");
    if (!value_ptr || !(value_ptr->isString())) {
        throw std::invalid_argument("motor_id not found or has invalid format");
    }
    return static_cast<StepperMotorId>(value_ptr->asString());
}

inline static typename StepperMotorRequest<StepperCreateCfg>::Type retrieve_request_type(const web::http::http_request& http_request) {
    using ReqType = typename StepperMotorRequest<StepperCreateCfg>::Type;
    const auto type_map = std::map<std::string, ReqType> {
        {"POST",  ReqType::CREATE_STEPPER},
        {"DEL",  ReqType::DELETE_STEPPER},
        {"GET",    ReqType::READ_STEPPER},
        {"PUT",           ReqType::STEPS},
    };
    const auto iter = type_map.find(http_request.method());
    if (type_map.end() == iter) {
        throw std::invalid_argument("unsupported request type");
    }
    return iter->second;
}

inline static typename StepperMotorRequest<StepperCreateCfg>::Steps retrieve_steps(const Json::Value& steps_cfg) {
    using ReqSteps = typename StepperMotorRequest<StepperCreateCfg>::Steps;
    const auto type_map = std::map<std::string, StepperMotorDirection> {
        {"CW",  StepperMotorDirection::CW},
        {"CCW", StepperMotorDirection::CCW},
    };
    const auto direction_ptr = steps_cfg.find("direction");
    if (!direction_ptr || !(direction_ptr->isString())) {
        throw std::invalid_argument("direction not found or has invalid format");
    }
    const auto iter = type_map.find(direction_ptr->asString());
    if (type_map.end() == iter) {
        throw std::invalid_argument("unsupported direction");
    }
    const auto steps_number_ptr = steps_cfg.find("steps_number");
    if (!steps_number_ptr || !(steps_number_ptr->isInt())) {
        throw std::invalid_argument("steps_number not found or has invalid format");
    }
    const auto step_duration_ptr = steps_cfg.find("step_duration");
    if (!step_duration_ptr || !(step_duration_ptr->isInt())) {
        throw std::invalid_argument("step_duration not found or has invalid format");
    }
    return ReqSteps {
        .direction = iter->second,
        .steps_number = static_cast<std::size_t>(steps_number_ptr->asInt()),
        .step_duration = static_cast<std::size_t>(step_duration_ptr->asInt()),
    };
}

inline StepperMotorRequest<StepperCreateCfg> transform_to_request(const web::http::http_request& http_request) {
    const auto request_task = http_request.extract_vector();
    if (pplx::task_status::completed != request_task.wait()) {
        throw std::runtime_error("failed to extract request data");
    }
    const auto raw_data = request_task.get();
    auto reader = Json::Reader();
    auto root = Json::Value();
    if (!reader.parse(std::string(raw_data.begin(), raw_data.end()), root)) {
        throw std::invalid_argument("failed to parse data from input");
    }
    if (!root.isObject()) {
        throw std::invalid_argument("wrong data format");
    }

    const auto request_type = retrieve_request_type(http_request);
    const auto motor_id = retrieve_motor_id(root);

    auto request = StepperMotorRequest<StepperCreateCfg>(request_type, motor_id);
    const auto create_cfg_ptr = root.find("create_cfg");
    if (create_cfg_ptr && create_cfg_ptr->isString()) {
        request.set_create_config(static_cast<StepperCreateCfg>(create_cfg_ptr->asString()));
    }
    const auto steps_cfg_ptr = root.find("steps_cfg");
    if (steps_cfg_ptr && steps_cfg_ptr->isObject()) {
        request.set_steps(retrieve_steps(*steps_cfg_ptr));
    }
    return request;
}

inline web::http::http_response transform_to_response(const StepperMotorResponse& response) {
    using RespRes = typename StepperMotorResponse::ResultCode;
    const auto type_map = std::map<RespRes, web::http::status_code> {
        {RespRes::OK,           web::http::status_codes::OK},
        {RespRes::BAD_REQUEST,  web::http::status_codes::BadRequest},
        {RespRes::NOT_FOUND,    web::http::status_codes::NotFound},
        {RespRes::UNSUPPORTED,  web::http::status_codes::MethodNotAllowed},
        {RespRes::EXCEPTION,    web::http::status_codes::InternalError},
    };

    const auto iter = type_map.find(response.code());
    if (type_map.end() == iter) {
        throw std::invalid_argument("unsupported result code");
    }
    auto http_resp = web::http::http_response(iter->second);
    auto resp_body = Json::Value();
    if (response.has_message()) {
        resp_body["message"] = response.message();
    }
    if (response.has_state()) {
        resp_body["state"] = static_cast<int>(response.state());
    }
    const auto resp_body_raw = Json::FastWriter().write(resp_body);
    http_resp.set_body(resp_body_raw, utf8string("application/json; charset=utf-8"));
    return http_resp;
}

Manager<StepperMotorRequest<StepperCreateCfg>, StepperMotorResponse> *create_manager(const StepperMotorManagerConfig& cfg) {
    using namespace manager_tests;
    (void)cfg;

    return new StepperMotorManager<StepperCreateCfg>(
        TestStepperMotorCreator<StepperCreateCfg>(
            [](const StepperMotorDirection& dir) {
                auto dir_str = std::string("CCW");
                if (StepperMotorDirection::CW == dir) {
                    dir_str = "CW";
                }
                std::cout << "motor steps in direction: " << dir_str << std::endl;
            }
        ),
        TestStepperMotorDelayGenerator()
    );
}