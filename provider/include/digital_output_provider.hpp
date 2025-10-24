#ifndef	DIGITAL_OUTPUT_PROVIDER_HPP
#define	DIGITAL_OUTPUT_PROVIDER_HPP

#include "provider.hpp"

namespace provider {
	template <typename Tdigital_state>
	class DigitalOutputProvider: public Provider {
	public:
		virtual void set(const Tdigital_state& state) = 0;
	};
}

#endif // DIGITAL_OUTPUT_PROVIDER_HPP