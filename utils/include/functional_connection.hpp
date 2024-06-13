#ifndef	FUNCTIONAL_CONNECTION_HPP
#define	FUNCTIONAL_CONNECTION_HPP

#include <functional>

#include "connection.hpp"

namespace mcu_control_utl {
	template <typename Tdata, typename Ttime>
	class FunctionalConnection: public mcu_control::Connection<Tdata, Ttime> {
	public:
		using SendAction = std::function<void(const Tdata&)>;
		using PollAction = std::function<bool(const Ttime&)>;
		using ReadAction = std::function<Tdata()>;

		FunctionalConnection(const SendAction& send_action, const PollAction& poll_action, const ReadAction& read_action);
		FunctionalConnection(const FunctionalConnection& other) = delete;
		FunctionalConnection& operator=(const FunctionalConnection& other) = delete;

		void send_data(const Tdata& data) const override;
		bool poll_data(const Ttime& delay) const override;
		Tdata read_data() const override;

	private:
		SendAction m_send_action;
		PollAction m_poll_action;
		ReadAction m_read_action;
	};

	template <typename Tdata, typename Ttime>
	inline FunctionalConnection<Tdata, Ttime >::FunctionalConnection(const SendAction& send_action, const PollAction& poll_action, const ReadAction& read_action): m_send_action(send_action), m_poll_action(poll_action), m_read_action(read_action) {

	}

	template <typename Tdata, typename Ttime>
	inline void FunctionalConnection<Tdata, Ttime >::send_data(const Tdata& data) const {
		m_send_action(data);
	}

	template <typename Tdata, typename Ttime>
	inline bool FunctionalConnection<Tdata, Ttime >::poll_data(const Ttime& delay) const {
		return m_poll_action(delay);
	}

	template <typename Tdata, typename Ttime>
	inline Tdata FunctionalConnection<Tdata, Ttime >::read_data() const {
		return m_read_action();
	}
}
#endif // FUNCTIONAL_CONNECTION_HPP