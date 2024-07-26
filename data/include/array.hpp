#ifndef	ARRAY_HPP
#define	ARRAY_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "data.hpp"

namespace mcu_server {
	class Array: public Data {
	public:
		Array() = default;
		Array(const Array& other);
		Array& operator=(const Array& other);

		Type type() const override;
		Data *clone() const override;

		std::size_t size() const;
		Data& access(int index);
		const Data& access(int index) const;
		void remove(int index);
		void push_front(const Data& data);
		void push_back(const Data& data);
		void for_each(const std::function<void(int, const Data&)>& action) const;
	private:
		using DataContainer = std::vector<std::unique_ptr<Data>>;
		DataContainer m_data;
		void throw_if_out_of_boundaries(int index) const;
		static DataContainer copy_container(const Array& array);
	};

	inline Array::Array(const Array& other): m_data(copy_container(other)) {

	}
	
	inline Array& Array::operator=(const Array& other) {
		m_data = copy_container(other);
		return *this;
	}

	inline Data::Type Array::type() const {
		return Type::ARRAY;
	}

	inline Data *Array::clone() const {
		return new Array(*this);
	}

	inline std::size_t Array::size() const {
		return m_data.size();
	}

	inline Data& Array::access(int index) {
		throw_if_out_of_boundaries(index);
		return std::ref(*m_data[index]);
	}

	inline const Data& Array::access(int index) const {
		throw_if_out_of_boundaries(index);
		return std::ref(*m_data[index]);
	}

	inline void Array::remove(int index) {
		throw_if_out_of_boundaries(index);
		m_data.erase(m_data.begin() + index);
	}

	inline void Array::push_front(const Data& data) {
		m_data.insert(m_data.begin(), std::unique_ptr<Data>(data.clone()));
	}

	inline void Array::push_back(const Data& data) {
		m_data.push_back(std::unique_ptr<Data>(data.clone()));
	}

	inline void Array::for_each(const std::function<void(int, const Data&)>& action) const {
		for (auto i = 0; i < m_data.size(); ++i) {
			action(i, access(i));
		}
	}

	inline void Array::throw_if_out_of_boundaries(int index) const {
		if ((0 > index) || (m_data.size() <= index)) {
			throw std::invalid_argument("received index is out of array boudaries");
		}
	}

	inline Array::DataContainer Array::copy_container(const Array& array) {
		DataContainer container;
		array.for_each(
			[&container](int index, const Data& data) {
				container.push_back(std::unique_ptr<Data>(data.clone()));
			}
		);
		return container;
	}
} // namespace mcu_server

#endif // ARRAY_HPP