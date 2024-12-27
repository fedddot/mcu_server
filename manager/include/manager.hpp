#ifndef	MANAGER_HPP
#define	MANAGER_HPP

namespace manager {
	template <typename Trequest, typename Tresponse>
	class Manager {
	public:
		virtual ~Manager() noexcept = default;
		virtual Tresponse run(const Trequest& request) = 0;
	};
}

#endif // MANAGER_HPP