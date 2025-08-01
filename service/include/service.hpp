#ifndef	SERVICE_HPP
#define	SERVICE_HPP

namespace service {
	template <typename ApiRequest, typename ApiResponse>
	class Service {
	public:
		virtual ~Service() noexcept = default;
		virtual ApiResponse run_api_request(const ApiRequest& request) = 0;
	};
}

#endif // SERVICE_HPP