#ifndef	PWM_CONTROLLER_HPP
#define	PWM_CONTROLLER_HPP

#include "provider.hpp"

namespace provider {
	class PwmController: public Provider {
	public:
		virtual ~PwmController() noexcept = default;
		virtual void start(const double duty_cycle) = 0;
		virtual void stop() = 0;
	};
}

#endif // PWM_CONTROLLER_HPP