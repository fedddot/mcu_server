#ifndef	DIGITAL_INPUT_PROVIDER_HPP
#define	DIGITAL_INPUT_PROVIDER_HPP

#include "provider.hpp"

namespace provider {
	template <typename Tdigital_state>
	class DigitalInputProvider: public Provider {
	public:
		virtual Tdigital_state get() const = 0;
	};
}

#endif // DIGITAL_INPUT_PROVIDER_HPP