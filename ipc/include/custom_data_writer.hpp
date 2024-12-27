#ifndef	CUSTOM_DATA_WRITER_HPP
#define	CUSTOM_DATA_WRITER_HPP

#include <functional>
#include <stdexcept>

#include "data_writer.hpp"

namespace ipc {
	template <typename Tdata>
	class CustomDataWriter: public DataWriter<Tdata> {
	public:
		using WriteCustomAction = std::function<void(const Tdata&)>;
		CustomDataWriter(const WriteCustomAction& custom_action);
		CustomDataWriter(const CustomDataWriter&) = default;
		CustomDataWriter& operator=(const CustomDataWriter&) = default;
		
		void write(const Tdata& data) const override;
	private:
		WriteCustomAction m_custom_action;
	};
	
	template <typename Tdata>
	inline CustomDataWriter<Tdata>::CustomDataWriter(const WriteCustomAction& custom_action): m_custom_action(custom_action) {
		if (!m_custom_action) {
			throw std::invalid_argument("invalid write action received");
		}
	}

	template <typename Tdata>
	inline void CustomDataWriter<Tdata>::write(const Tdata& data) const {
		m_custom_action(data);
	}
}

#endif // CUSTOM_DATA_WRITER_HPP