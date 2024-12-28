#ifndef	INTEGER_HPP
#define	INTEGER_HPP

#include "data.hpp"

namespace data {
	class Integer: public Data {
	public:
		Integer(int val);
		Integer(const Integer& other) = default;
		Integer& operator=(const Integer& other) = default;
		Type type() const override;
		Data *clone() const override;
		int get() const;
	private:
		int m_val;
	};

	inline Integer::Integer(int val): m_val(val) {

	}

	inline Integer::Type Integer::type() const {
		return Type::INT;
	}

	inline Data *Integer::clone() const {
		return new Integer(*this);
	}

	inline int Integer::get() const {
		return m_val;
	}
}

#endif // INTEGER_HPP