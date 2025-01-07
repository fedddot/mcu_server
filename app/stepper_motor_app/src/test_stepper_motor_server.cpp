#include <exception>
#include <stdexcept>
#include <string>

#include "json/reader.h"
#include "json/value.h"

#include "host.hpp"
#include "ipc_connection.hpp"
#include "manager.hpp"
#include "providers.hpp"
#include "stepper_motor_http_ipc_server.hpp"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_request_data.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"

using namespace manager;
using namespace host;
using namespace ipc;

using ProvidersConfig = std::string;
using IpcConfig = std::string;
using StepperCreateCfg = std::string;

using StepperMotorHost = Host<StepperMotorRequest, StepperMotorResponse, StepperMotorProviderType, ProvidersConfig, StepperMotorManagerConfig, IpcConfig>;

static IpcConnection<StepperMotorRequest, StepperMotorResponse> *create_ipc(const IpcConfig& cfg);
static Manager<StepperMotorRequest, StepperMotorResponse> *create_manager(Providers<StepperMotorProviderType> *providers, const StepperMotorManagerConfig& cfg);
static Providers<StepperMotorProviderType> *create_providers(const ProvidersConfig& cfg);

int main(void) {
    auto host = StepperMotorHost(
        create_ipc,
        "",
        create_manager,
        "",
        create_providers,
        "",
        [](const std::exception&) {
            return StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
        }
    );

    host.run();

    return 0;
}

static StepperMotorCreateRequestData<StepperMotorId, StepperCreateCfg> parse_create_request(const std::vector<char>& raw_data) {
    auto reader = Json::Reader();
    auto root = Json::Value();
    if (!reader.parse(std::string(raw_data.begin(), raw_data.end()), root)) {
        throw std::invalid_argument("failed to parse data from input");
    }
    if (!root.isObject()) {
        throw std::invalid_argument("wrong data format");
    }
    const auto id_ptr = root.find("id");
    if (!id_ptr || !(id_ptr->isString())) {
        throw std::invalid_argument("id not found or has invalid format");
    }
    const auto create_cfg_ptr = root.find("create_cfg");
    if (!create_cfg_ptr || !(create_cfg_ptr->isString())) {
        throw std::invalid_argument("create_cfg not found or has invalid format");
    }
    return StepperMotorCreateRequestData<StepperMotorId, StepperCreateCfg>(
        static_cast<StepperMotorId>(id_ptr->asString()),
        static_cast<StepperCreateCfg>(create_cfg_ptr->asString())
    );
}

IpcConnection<StepperMotorRequest, StepperMotorResponse> *create_ipc(const IpcConfig& cfg) {
    // TODO: retrieve the params from cfg
    return new StepperMotorHttpIpcServer<StepperCreateCfg>(
        parse_create_request,
        "http://127.0.0.1:5555",
        2,
        100
    );
}

Manager<StepperMotorRequest, StepperMotorResponse> *create_manager(Providers<StepperMotorProviderType> *providers, const StepperMotorManagerConfig& cfg) {
    (void)cfg;
    return new StepperMotorManager<StepperCreateCfg>(providers);
}

class TestProviders: public Providers<StepperMotorProviderType> {
public:
    TestProviders() = default;
    TestProviders(const TestProviders&) = delete;
    TestProviders& operator=(const TestProviders&) = delete;
    
    manager::Provider& access_provider(const StepperMotorProviderType& id) const override {
        const auto iter = m_providers.find(id);
        if (m_providers.end() == iter) {
            throw std::invalid_argument("provider not found");
        }
        return std::ref(*(iter->second));
    }
private:
    mutable std::map<StepperMotorProviderType, std::unique_ptr<manager::Provider>> m_providers;
};

Providers<StepperMotorProviderType> *create_providers(const ProvidersConfig& cfg) {
    return new TestProviders();
}