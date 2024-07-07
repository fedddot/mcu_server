#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "data_extractor.hpp"
#include "data_sender.hpp"
#include "engine.hpp"
#include "parser.hpp"
#include "serializer.hpp"

namespace mcu_server {
	template <typename Tgpio_id>
	class McuServer {
	public:
		using RawData = std::string;
		using McuEngine = engine::Engine<engine::Data *(const engine::Data&)>;
		using RawDataSender = DataSender<RawData>;
		using RawDataExtractor = DataExtractor<RawData>;
		using RawDataParser = Parser<engine::Data *(const RawData&)>;
		using RawDataSerializer = Serializer<RawData(const engine::Data&)>;		
		
		McuServer(McuEngine *engine, const RawDataSender& sender, const RawDataExtractor& extractor, const RawDataParser& parser, const RawDataSerializer& serializer);
		McuServer(const McuServer& other) = delete;
		McuServer& operator=(const McuServer& other) = delete;
		virtual ~McuServer() noexcept = default;

		void feed(const RawData& data);
	private:
		McuEngine *m_engine;
		std::unique_ptr<RawDataSender> m_sender;
		std::unique_ptr<RawDataExtractor> m_extractor;
		std::unique_ptr<RawDataParser> m_parser;
		std::unique_ptr<RawDataSerializer> m_serializer;
		RawData m_data;
	};

	template <typename Tgpio_id>
	McuServer<Tgpio_id>::McuServer(McuEngine *engine, const RawDataSender& sender, const RawDataExtractor& extractor, const RawDataParser& parser, const RawDataSerializer& serializer): m_engine(engine), m_sender(sender.clone()), m_extractor(extractor.clone()), m_parser(parser.clone()), m_serializer(serializer.clone()) {
		if (!engine) {
			throw std::invalid_argument("invalid engine ptr received");
		}
	}

	template <typename Tgpio_id>
	void McuServer<Tgpio_id>::feed(const RawData& data) {
		m_data.insert(m_data.end(), data.begin(), data.end());
		if (!(m_extractor->is_extractable(m_data))) {
			return;
		}
		auto extracted_data = m_extractor->extract(&m_data);
		const std::unique_ptr<engine::Data> parsed_data(m_parser->parse(extracted_data));
		const std::unique_ptr<engine::Data> report(m_engine->run(*parsed_data));
		auto serialized_report = m_serializer->serialize(*report);
		m_sender->send(serialized_report);
	}
}

#endif // MCU_SERVER_HPP