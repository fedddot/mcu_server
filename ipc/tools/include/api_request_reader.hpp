#ifndef	API_REQUEST_READER_HPP
#define	API_REQUEST_READER_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "ipc_data_reader.hpp"
#include "ipc_queue.hpp"
#include "package_reader.hpp"

namespace ipc {
	template <typename ApiRequest, std::size_t HSIZE>
	class ApiRequestReader: public DataReader<std::optional<ApiRequest>(void)> {
	public:
		using SizeRetriever = typename PackageReader<HSIZE>::SizeRetriever;
		using ApiRequestParser = std::function<ApiRequest(const std::vector<std::uint8_t>&)>;

		ApiRequestReader(
			IpcQueue<std::uint8_t> *queue_ptr,
			const SizeRetriever& size_retriever,
			const ApiRequestParser& request_parser
		): m_queue_ptr(queue_ptr), m_package_reader(queue_ptr, size_retriever), m_request_parser(request_parser) {
			if (!m_queue_ptr || !m_request_parser) {
				throw std::invalid_argument("invalid args received");
			}
		}
		ApiRequestReader(const ApiRequestReader&) = default;
		ApiRequestReader& operator=(const ApiRequestReader&) = default;
		std::optional<ApiRequest> read() const override {
			try {
				auto package_data = m_package_reader.read();
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
		PackageReader<HSIZE> m_package_reader;
		ApiRequestParser m_request_parser;
	};
}

#endif // API_REQUEST_READER_HPP