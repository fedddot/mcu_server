#ifndef	PWM_CONTROLLER_HPP
#define	PWM_CONTROLLER_HPP

/// @file pwm_controller.hpp
/// @brief Interface for controlling PWM (Pulse Width Modulation) outputs
/// @details Provides a hardware-independent interface for managing PWM signals.
/// The interface allows control of duty cycle and output state, abstracting
/// the underlying hardware-specific PWM implementation details.

#include "provider.hpp"

namespace provider {

	/// @brief Abstract interface for controlling PWM outputs
	class PwmController: public Provider {
	public:
		virtual ~PwmController() noexcept = default;

		/// @brief Starts PWM output with the specified duty cycle
		/// 
		/// @param duty_cycle The desired duty cycle in percent (0.0 to 100.0)
		/// @throw std::invalid_argument if duty_cycle is out of valid range
		virtual void start(const double duty_cycle) = 0;

		/// @brief Stops PWM output
		virtual void stop() = 0;
	};
}

#endif // PWM_CONTROLLER_HPP