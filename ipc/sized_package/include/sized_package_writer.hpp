#ifndef	SIZED_PACKAGE_WRITER_HPP
#define	SIZED_PACKAGE_WRITER_HPP

#include "response_writer.hpp"

namespace ipc {
	template <typename Response>
	class SizedPackageWriter: public ResponseWriter<Response> {
	public:
		void write(const Response& response) const override;
	};
}

#endif // SIZED_PACKAGE_WRITER_HPP