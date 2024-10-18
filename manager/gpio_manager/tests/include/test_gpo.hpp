#ifndef	TEST_GPO_HPP
#define	TEST_GPO_HPP

#include "gpo.hpp"

namespace manager_uts {
	class TestGpo: public manager::Gpo {
	public:
		TestGpo();
		TestGpo(const TestGpo&) = default;
		TestGpo& operator=(const TestGpo&) = default;

		State state() const override;
		void set_state(const State& state) override;

		manager::Gpio *clone() const override;
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

	inline manager::Gpio *TestGpo::clone() const {
		return new TestGpo(*this);
	}
}
#endif // TEST_GPO_HPP