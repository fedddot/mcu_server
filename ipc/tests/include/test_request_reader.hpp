#ifndef	TEST_REQUEST_READER_HPP
#define	TEST_REQUEST_READER_HPP

#include <functional>
#include <optional>

#include "request_reader.hpp"

namespace ipc {
	template <typename Request>
	class TestRequestReader: public IpcDataReader<Request> {
	public:
		using Action = std::function<std::optional<Request>(void)>;
		TestRequestReader(const Action& action): m_action(action) {
			
		}
		TestRequestReader(const TestRequestReader&) = default;
		TestRequestReader& operator=(const TestRequestReader&) = default;
		
		std::optional<Request> read() override {
			return m_action();
		}
	private:
		Action m_action;
	};
}

#endif // TEST_REQUEST_READER_HPP