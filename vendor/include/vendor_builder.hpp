#ifndef	VENDOR_BUILDER_HPP
#define	VENDOR_BUILDER_HPP

#include "vendor.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	class VendorBuilder {
	public:
		virtual ~VendorBuilder() noexcept = default;
		virtual Instance<Vendor> build() const = 0;
	};
}

#endif // VENDOR_BUILDER_HPP