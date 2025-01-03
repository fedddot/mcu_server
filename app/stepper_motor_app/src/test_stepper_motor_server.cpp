#include <exception>
#include <string>

#include "host.hpp"
// #include "http_ipc_connection.hpp"
#include "ipc_connection.hpp"
#include "manager.hpp"
#include "providers.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"

using namespace manager;
using namespace host;
using namespace ipc;

using ProvidersConfig = std::string;
using IpcConfig = std::string;

using StepperMotorHost = Host<StepperMotorRequest, StepperMotorResponse, StepperMotorProviderType, ProvidersConfig, StepperMotorManagerConfig, IpcConfig>;

static IpcConnection<StepperMotorRequest, StepperMotorResponse> *create_ipc(const IpcConfig& cfg);
static Manager<StepperMotorRequest, StepperMotorResponse, StepperMotorProviderType> *create_manager(Providers<StepperMotorProviderType> *providers, const StepperMotorManagerConfig& cfg);
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
            return StepperMotorResponse(StepperMotorResponseCode::EXCEPTION);
        }
    );
}