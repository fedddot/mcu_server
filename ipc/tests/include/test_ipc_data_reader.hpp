#ifndef	TEST_REQUEST_READER_HPP
#define	TEST_REQUEST_READER_HPP

#include <functional>
#include <optional>

#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"

namespace ipc {
	template <typename IpcData>
	class TestIpcDataReader: public IpcDataReader<IpcData>, public Clonable<IpcDataReader<IpcData>> {
	public:
		using Action = std::function<std::optional<Instance<IpcData>>(void)>;
		TestIpcDataReader(const Action& action): m_action(action) {
			
		}
		TestIpcDataReader(const TestIpcDataReader&) = default;
		TestIpcDataReader& operator=(const TestIpcDataReader&) = default;
		
		std::optional<Instance<IpcData>> read() override {
			return m_action();
		}
		IpcDataReader<IpcData> *clone() const override {
			return new TestIpcDataReader(*this);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_REQUEST_READER_HPP