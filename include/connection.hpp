#ifndef	CONNECTION_HPP
#define	CONNECTION_HPP

namespace mcu_control {
	template <typename Tdata, typename Ttime>
	class Connection {
	public:
		virtual ~Connection() noexcept = default;
		virtual void send_data(const Tdata& data) const = 0;
		virtual bool poll_data(const Ttime& delay) const = 0;
		virtual Tdata read_data() const = 0;
	};
}
#endif // CONNECTION_HPP