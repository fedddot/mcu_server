#ifndef	RAW_DATA_PACKAGE_WRITER_HPP
#define	RAW_DATA_PACKAGE_WRITER_HPP

#include <functional>
#include <vector>

#include "ipc_instance.hpp"
#include "ipc_data.hpp"
#include "ipc_data_writer.hpp"
#include "raw_data_package_descriptor.hpp"

namespace ipc {
	class RawDataPackageWriter: public IpcDataWriter<RawData>, public Clonable<IpcDataWriter<RawData>> {
	public:
		using PackageSizeSerializer = std::function<RawData(const RawDataPackageDescriptor&, const std::size_t&)>;
		using RawDataWriter = std::function<void(const RawData&)>;

		RawDataPackageWriter(
			const RawDataPackageDescriptor& descriptor,
			const PackageSizeSerializer& size_serializer,
			const RawDataWriter& raw_data_writer
		);
		RawDataPackageWriter(const RawDataPackageWriter&) = default;
		RawDataPackageWriter& operator=(const RawDataPackageWriter&) = default;
		void write(const RawData& data) const override;
		IpcDataWriter<RawData> *clone() const override;
	private:
		RawDataPackageDescriptor m_descriptor;
		PackageSizeSerializer m_size_serializer;
		RawDataWriter m_raw_data_writer;
	};

	inline RawDataPackageWriter::RawDataPackageWriter(
		const RawDataPackageDescriptor& descriptor,
		const PackageSizeSerializer& size_serializer,
		const RawDataWriter& raw_data_writer
	): m_descriptor(descriptor), m_size_serializer(size_serializer), m_raw_data_writer(raw_data_writer) {
		if (!m_raw_data_writer) {
			throw std::invalid_argument("invalid raw data writer received");
		}
		if (!m_size_serializer) {
			throw std::invalid_argument("invalid size serializer received");
		}
	}

	inline void RawDataPackageWriter::write(const RawData& data) const {
		const auto preamble = m_descriptor.preamble();
		const auto encoded_size = m_size_serializer(m_descriptor, data.size());
		auto raw_data = RawData();

		raw_data.insert(
			raw_data.end(),
			preamble.begin(),
			preamble.end()
		);
		raw_data.insert(
			raw_data.end(),
			encoded_size.begin(),
			encoded_size.end()
		);
		raw_data.insert(
			raw_data.end(),
			data.begin(),
			data.end()
		);
		m_raw_data_writer(raw_data);
	}

	inline IpcDataWriter<RawData> *RawDataPackageWriter::clone() const {
		return new RawDataPackageWriter(*this);
	}
}

#endif // RAW_DATA_PACKAGE_WRITER_HPP