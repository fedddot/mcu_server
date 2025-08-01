#ifndef	TEST_DATA_READER_HPP
#define	TEST_DATA_READER_HPP

#include <functional>

#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename Signature>
	class TestDataReader;

	template <typename Result, typename... Args>
	class TestDataReader<Result(Args...)>: public DataReader<Result(Args...)> {
	public:
		using Action = std::function<Result(Args...)>;
		TestDataReader(const Action& action): m_action(action) {
			
		}
		TestDataReader(const TestDataReader&) = default;
		TestDataReader& operator=(const TestDataReader&) = default;
		
		Result read(Args... args) const override {
			return m_action(std::forward<Args>(args)...);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_DATA_READER_HPP