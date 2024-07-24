#ifndef	DATA_ENCODER_DECODER_HPP
#define	DATA_ENCODER_DECODER_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "array.hpp"
#include "data.hpp"
#include "object.hpp"

namespace mcu_server_utl {
	class DataEncoderDecoder {
	public:
		DataEncoderDecoder(const std::map<std::string, std::string>& encoding_map);
		DataEncoderDecoder(const DataEncoderDecoder& other) = default;
		DataEncoderDecoder& operator=(const DataEncoderDecoder& other) = default;
		virtual ~DataEncoderDecoder() noexcept = default;

		engine::Data *encode(const engine::Data& data) const;
		engine::Data *decode(const engine::Data& data) const;
	private:
		std::map<std::string, std::string> m_encoding_map;
		engine::Data *encode_array(const engine::Data& data) const;
		engine::Data *encode_object(const engine::Data& data) const;

		engine::Data *decode_array(const engine::Data& data) const;
		engine::Data *decode_object(const engine::Data& data) const;

		std::string encode_field_name(const std::string& field_name) const;
		std::string decode_field_name(const std::string& field_name) const;
	};

	inline DataEncoderDecoder::DataEncoderDecoder(const std::map<std::string, std::string>& encoding_map): m_encoding_map(encoding_map) {

	}

	inline engine::Data *DataEncoderDecoder::encode(const engine::Data& data) const {
		switch (data.type()) {
		case engine::Data::Type::ARRAY:
			return encode_array(data);
		case engine::Data::Type::OBJECT:
			return encode_object(data);
		default:
			return data.clone();
		}
	}

	inline engine::Data *DataEncoderDecoder::encode_array(const engine::Data& data) const {
		engine::Array result;
		engine::Data::cast<engine::Array>(data).for_each(
			[&result, this](int index, const engine::Data& member) {
				std::unique_ptr<engine::Data> encoded_data(encode(member));
				result.push_back(*encoded_data);
			}
		);
		return result.clone();
	}

	inline engine::Data *DataEncoderDecoder::encode_object(const engine::Data& data) const {
		engine::Object result;
		engine::Data::cast<engine::Object>(data).for_each(
			[&result, this](const std::string& field_name, const engine::Data& member) {
				std::unique_ptr<engine::Data> encoded_data(encode(member));
				result.add(encode_field_name(field_name), *encoded_data);
			}
		);
		return result.clone();
	}

	inline engine::Data *DataEncoderDecoder::decode(const engine::Data& data) const {
		switch (data.type()) {
		case engine::Data::Type::ARRAY:
			return decode_array(data);
		case engine::Data::Type::OBJECT:
			return decode_object(data);
		default:
			return data.clone();
		}
	}

	inline engine::Data *DataEncoderDecoder::decode_array(const engine::Data& data) const {
		engine::Array result;
		engine::Data::cast<engine::Array>(data).for_each(
			[&result, this](int index, const engine::Data& member) {
				std::unique_ptr<engine::Data> decoded_data(decode(member));
				result.push_back(*decoded_data);
			}
		);
		return result.clone();
	}

	inline engine::Data *DataEncoderDecoder::decode_object(const engine::Data& data) const {
		engine::Object result;
		engine::Data::cast<engine::Object>(data).for_each(
			[&result, this](const std::string& field_name, const engine::Data& member) {
				std::unique_ptr<engine::Data> decoded_data(decode(member));
				result.add(decode_field_name(field_name), *decoded_data);
			}
		);
		return result.clone();
	}

	inline std::string DataEncoderDecoder::encode_field_name(const std::string& field_name) const {
		auto encoded_field_iter = m_encoding_map.find(field_name);
		if (m_encoding_map.end() == encoded_field_iter) {
			throw std::invalid_argument("received field name " + field_name + " is not present in the coding table");
		}
		return encoded_field_iter->second;
	}

	inline std::string DataEncoderDecoder::decode_field_name(const std::string& field_name) const {
		for (auto iter: m_encoding_map) {
			if (field_name == iter.second) {
				return iter.first;
			}
		}
		throw std::invalid_argument("received field name " + field_name + " is not present in the decoding table");
	}
}

#endif // DATA_ENCODER_DECODER_HPP