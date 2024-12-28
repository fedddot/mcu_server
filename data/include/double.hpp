#ifndef	DOUBLE_HPP
#define	DOUBLE_HPP

#include "data.hpp"

namespace data {
	class Double: public Data {
	public:
		Double(double val);
		Double(const Double& other) = default;
		Double& operator=(const Double& other) = default;
		Type type() const override;
		Data *clone() const override;
		double get() const;
	private:
		double m_val;
	};

	inline Double::Double(double val): m_val(val) {

	}

	inline Double::Type Double::type() const {
		return Type::DOUBLE;
	}

	inline Data *Double::clone() const {
		return new Double(*this);
	}

	inline double Double::get() const {
		return m_val;
	}
}

#endif // DOUBLE_HPP