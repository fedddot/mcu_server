#ifndef	DELAY_HPP
#define	DELAY_HPP

namespace mcu_platform {
	class Delay {
	public:
		virtual ~Delay() noexcept = default;
		virtual void delay(int delay_ms) const = 0;
	};
}
#endif // DELAY_HPP