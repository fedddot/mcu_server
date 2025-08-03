#ifndef	DATA_READER_HPP
#define	DATA_READER_HPP

namespace ipc {
	template <typename Signature>
	class DataReader;

	template <typename Result, typename... Args>
	class DataReader<Result(Args...)> {
	public:
		virtual ~DataReader() noexcept = default;
		virtual Result read(Args...) const = 0;
	};
}

#endif // DATA_READER_HPP