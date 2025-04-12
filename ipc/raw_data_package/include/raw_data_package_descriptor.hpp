#ifndef	RAW_DATA_PACKAGE_DESCRIPTOR_HPP
#define	RAW_DATA_PACKAGE_DESCRIPTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace ipc {
	template <typename Input, typename Output>
	class DataTransformer {
	public:
		virtual ~DataTransformer() noexcept = default;
		virtual Output transform(const Input& input) const = 0;
	};

	class DefaultPackageSizeParser: public DataTransformer<std::vector<char>, std::size_t> {
	public:
		DefaultPackageSizeParser(const std::size_t& raw_data_size);
		DefaultPackageSizeParser(const DefaultPackageSizeParser&) = default;
		DefaultPackageSizeParser& operator=(const DefaultPackageSizeParser&) = default;
		std::size_t transform(const std::vector<char>& input) const override;
		std::size_t raw_data_size() const;
	private:
		enum: std::size_t {
			BITS_IN_BYTE = 8UL,
		};
		std::size_t m_raw_data_size;
	};

	inline DefaultPackageSizeParser::DefaultPackageSizeParser(const std::size_t& raw_data_size): m_raw_data_size(raw_data_size) {
		if (1UL > m_raw_data_size) {
			throw std::invalid_argument("received raw data size is too small");
		}
	}

	inline std::size_t DefaultPackageSizeParser::transform(const std::vector<char>& input) const {
		if (m_raw_data_size != input.size()) {
			throw std::invalid_argument("invalid raw data size");
		}
		auto decoded_size = std::size_t(0UL);
		for (auto i = 0; i < m_raw_data_size; ++i) {
			decoded_size <<= BITS_IN_BYTE;
			decoded_size |= static_cast<std::size_t>(input[m_raw_data_size - 1 - i]);
		}
		return decoded_size;
	}

	inline std::size_t DefaultPackageSizeParser::raw_data_size() const {
		return m_raw_data_size;
	}

	class DefaultPackageSizeSerializer: public DataTransformer<std::size_t, std::vector<char>> {
	public:
		DefaultPackageSizeSerializer(const std::size_t& raw_data_size);
		DefaultPackageSizeSerializer(const DefaultPackageSizeSerializer&) = default;
		DefaultPackageSizeSerializer& operator=(const DefaultPackageSizeSerializer&) = default;
		std::vector<char> transform(const std::size_t& input) const override;
		std::size_t raw_data_size() const;
	private:
		enum: std::size_t {
			BITS_IN_BYTE = 8UL,
		};
		std::size_t m_raw_data_size;
	};

	inline DefaultPackageSizeSerializer::DefaultPackageSizeSerializer(const std::size_t& raw_data_size): m_raw_data_size(raw_data_size) {
		if (1UL > m_raw_data_size) {
			throw std::invalid_argument("received raw data size is too small");
		}
	}

	inline std::vector<char> DefaultPackageSizeSerializer::transform(const std::size_t& input) const {
		auto encoded_size = std::vector<char>(m_raw_data_size, 0UL);
		for (auto i = 0; i < m_raw_data_size; ++i) {
			const auto less_sign_byte = static_cast<char>((input >> BITS_IN_BYTE * i) & 0xFF);
			encoded_size[i] = less_sign_byte;
		}
		return encoded_size;
	}

	inline std::size_t DefaultPackageSizeSerializer::raw_data_size() const {
		return m_raw_data_size;
	}
}

#endif // RAW_DATA_PACKAGE_DESCRIPTOR_HPP