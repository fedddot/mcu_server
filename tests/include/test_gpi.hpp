#ifndef	TEST_GPI_HPP
#define	TEST_GPI_HPP

#include "gpi.hpp"
#include "gpio.hpp"

namespace server_ut {
	class TestGpi: public server::Gpi {
	public:
		TestGpi();
		TestGpi(const TestGpi& other) = default;
		TestGpi& operator=(const TestGpi& other) = default;
		
		virtual State state() const override;
		virtual server::Gpio *clone() const override;
	private:
		State m_state;
	};

	inline TestGpi::TestGpi(): m_state(State::LOW) {

	}

	inline TestGpi::State TestGpi::state() const {
		return m_state;
	}

	inline server::Gpio *TestGpi::clone() const {
		return new TestGpi(*this);
	}
}
#endif // TEST_GPI_HPP