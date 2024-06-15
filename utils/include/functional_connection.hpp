#ifndef	FUNCTIONAL_CONNECTION_HPP
#define	FUNCTIONAL_CONNECTION_HPP

#include <functional>

#include "connection.hpp"

namespace mcu_control_utl {
	template <typename Tdata>
	class FunctionalConnection: public mcu_control::Connection<Tdata> {
	public:
		using SendAction = std::function<void(const Tdata&)>;
		using IsReadableAction = std::function<bool()>;
		using ReadAction = std::function<Tdata()>;

		FunctionalConnection(const SendAction& send_action, const IsReadableAction& is_readable_action, const ReadAction& read_action);
		FunctionalConnection(const FunctionalConnection& other) = delete;
		FunctionalConnection& operator=(const FunctionalConnection& other) = delete;

		void send_data(const Tdata& data) const override;
		bool is_readable() const override;
		Tdata read_data() const override;

	private:
		SendAction m_send_action;
		IsReadableAction m_is_readable_action;
		ReadAction m_read_action;
	};

	template <typename Tdata>
	inline FunctionalConnection<Tdata>::FunctionalConnection(const SendAction& send_action, const IsReadableAction& is_readable_action, const ReadAction& read_action): m_send_action(send_action), m_is_readable_action(is_readable_action), m_read_action(read_action) {

	}

	template <typename Tdata>
	inline void FunctionalConnection<Tdata>::send_data(const Tdata& data) const {
		m_send_action(data);
	}

	template <typename Tdata>
	inline bool FunctionalConnection<Tdata>::is_readable() const {
		return m_is_readable_action();
	}

	template <typename Tdata>
	inline Tdata FunctionalConnection<Tdata>::read_data() const {
		return m_read_action();
	}
}
#endif // FUNCTIONAL_CONNECTION_HPP