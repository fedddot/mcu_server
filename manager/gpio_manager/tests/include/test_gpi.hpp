#ifndef	TEST_GPI_HPP
#define	TEST_GPI_HPP

#include "gpi.hpp"
#include "gpio.hpp"

namespace manager_uts {
	class TestGpi: public manager::Gpi {
	public:
		TestGpi();
		TestGpi(const TestGpi&) = default;
		TestGpi& operator=(const TestGpi&) = default;

		State state() const override;
		void update_test_state(const State& state);
		manager::Gpio *clone() const override;
	private:
		State m_state;
	};

	inline TestGpi::TestGpi(): m_state(State::LOW) {
		
	}

	inline TestGpi::State TestGpi::state() const {
		return m_state;
	}

	inline void TestGpi::update_test_state(const State& state) {
		m_state = state;
	}

	inline manager::Gpio *TestGpi::clone() const {
		return new TestGpi(*this);
	}
}
#endif // TEST_GPI_HPP