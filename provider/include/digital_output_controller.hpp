#ifndef	DIGITAL_OUTPUT_CONTROLLER_HPP
#define	DIGITAL_OUTPUT_CONTROLLER_HPP

#include "provider.hpp"

namespace provider {
	class DigitalOutputController: public Provider {
	public:
		enum class State: int {
			HIGH,
			LOW
		};
		virtual void set(const State& state) = 0;
		virtual State get() const = 0;
	};
}

#endif // DIGITAL_OUTPUT_CONTROLLER_HPP