#ifndef	INTEGER_HPP
#define	INTEGER_HPP

#include "data.hpp"
#include "value.hpp"

namespace engine {
	class Integer: public Value<int> {
	public:
		Integer(int val);
		Integer(const Integer& other) = default;
		Integer& operator=(const Integer& other) = default;
		Type type() const override;
		virtual Data *clone() const override;
	};

	inline Integer::Integer(int val): Value<int>(val) {

	}

	inline Integer::Type Integer::type() const {
		return Type::INT;
	}

	inline Data *Integer::clone() const {
		return new Integer(*this);
	}
}

#endif // INTEGER_HPP