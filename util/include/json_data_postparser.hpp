#ifndef	JSON_DATA_POSTPARSER_HPP
#define	JSON_DATA_POSTPARSER_HPP

#include <map>
#include <memory>
#include <string>

#include "array.hpp"
#include "data.hpp"
#include "object.hpp"
#include "parser.hpp"

namespace Json {
	class Value;
}

namespace mcu_server_utl {
	class JsonDataPostParser: public mcu_server::Parser<engine::Data *(const engine::Data&)> {
	public:
		JsonDataPostParser() = default;
		JsonDataPostParser(const JsonDataPostParser& other) = default;
		JsonDataPostParser& operator=(const JsonDataPostParser& other) = default;
		
		engine::Data *parse(const engine::Data& data) const override;
		mcu_server::Parser<engine::Data *(const engine::Data&)> *clone() const override;
	private:
		static const std::map<std::string, std::string> s_replacements;
		engine::Data *preparse_object(const engine::Data& data) const;
		engine::Data *preparse_array(const engine::Data& data) const;
	};

	inline engine::Data *JsonDataPostParser::parse(const engine::Data& data) const {
		switch (data.type()) {
		case engine::Data::Type::OBJECT:
			return preparse_object(data);
		case engine::Data::Type::ARRAY:
			return preparse_array(data);
		default:
			return data.clone();
		}
	}

	inline mcu_server::Parser<engine::Data *(const engine::Data&)> *JsonDataPostParser::clone() const {
		return new JsonDataPostParser(*this);
	}

	inline engine::Data *JsonDataPostParser::preparse_object(const engine::Data& data) const {
		using namespace engine;
		Object preparsed;
		Data::cast<Object>(data).for_each(
			[&preparsed, this](const std::string& field, const Data& member) {
				auto preparsed_field = field;
				auto field_iter = s_replacements.find(field);
				if (s_replacements.end() != field_iter) {
					preparsed_field = field_iter->second;
				}
				std::unique_ptr<Data> preparsed_member(parse(member));
				preparsed.add(preparsed_field, *preparsed_member);
			}
		);
		return preparsed.clone();
	}
	
	inline engine::Data *JsonDataPostParser::preparse_array(const engine::Data& data) const {
		using namespace engine;
		Array preparsed;
		Data::cast<Array>(data).for_each(
			[&preparsed, this](int index, const Data& member) {
				std::unique_ptr<Data> preparsed_member(parse(member));
				preparsed.push_back(*preparsed_member);
			}
		);
		return preparsed.clone();
	}
}

#endif // JSON_DATA_POSTPARSER_HPP