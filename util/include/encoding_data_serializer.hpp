#ifndef	ENCODING_DATA_SERIALIZER_HPP
#define	ENCODING_DATA_SERIALIZER_HPP

#include <memory>
#include <string>

#include "data.hpp"
#include "data_encoder_decoder.hpp"
#include "json_data_serializer.hpp"
#include "serializer.hpp"

namespace mcu_server_utl {
	class EncodingDataSerializer: public mcu_server::Serializer<std::string(const engine::Data&)> {
	public:
		EncodingDataSerializer(const DataEncoderDecoder& data_encoder);
		EncodingDataSerializer(const EncodingDataSerializer& other) = default;
		EncodingDataSerializer& operator=(const EncodingDataSerializer& other) = default;
		
		std::string serialize(const engine::Data& data) const override;
		mcu_server::Serializer<std::string(const engine::Data&)> *clone() const override;
	private:
		DataEncoderDecoder m_data_encoder;
		JsonDataSerializer m_json_serializer;
	};

	inline EncodingDataSerializer::EncodingDataSerializer(const DataEncoderDecoder& data_encoder): m_data_encoder(data_encoder) {

	}

	inline std::string EncodingDataSerializer::serialize(const engine::Data& data) const {
		std::unique_ptr<engine::Data> encoded_data(m_data_encoder.encode(data));
		return m_json_serializer.serialize(*encoded_data);
	}

	inline mcu_server::Serializer<std::string(const engine::Data&)> *EncodingDataSerializer::clone() const {
		return new EncodingDataSerializer(*this);
	}
}

#endif // ENCODING_DATA_SERIALIZER_HPP