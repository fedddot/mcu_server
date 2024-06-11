#ifndef	STRING_HPP
#define	STRING_HPP

#include <string>
#include "data.hpp"
#include "value.hpp"

namespace engine {
	class String: public Value<std::string> {
	public:
		String(const std::string& str);
		String(const String& other) = default;
		String& operator=(const String& other) = default;
		Type type() const override;
		virtual Data *clone() const override;
	}; // String

	inline String::String(const std::string& str): Value<std::string>(str) {

	}

	inline String::Type String::type() const {
		return Type::STR;
	}

	inline Data *String::clone() const {
		return new String(*this);
	}
}

#endif // STRING_HPP