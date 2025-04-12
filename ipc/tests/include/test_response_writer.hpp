#ifndef	TEST_RESPONSE_WRITER_HPP
#define	TEST_RESPONSE_WRITER_HPP

#include "response_writer.hpp"
#include <functional>

namespace ipc {
	template <typename Response>
	class TestResponseWriter: public IpcDataWriter<Response> {
	public:
		using Action = std::function<void(const Response&)>;
		TestResponseWriter(const Action& action): m_action(action) {
			
		}
		TestResponseWriter(const TestResponseWriter&) = default;
		TestResponseWriter& operator=(const TestResponseWriter&) = default;

		void write(const Response& response) const override {
			m_action(response);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_RESPONSE_WRITER_HPP