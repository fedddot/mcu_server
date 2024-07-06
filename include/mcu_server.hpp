#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include "data.hpp"
#include "data_extractor.hpp"
#include "data_sender.hpp"
#include "engine.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace mcu_server {
	template <typename Tgpio_id>
	class McuServer {
	public:
		using RawData = std::string;
		using McuEngine = engine::Engine<engine::Data(const engine::Data&)>;
		McuServer(McuEngine *engine, const DataSender<RawData>& sender, const DataExtractor<RawData>& extractor);
		McuServer(const McuServer& other) = delete;
		McuServer& operator=(const McuServer& other) = delete;
		virtual ~McuServer() noexcept = default;

		void feed(const RawData& data);
	private:
		McuEngine *m_engine;
		std::unique_ptr<DataSender<RawData>> m_sender;
		std::unique_ptr<DataSender<RawData>> m_extractor;
		RawData m_data;
	};

	template <typename Tgpio_id>
	McuServer<Tgpio_id>::McuServer(McuEngine *engine, const DataSender<RawData>& sender, const DataExtractor<RawData>& extractor): m_engine(engine), m_sender(sender.clone()), m_extractor(extractor.clone()) {
		if (!engine) {
			throw std::invalid_argument("invalid engine ptr received");
		}
	}

	template <typename Tgpio_id>
	void McuServer<Tgpio_id>::feed(const RawData& data) {
		m_data.insert(m_data.end(), data.begin(), data.end());
		if (m_extractor->is_extractable(m_data)) {
			
		}
	}
}

#endif // MCU_SERVER_HPP