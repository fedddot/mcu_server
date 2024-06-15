#ifndef	TEST_GPI_HPP
#define	TEST_GPI_HPP

#include "gpi.hpp"

namespace server_uts {
	class TestGpi: public server::Gpi {
	public:
		TestGpi();
		TestGpi(const TestGpi&) = default;
		TestGpi& operator=(const TestGpi&) = default;

		State state() const override;
		void update_test_state(const State& state);
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
}
#endif // TEST_GPI_HPP