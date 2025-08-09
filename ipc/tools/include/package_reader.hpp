#ifndef	RAW_DATA_PACKAGE_READER_HPP
#define	RAW_DATA_PACKAGE_READER_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "ipc_queue.hpp"
#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename Header, typename HeaderReader>
	class PackageReader: public DataReader<std::optional<std::vector<std::uint8_t>>(void)> {
		static_assert(std::is_base_of<DataReader<std::optional<Header>(IpcQueue<std::uint8_t> *)>, HeaderReader>::value, "HeaderReader is not inheriting from DataReader<std::optional<Header>(IpcQueue<std::uint8_t> *)>");
	public:
		PackageReader(
			IpcQueue<std::uint8_t> *queue_ptr,
			HeaderReader&& header_reader
		): m_queue_ptr(queue_ptr), m_header_reader(header_reader) {
			if (!m_queue_ptr) {
				throw std::invalid_argument("invalid bytes queue ptr received");
			}
		}
		PackageReader(PackageReader&&) = default;
		PackageReader(const PackageReader&) = delete;
		PackageReader& operator=(const PackageReader&) = delete;
		
		std::optional<std::vector<std::uint8_t>> read() const override {
			try {
				if (!m_header) {
					const auto header = m_header_reader.read(m_queue_ptr);
					if (!header) {
						return std::nullopt;
					}
					if (!header.value().validate()) {
						m_queue_ptr->clear();
						return std::nullopt;
					}
					m_header = header;
				}
				const auto data_size = m_header.value().data_size();
				if (m_queue_ptr->size() < data_size) {
					return std::nullopt;
				}
				std::vector<std::uint8_t> package_data(data_size, 0);
				for (std::size_t i = 0; i < data_size; ++i) {
					package_data[i] = m_queue_ptr->dequeue();
				}
				m_header = std::nullopt;
				return package_data;
			} catch (...) {
				m_queue_ptr->clear();
				throw;
			}
		}
	private:
		IpcQueue<std::uint8_t> *m_queue_ptr;
		HeaderReader m_header_reader;
		std::optional<Header> m_header;
	};
}

#endif // RAW_DATA_PACKAGE_READER_HPP