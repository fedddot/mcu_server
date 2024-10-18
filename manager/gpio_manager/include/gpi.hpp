#ifndef	GPI_HPP
#define	GPI_HPP

#include "gpio.hpp"

namespace manager {
	class Gpi: public Gpio {
	public:
		virtual State state() const = 0;
		Direction direction() const override;
	};

	inline Gpi::Direction Gpi::direction() const {
		return Gpi::Direction::IN;
	}
}
#endif // GPI_HPP