#ifndef	TEST_REQUEST_READER_HPP
#define	TEST_REQUEST_READER_HPP

#include <functional>

#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename Result, typename... Args>
	class TestIpcDataReader: public DataReader<Result(Args...)> {
	public:
		using Action = std::function<Result(Args...)>;
		TestIpcDataReader(const Action& action): m_action(action) {
			
		}
		TestIpcDataReader(const TestIpcDataReader&) = default;
		TestIpcDataReader& operator=(const TestIpcDataReader&) = default;
		
		Result read(Args... args) const override {
			return m_action(std::forward<Args>(args)...);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_REQUEST_READER_HPP