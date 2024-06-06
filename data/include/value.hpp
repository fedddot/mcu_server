#ifndef	VALUE_HPP
#define	VALUE_HPP

#include "data.hpp"

namespace server {
	template <class T>
	class Value: public Data {
	public:
		Value(const T& val);
		Value(const Value& other) = default;
		Value& operator=(const Value& other) = default;
		T get() const;
		void set(const T& val);
	private:
		T m_val;
	};

	template <class T>
	Value<T>::Value(const T& val): m_val(val) {

	}

	template <class T>
	T Value<T>::get() const {
		return m_val;
	}

	template <class T>
	void Value<T>::set(const T& val) {
		m_val = val;
	}
} // namespace server

#endif // VALUE_HPP