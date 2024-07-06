#ifndef	DATA_EXTRACTOR_HPP
#define	DATA_EXTRACTOR_HPP

namespace mcu_server {
	
	template <typename Tdata>
	class DataExtractor {
	public:
		virtual ~DataExtractor() noexcept = default;
		virtual bool is_extractable(const Tdata& data) const = 0;
		virtual Tdata extract(Tdata *data) const = 0;
		virtual DataExtractor *clone() const = 0;
	};
}

#endif // DATA_EXTRACTOR_HPP