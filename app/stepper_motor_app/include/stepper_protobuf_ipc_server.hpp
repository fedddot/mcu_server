#ifndef	STEPPER_PROTOBUF_IPC_SERVER_HPP
#define	STEPPER_PROTOBUF_IPC_SERVER_HPP

#include <cstddef>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "stepper_service.pb.h"

#include "ipc_option.hpp"
#include "ipc_server.hpp"

#include "protobuf_ipc_server.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace ipc {
	template <typename Tcreate_cfg>
	class StepperProtobufIpcServer: public IpcServer<manager::StepperMotorRequest<Tcreate_cfg>, manager::StepperMotorResponse> {
	public:
		using ByteWriter = std::function<void(const std::vector<char>&)>;
		using Handler = typename IpcServer<manager::StepperMotorRequest<Tcreate_cfg>, manager::StepperMotorResponse>::Handler;

		StepperProtobufIpcServer(
			const ByteWriter& byte_writer
		);
		StepperProtobufIpcServer(const StepperProtobufIpcServer&) = delete;
		StepperProtobufIpcServer& operator=(const StepperProtobufIpcServer&) = delete;

		void serve(const Handler& handler) override;
		void stop() override;

		void serve_once(const Handler& handler);
		void feed(const pb_byte_t byte);
	private:
		using Request = manager::StepperMotorRequest<Tcreate_cfg>;
		using Response = manager::StepperMotorResponse;

		ByteWriter m_byte_writer;
		std::unique_ptr<ProtobufIpcServer<Request, Response, N>> m_pb_server;

		Option<Request> read_request(pb_istream_t *input_stream);
		void write_response(const Response& response); 
		
		static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
		static bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg);
		static stepper_service_ResultCode cast_result_code(const Response::ResultCode& code);
		static typename Request::Type cast_operation_code(const stepper_service_Operation& operation);
	};

	template <typename Tcreate_cfg>
	inline StepperProtobufIpcServer<Tcreate_cfg, N>::StepperProtobufIpcServer(
		const ByteWriter& byte_writer
	): m_byte_writer(byte_writer), m_pb_server(nullptr) {
		m_pb_server = std::make_unique<ProtobufIpcServer<Request, Response, N>>(
			[this](pb_istream_t *input_stream) {
				return read_request(input_stream);
			},
			[this](const Response& response) {
				write_response(response);
			}
		);
	}

	template <typename Tcreate_cfg>
	inline void StepperProtobufIpcServer<Tcreate_cfg, N>::serve(const Handler& handler) {
		m_pb_server->serve(handler);
	}

	template <typename Tcreate_cfg>
	inline void StepperProtobufIpcServer<Tcreate_cfg, N>::serve_once(const Handler& handler) {
		m_pb_server->serve_once(handler);
	}
	
	template <typename Tcreate_cfg>
	inline void StepperProtobufIpcServer<Tcreate_cfg, N>::stop() {
		m_pb_server->stop();
	}

	template <typename Tcreate_cfg>
	inline void StepperProtobufIpcServer<Tcreate_cfg, N>::feed(const pb_byte_t ch) {
		m_pb_server->feed(ch);
	}

	template <typename Tcreate_cfg>
	inline Option<typename StepperProtobufIpcServer<Tcreate_cfg, N>::Request> StepperProtobufIpcServer<Tcreate_cfg, N>::read_request(pb_istream_t *input_stream) {
		enum: int { MAX_STR_LEN = 0xFF };
		char read_buff[MAX_STR_LEN] = { '\0' };
		auto proto_request = stepper_service_StepperRequest {};
		proto_request.motor_id.funcs.decode = decode_string;
		proto_request.motor_id.arg = read_buff;
		if (!pb_decode_delimited(input_stream, stepper_service_StepperRequest_fields, &proto_request)) {
			return Option<Request>(nullptr);
		}
		const auto motor_id = std::string((const char *)proto_request.motor_id.arg);
		return Option<Request>(
			new Request(
				cast_operation_code(proto_request.operation),
				motor_id
			)
		);
	}

	template <typename Tcreate_cfg>
	inline void StepperProtobufIpcServer<Tcreate_cfg, N>::write_response(const StepperProtobufIpcServer<Tcreate_cfg, N>::Response& response) {
		auto grpc_response = stepper_service_StepperResponse {};
		grpc_response.code = cast_result_code(response.code());
		auto message = std::string("");
		if (response.has_message()) {
			message = response.message();
		}
		grpc_response.message.arg = const_cast<char *>(message.c_str());
		grpc_response.message.funcs.encode = encode_string;

		auto grpc_response_size = std::size_t(0UL);
		if (!pb_get_encoded_size(&grpc_response_size, stepper_service_StepperResponse_fields, &grpc_response)) {
			throw std::runtime_error("failed to evaluate response size");
		}
		grpc_response_size++;
		auto buff = std::unique_ptr<pb_byte_t[]>(new pb_byte_t[grpc_response_size]);
		auto buff_stream = pb_ostream_from_buffer(buff.get(), grpc_response_size);
		if (!pb_encode_delimited(&buff_stream, stepper_service_StepperResponse_fields, &grpc_response)) {
			throw std::runtime_error("failed to encode response");
		}
		const auto raw_data = std::vector<char>((const char *)(buff.get()), (const char *)(buff.get() + grpc_response_size));
		m_byte_writer(raw_data);
	}

	template <typename Tcreate_cfg>
	inline bool StepperProtobufIpcServer<Tcreate_cfg, N>::encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
		const auto string_casted = static_cast<const char *>(*arg);
		const auto string_size = std::strlen(string_casted);
		if (!pb_encode_tag_for_field(stream, field)) {
			return false;
		}
		return pb_encode_string(stream, static_cast<const pb_byte_t *>(*arg), string_size);
	}

	template <typename Tcreate_cfg>
	inline bool StepperProtobufIpcServer<Tcreate_cfg, N>::decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg) {
		auto string_field = std::string("");
		while (stream->bytes_left) {
			pb_byte_t buff = '\0';
			if (!pb_read(stream, &buff, 1UL)) {
				return false;
			}
			if ('\0' == buff) {
				break;
			}
			string_field.push_back(buff);
		}
		string_field.copy((char *)*arg, string_field.size());
		return true;
	}

	template <typename Tcreate_cfg>
	inline stepper_service_ResultCode StepperProtobufIpcServer<Tcreate_cfg, N>::cast_result_code(const Response::ResultCode& code) {
		const auto result_code_mapping = std::map<typename Response::ResultCode, stepper_service_ResultCode> {
			{Response::ResultCode::OK,			stepper_service_ResultCode_OK},
			{Response::ResultCode::BAD_REQUEST,	stepper_service_ResultCode_BAD_REQUEST},
			{Response::ResultCode::NOT_FOUND,	stepper_service_ResultCode_NOT_FOUND},
			{Response::ResultCode::EXCEPTION,	stepper_service_ResultCode_EXCEPTION},
			{Response::ResultCode::UNSUPPORTED,	stepper_service_ResultCode_UNSUPPORTED},
		};
		const auto iter = result_code_mapping.find(code);
		if (result_code_mapping.end() == iter) {
			throw std::invalid_argument("failed to cast result code: unsupported code");
		}
		return iter->second;
	}
	
	template <typename Tcreate_cfg>
	inline typename StepperProtobufIpcServer<Tcreate_cfg, N>::Request::Type StepperProtobufIpcServer<Tcreate_cfg, N>::cast_operation_code(const stepper_service_Operation& operation) {
		const auto operation_mapping = std::map<stepper_service_Operation, typename Request::Type> {
			{stepper_service_Operation_CREATE,  Request::Type::CREATE_STEPPER},
			{stepper_service_Operation_READ,    Request::Type::READ_STEPPER},
			{stepper_service_Operation_UPDATE,  Request::Type::STEPS},
			{stepper_service_Operation_DELETE,  Request::Type::DELETE_STEPPER},
		};
		const auto iter = operation_mapping.find(operation);
		if (operation_mapping.end() == iter) {
			throw std::invalid_argument("failed to cast operation code: unsupported code");
		}
		return iter->second;
	}
}

#endif // STEPPER_PROTOBUF_IPC_SERVER_HPP