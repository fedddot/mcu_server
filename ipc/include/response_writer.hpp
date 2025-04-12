#ifndef	IPC_DATA_WRITER_HPP
#define	IPC_DATA_WRITER_HPP

namespace ipc {
	/// @brief Synchronous IPC Responses Writer
	template <typename Response>
	class ResponseWriter {
	public:
		virtual ~ResponseWriter() noexcept = default;

		/// @brief Writes an IPC response into an IPC stream 
		virtual void write(const Response& response) const = 0;
	};
}

#endif // IPC_DATA_WRITER_HPP