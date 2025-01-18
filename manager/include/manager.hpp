#ifndef	MANAGER_HPP
#define	MANAGER_HPP

namespace manager {
	template <typename Request, typename Response>
	class Manager {
	public:
		virtual ~Manager() noexcept = default;
		virtual Response run(const Request& request) = 0;
	};
}

#endif // MANAGER_HPP