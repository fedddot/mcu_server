#ifndef	GPO_HPP
#define	GPO_HPP

#include "gpio.hpp"

namespace mcu_platform {
	class Gpo: public Gpio {
	public:
		virtual State state() const = 0;
		virtual void set_state(const State& state) = 0;
		Direction direction() const override;
	};

	inline Gpo::Direction Gpo::direction() const {
		return Gpo::Direction::OUT;
	}
}
#endif // GPO_HPP