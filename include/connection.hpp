#ifndef	CONNECTION_HPP
#define	CONNECTION_HPP

namespace mcu_control {
	template <typename Tdata>
	class Connection {
	public:
		virtual ~Connection() noexcept = default;
		virtual void send_data(const Tdata& data) const = 0;
		virtual bool is_readable() const = 0;
		virtual Tdata read_data() const = 0;
	};
}
#endif // CONNECTION_HPP