#ifndef	TEST_GPO_HPP
#define	TEST_GPO_HPP

#include "gpio.hpp"
#include "gpo.hpp"

namespace server_ut {
	class TestGpo: public server::Gpo {
	public:
		TestGpo();
		TestGpo(const TestGpo& other) = default;
		TestGpo& operator=(const TestGpo& other) = default;
		
		virtual State state() const override;
		virtual void set_state(const State& state) override;
		virtual server::Gpio *clone() const override;
	private:
		State m_state;
	};

	inline TestGpo::TestGpo(): m_state(State::LOW) {

	}

	inline TestGpo::State TestGpo::state() const {
		return m_state;
	}

	inline void TestGpo::set_state(const State& state) {
		m_state = state;
	}

	inline server::Gpio *TestGpo::clone() const {
		return new TestGpo(*this);
	}
}
#endif // TEST_GPO_HPP