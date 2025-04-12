#ifndef	TEST_RESPONSE_WRITER_HPP
#define	TEST_RESPONSE_WRITER_HPP

#include <functional>

#include "clonable_ipc_data_writer.hpp"
#include "ipc_data_writer.hpp"

namespace ipc {
	template <typename IpcData>
	class TestIpcDataWriter: public ClonableIpcDataWriter<IpcData> {
	public:
		using Action = std::function<void(const IpcData&)>;
		TestIpcDataWriter(const Action& action): m_action(action) {
			
		}
		TestIpcDataWriter(const TestIpcDataWriter&) = default;
		TestIpcDataWriter& operator=(const TestIpcDataWriter&) = default;

		void write(const IpcData& response) const override {
			m_action(response);
		}

		IpcDataWriter<IpcData> *clone() const {
			return new TestIpcDataWriter(*this);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_RESPONSE_WRITER_HPP