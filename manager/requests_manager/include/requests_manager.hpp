#ifndef	REQUESTS_MANAGER_HPP
#define	REQUESTS_MANAGER_HPP

#include <exception>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "data.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"

namespace manager {
	class RequestsManager: public server::Resource {
	public:
		using RequestsRetriever = std::function<std::vector<server::Request>(const server::Data&)>;

		RequestsManager(server::Resource *vendor, const RequestsRetriever& requests_retriever);
		RequestsManager(const RequestsManager& other) = default;
		RequestsManager& operator=(const RequestsManager&) = delete;
		
		server::Response run_request(const server::Request& request) const override;
		server::Resource *clone() const override;
	private:
		server::Resource *m_vendor;
		RequestsRetriever m_requests_retriever;

		using ResponseCode = typename server::Response::ResponseCode;
		static server::Response report_failure(const ResponseCode& code, const std::string& what);
	};

	inline RequestsManager::RequestsManager(server::Resource *vendor, const RequestsRetriever& requests_retriever): m_vendor(vendor), m_requests_retriever(requests_retriever) {
		if (!m_vendor || !m_requests_retriever) {
			throw std::invalid_argument("invalid argument received");
		}
	}
	
	inline server::Response RequestsManager::run_request(const server::Request& request) const {
		using Method = typename server::Request::Method;
		if (Method::UPDATE != request.method()) {
			return report_failure(ResponseCode::BAD_REQUEST, "unsupported method request received");
		}
		try {
			auto requests(m_requests_retriever(request.body()));
			for (const auto& request: requests) {
				auto response(m_vendor->run_request(request));
				if (ResponseCode::OK != response.code()) {
					return response;
				}
			}
			return server::Response(ResponseCode::OK, server::Response::Body());
		} catch (const std::exception& e) {
			return report_failure(ResponseCode::UNSPECIFIED, std::string(e.what()));
		}
	}

	
	inline server::Resource *RequestsManager::clone() const {
		return new RequestsManager(*this);
	}
}

#endif // REQUESTS_MANAGER_HPP