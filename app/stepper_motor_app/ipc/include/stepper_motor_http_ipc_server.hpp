#ifndef	STEPPER_MOTOR_HTTP_IPC_SERVER_HPP
#define	STEPPER_MOTOR_HTTP_IPC_SERVER_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "cpprest/http_msg.h"
#include "ipc_connection.hpp"
#include "http_ipc_connection.hpp"
#include "pplx/pplxtasks.h"
#include "stepper_motor_request.hpp"
#include "stepper_motor_request_data.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"

namespace ipc {
	template <typename Tcreate_cfg>
	class StepperMotorHttpIpcServer: public IpcConnection<manager::StepperMotorRequest, manager::StepperMotorResponse> {
	public:
		using CreateRequestDataParser = std::function<manager::StepperMotorCreateRequestData<manager::StepperMotorId, Tcreate_cfg>(const std::vector<char>&)>;
		StepperMotorHttpIpcServer(
			const CreateRequestDataParser& create_request_parser,
			const std::string& uri,
			const unsigned int polling_timeout_s,
			const unsigned int response_timeout_s
		);
		StepperMotorHttpIpcServer(const StepperMotorHttpIpcServer&) = delete;
		StepperMotorHttpIpcServer& operator=(const StepperMotorHttpIpcServer&) = delete;
		
		void write(const manager::StepperMotorResponse& response) const override;
		bool readable() const override;
		manager::StepperMotorRequest read() override;
	private:
		CreateRequestDataParser m_create_request_parser;
		HttpIpcConnection<manager::StepperMotorRequest, manager::StepperMotorResponse> m_http_connection;
		manager::StepperMotorRequest http_request_to_stepper_request(const web::http::http_request& request) const;
		web::http::http_response stepper_response_to_http_response(const manager::StepperMotorResponse& response) const;
	};

	template <typename Tcreate_cfg>
	inline StepperMotorHttpIpcServer<Tcreate_cfg>::StepperMotorHttpIpcServer(
		const CreateRequestDataParser& create_request_parser,
		const std::string& uri,
		const unsigned int polling_timeout_s,
		const unsigned int response_timeout_s
	):
		m_create_request_parser(create_request_parser),
		m_http_connection(
			uri, polling_timeout_s, response_timeout_s,
			[this](const web::http::http_request& request) {
				return http_request_to_stepper_request(request);
			},
			[this](const manager::StepperMotorResponse& response) {
				return stepper_response_to_http_response(response);
			}
		) {

	}

	template <typename Tcreate_cfg>
	inline void StepperMotorHttpIpcServer<Tcreate_cfg>::write(const manager::StepperMotorResponse& response) const {
		m_http_connection.write(response);
	}

	template <typename Tcreate_cfg>
	inline bool StepperMotorHttpIpcServer<Tcreate_cfg>::readable() const {
		return m_http_connection.readable();
	}

	template <typename Tcreate_cfg>
	inline manager::StepperMotorRequest StepperMotorHttpIpcServer<Tcreate_cfg>::read() {
		return m_http_connection.read();
	}

	template <typename Tcreate_cfg>
	inline manager::StepperMotorRequest StepperMotorHttpIpcServer<Tcreate_cfg>::http_request_to_stepper_request(const web::http::http_request& request) const {
		using namespace manager;

		auto generate_create_request_data = [this](const web::http::http_request& request) {
			auto data_retrieve_task = request.extract_vector();
			if (pplx::task_status::completed != data_retrieve_task.wait()) {
				throw std::runtime_error("failed to retrieve request data");
			}
			const auto raw_data = data_retrieve_task.get();
			return m_create_request_parser(std::vector<char>(raw_data.begin(), raw_data.end())); 
		};

		if ("POST" == request.method()) {
			auto motor_request = StepperMotorRequest(StepperMotorRequest::Type::CREATE_STEPPER);
			motor_request.set_data(generate_create_request_data(request));
			return motor_request;
		}
		
		throw std::invalid_argument("unsupported http method");
	}

	template <typename Tcreate_cfg>
	inline web::http::http_response StepperMotorHttpIpcServer<Tcreate_cfg>::stepper_response_to_http_response(const manager::StepperMotorResponse& response) const {
		throw std::runtime_error("NOT IMPLEMENTED: stepper_response_to_http_response");
	}
}

#endif // STEPPER_MOTOR_HTTP_IPC_SERVER_HPP