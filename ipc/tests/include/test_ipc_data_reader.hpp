#ifndef	TEST_REQUEST_READER_HPP
#define	TEST_REQUEST_READER_HPP

#include <functional>
#include <optional>

#include "clonable_ipc_data_reader.hpp"
#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename IpcData>
	class TestIpcDataReader: public ClonableIpcDataReader<IpcData> {
	public:
		using Action = std::function<std::optional<IpcData>(void)>;
		TestIpcDataReader(const Action& action): m_action(action) {
			
		}
		TestIpcDataReader(const TestIpcDataReader&) = default;
		TestIpcDataReader& operator=(const TestIpcDataReader&) = default;
		
		std::optional<IpcData> read() override {
			return m_action();
		}

		IpcDataReader<IpcData> *clone() const {
			return new TestIpcDataReader(*this);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_REQUEST_READER_HPP