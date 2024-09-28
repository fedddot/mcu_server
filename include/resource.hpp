#ifndef	RESOURCE_HPP
#define	RESOURCE_HPP

namespace server {
	template <typename Tid, typename Signature>
	class Resource;

	template <typename Tid, typename Tresponse, typename... Args>
	class Resource<Tid, Tresponse(Args...)> {
	public:
		virtual ~Resource() noexcept = default;
		virtual Tid id() const = 0;
		virtual Tresponse run_request(Args...) const = 0;
		virtual Resource *clone() const = 0;
	};
}

#endif // RESOURCE_HPP