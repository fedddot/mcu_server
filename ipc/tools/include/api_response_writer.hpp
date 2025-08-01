#ifndef	API_REQUEST_WRITER_HPP
#define	API_REQUEST_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

#include "ipc_data_writer.hpp"
#include "package_writer.hpp"

namespace ipc {
	template <typename ApiResponse, std::size_t HSIZE>
	class ApiResponseWriter: public DataWriter<ApiResponse> {
	public:
		using HeaderGenerator = typename PackageWriter<HSIZE>::HeaderGenerator;
		using RawDataWriter = typename PackageWriter<HSIZE>::RawDataWriter;
		using ApiResponseSerializer = std::function<std::vector<std::uint8_t>(const ApiResponse&)>;

		ApiResponseWriter(
			const HeaderGenerator& header_generator,
			const RawDataWriter& raw_data_writer,
			const ApiResponseSerializer& response_serializer
		): m_package_writer(header_generator, raw_data_writer), m_response_serializer(response_serializer) {
			if (!m_response_serializer) {
				throw std::invalid_argument("invalid args received");
			}
		}
		ApiResponseWriter(const ApiResponseWriter&) = default;
		ApiResponseWriter& operator=(const ApiResponseWriter&) = default;
		void write(const ApiResponse& response) const override {
			const auto package_data = m_response_serializer(response);
			m_package_writer.write(package_data);
		}
	private:
		PackageWriter<HSIZE> m_package_writer;
		ApiResponseSerializer m_response_serializer;
	};
}

#endif // API_REQUEST_WRITER_HPP