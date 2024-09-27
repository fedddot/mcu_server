#include "gtest/gtest.h"
#include <memory>

#include "vendor.hpp"

using namespace server;
// using namespace server_uts;

TEST(ut_vendor, ctor_cctor_clone_dtor_sanity) {
	// GIVEN
	const std::string test_vendor_id("test_vendor_id");
	
	// WHEN
	using VendorUnqPtr = std::unique_ptr<Vendor>;
	using ResourceUnqPtr = std::unique_ptr<Resource>;
	VendorUnqPtr instance_ptr(nullptr);
	VendorUnqPtr instance_ptr_copy(nullptr);
	ResourceUnqPtr instance_ptr_clone(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = VendorUnqPtr(new Vendor(test_vendor_id)));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_EQ(test_vendor_id, instance_ptr->id());

	// cctor
	ASSERT_NO_THROW(instance_ptr_copy = VendorUnqPtr(new Vendor(*instance_ptr)));
	ASSERT_NE(nullptr, instance_ptr_copy);
	ASSERT_EQ(instance_ptr->id(), instance_ptr_copy->id());

	// clone
	ASSERT_NO_THROW(instance_ptr_clone = ResourceUnqPtr(instance_ptr->clone()));
	ASSERT_NE(nullptr, instance_ptr_clone);
	ASSERT_EQ(instance_ptr->id(), instance_ptr_clone->id());

	// dtors
	ASSERT_NO_THROW(instance_ptr = nullptr);
	ASSERT_NO_THROW(instance_ptr_copy = nullptr);
	ASSERT_NO_THROW(instance_ptr_clone = nullptr);
}