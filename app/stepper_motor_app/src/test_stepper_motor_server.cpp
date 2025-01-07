#include <exception>
#include <stdexcept>
#include <string>

#include "cpprest/http_msg.h"
#include "host.hpp"
#include "http_ipc_connection.hpp"
#include "ipc_connection.hpp"
#include "manager.hpp"
#include "providers.hpp"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
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

    host.run_once();

    return 0;
}

IpcConnection<StepperMotorRequest, StepperMotorResponse> *create_ipc(const IpcConfig& cfg) {
    return new HttpIpcConnection<StepperMotorRequest, StepperMotorResponse>(
        "http://127.0.0.1:5555",
		2,
		10,
		[](const web::http::http_request& request)-> StepperMotorRequest {
            throw std::runtime_error("NOT IMPLEMENTED");
        },
        [](const StepperMotorResponse& response)-> web::http::http_response {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
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