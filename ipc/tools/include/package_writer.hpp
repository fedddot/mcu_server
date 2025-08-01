#ifndef	RAW_DATA_PACKAGE_WRITER_HPP
#define	RAW_DATA_PACKAGE_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "ipc_queue.hpp"
#include "ipc_data_writer.hpp"

namespace ipc {
	template <std::size_t HSIZE>
	class PackageWriter: public DataWriter<std::vector<std::uint8_t>> {
	public:
		using SizeRetriever = std::function<std::size_t(const IpcQueue<std::uint8_t>&)>;
		PackageWriter(
			IpcQueue<std::uint8_t> *queue_ptr,
			const SizeRetriever& size_retriever
		): m_queue_ptr(queue_ptr), m_size_retriever(size_retriever) {
			if (!m_queue_ptr || !m_size_retriever) {
				throw std::invalid_argument("invalid args in package writer received");
			}
		}
		PackageWriter(const PackageWriter&) = default;
		PackageWriter& operator=(const PackageWriter&) = default;
		std::optional<std::vector<std::uint8_t>> read() const override {
			try {
				if (m_queue_ptr->size() < HSIZE) {
					return std::nullopt;
				}
				const auto package_size = m_size_retriever(*m_queue_ptr);
				if (m_queue_ptr->size() < package_size + HSIZE) {
					return std::nullopt;
				}
				auto bytes_to_discard = HSIZE;
				while (bytes_to_discard) {
					m_queue_ptr->dequeue();
					--bytes_to_discard;
				}
				std::vector<std::uint8_t> package_data(package_size, 0);
				for (std::size_t i = 0; i < package_size; ++i) {
					package_data[i] = m_queue_ptr->dequeue();
				}
				return package_data;
			} catch (...) {
				m_queue_ptr->clear();
				throw;
			}
		}
	private:
		IpcQueue<std::uint8_t> *m_queue_ptr;
		SizeRetriever m_size_retriever;
	};
}

#endif // RAW_DATA_PACKAGE_WRITER_HPP