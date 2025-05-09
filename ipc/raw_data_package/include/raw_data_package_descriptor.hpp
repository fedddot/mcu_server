#ifndef	RAW_DATA_PACKAGE_DESCRIPTOR_HPP
#define	RAW_DATA_PACKAGE_DESCRIPTOR_HPP

#include <cstddef>
#include <stdexcept>

#include "ipc_data.hpp"

namespace ipc {
	class RawDataPackageDescriptor {
	public:
		RawDataPackageDescriptor(
			const RawData& preamble,
			const std::size_t& encoded_size_length
		);
		virtual ~RawDataPackageDescriptor() noexcept = default;
		RawData preamble() const;
		std::size_t encoded_size_length() const;
	private:
		RawData m_preamble;
		std::size_t m_encoded_size_length;
	};

	inline RawDataPackageDescriptor::RawDataPackageDescriptor(
		const RawData& preamble,
		const std::size_t& encoded_size_length
	): m_preamble(preamble), m_encoded_size_length(encoded_size_length) {
		if (m_preamble.empty()) {
			throw std::invalid_argument("empty preamble received");
		}
		if (m_encoded_size_length < 1) {
			throw std::invalid_argument("encoded size length must be at least 1");
		}
	}

	inline RawData RawDataPackageDescriptor::preamble() const {
		return m_preamble;
	}

	inline std::size_t RawDataPackageDescriptor::encoded_size_length() const {
		return m_encoded_size_length;
	}
}

#endif // RAW_DATA_PACKAGE_DESCRIPTOR_HPP