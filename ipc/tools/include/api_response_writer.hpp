#ifndef	API_REQUEST_WRITER_HPP
#define	API_REQUEST_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "ipc_data_writer.hpp"
#include "package_writer.hpp"

namespace ipc {
	template <typename ApiResponse, std::size_t HSIZE>
	class ApiResponseWriter: public DataWriter<ApiResponse> {
	public:
		using SizeRetriever = typename PackageWriter<HSIZE>::HeaderGenerator;
		using ApiResponseParser = std::function<ApiResponse(const std::vector<std::uint8_t>&)>;

		ApiResponseWriter(
			IpcQueue<std::uint8_t> *queue_ptr,
			const SizeRetriever& size_retriever,
			const ApiResponseParser& request_parser
		): m_queue_ptr(queue_ptr), m_package_writer(queue_ptr, size_retriever), m_request_parser(request_parser) {
			if (!m_queue_ptr || !m_request_parser) {
				throw std::invalid_argument("invalid args received");
			}
		}
		ApiResponseWriter(const ApiResponseWriter&) = default;
		ApiResponseWriter& operator=(const ApiResponseWriter&) = default;
		std::optional<ApiResponse> read() const override {
			try {
				auto package_data = m_package_writer.read();
				if (!package_data) {
					return std::nullopt;
				}
				return m_request_parser(*package_data);
			} catch (...) {
				m_queue_ptr->clear();
				throw;
			}
		}
	private:
		IpcQueue<std::uint8_t> *m_queue_ptr;
		PackageWriter<HSIZE> m_package_writer;
		ApiResponseParser m_request_parser;
	};
}

#endif // API_REQUEST_WRITER_HPP