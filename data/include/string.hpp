#ifndef	STRING_HPP
#define	STRING_HPP

#include <string>
#include "data.hpp"

namespace server {
	class String: public Data {
	public:
		String(const std::string& str);
		String(const String& other) = default;
		String& operator=(const String& other) = default;
		Type type() const override;
		virtual Data *clone() const override;
		std::string get() const;
	private:
		std::string m_val;
	}; // String

	inline String::String(const std::string& str): m_val(str) {

	}

	inline String::Type String::type() const {
		return Type::STR;
	}

	inline Data *String::clone() const {
		return new String(*this);
	}

	inline std::string String::get() const {
		return m_val;
	}
}

#endif // STRING_HPP