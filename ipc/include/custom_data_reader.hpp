#ifndef	CUSTOM_DATA_READER_HPP
#define	CUSTOM_DATA_READER_HPP

#include <functional>
#include <stdexcept>

#include "data_reader.hpp"

namespace ipc {
	template <typename Tdata>
	class CustomDataReader: public DataReader<Tdata> {
	public:
		using ReadCustomAction = std::function<Tdata()>;
		using ReadableCustomAction = std::function<bool()>;

		CustomDataReader(
			const ReadCustomAction& read_custom_action,
			const ReadCustomAction& readable_custom_action
		);
		CustomDataReader(const CustomDataReader&) = default;
		CustomDataReader& operator=(const CustomDataReader&) = default;
		
		bool readable() const override;
		Tdata read() const override;
	private:
		ReadCustomAction m_read_custom_action;
		ReadCustomAction m_readable_custom_action;
	};
	
	template <typename Tdata>
	inline CustomDataReader<Tdata>::CustomDataReader(
			const ReadCustomAction& read_custom_action,
			const ReadCustomAction& readable_custom_action
		): m_read_custom_action(read_custom_action), m_readable_custom_action(readable_custom_action) {
		if (!m_read_custom_action || !m_readable_custom_action) {
			throw std::invalid_argument("invalid actions received");
		}
	}

	template <typename Tdata>
	inline bool CustomDataReader<Tdata>::readable() const {
		return m_readable_custom_action();
	}

	template <typename Tdata>
	inline Tdata CustomDataReader<Tdata>::read() const {
		return m_read_custom_action();
	}
}

#endif // CUSTOM_DATA_READER_HPP