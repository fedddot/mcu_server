#ifndef	GPIO_REQUEST_HPP
#define	GPIO_REQUEST_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "data.hpp"

namespace manager {
	class GpioRequest {
	public:
		enum class Operation: int {
			CREATE,
			READ,
			WRITE,
			DELETE
		};
		enum class Field: int {
			ID,
			DIRECTION,
			IO_NUMBER
		};
		GpioRequest(const Operation& operation);
		GpioRequest(const GpioRequest&) = default;
		GpioRequest& operator=(const GpioRequest&) = default;
		virtual ~GpioRequest() noexcept = default;

		Operation operation() const;
		bool has_data(const Field& field) const;
		void set_data(const Field& field, const server::Data& data);
		const server::Data& get_data(const Field& field) const;
	private:
		Operation m_operation;
		std::map<Field, std::unique_ptr<server::Data>> m_data;
	};

	inline GpioRequest::GpioRequest(const Operation& operation): m_operation(operation) {

	}

	inline typename GpioRequest::Operation GpioRequest::operation() const {
		return m_operation;
	}

	inline bool GpioRequest::has_data(const Field& field) const {
		return m_data.find(field) != m_data.end();
	}

	inline void GpioRequest::set_data(const Field& field, const server::Data& data) {
		m_data[field] = std::unique_ptr<server::Data>(data.clone());
	}

	inline const server::Data& GpioRequest::get_data(const Field& field) const {
		const auto iter = m_data.find(field);
		if (m_data.end() == iter) {
			throw std::invalid_argument("mising request data");
		}
		return *(m_data.at(field));
	}
}

#endif // GPIO_REQUEST_HPP