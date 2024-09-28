#ifndef	RESOURCE_HPP
#define	RESOURCE_HPP

namespace server {
	template <typename Signature>
	class Resource;

	template <typename Tresponse, typename... Args>
	class Resource<Tresponse(Args...)> {
	public:
		virtual ~Resource() noexcept = default;
		virtual Tresponse run_request(Args...) const = 0;
		virtual Resource *clone() const = 0;
	};
}

#endif // RESOURCE_HPP