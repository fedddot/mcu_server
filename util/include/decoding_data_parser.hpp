#ifndef	DECODING_DATA_PARSER_HPP
#define	DECODING_DATA_PARSER_HPP

#include <memory>
#include <string>

#include "data.hpp"
#include "data_encoder_decoder.hpp"
#include "json_data_parser.hpp"
#include "parser.hpp"

namespace mcu_server_utl {
	class DecodingDataParser: public mcu_server::Parser<engine::Data *(const std::string&)> {
	public:
		DecodingDataParser(const DataEncoderDecoder& data_decoder);
		DecodingDataParser(const DecodingDataParser& other) = default;
		DecodingDataParser& operator=(const DecodingDataParser& other) = default;
		
		engine::Data *parse(const std::string& data) const override;
		mcu_server::Parser<engine::Data *(const std::string&)> *clone() const override;
	private:
		DataEncoderDecoder m_data_decoder;
		JsonDataParser m_json_parser;
	};

	inline DecodingDataParser::DecodingDataParser(const DataEncoderDecoder& data_decoder): m_data_decoder(data_decoder) {

	}

	inline engine::Data *DecodingDataParser::parse(const std::string& data) const {
		std::unique_ptr<engine::Data> parsed_data(m_json_parser.parse(data));
		return m_data_decoder.decode(*parsed_data);
	}

	inline mcu_server::Parser<engine::Data *(const std::string&)> *DecodingDataParser::clone() const {
		return new DecodingDataParser(*this);
	}
}

#endif // DECODING_DATA_PARSER_HPP