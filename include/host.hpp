#ifndef	SERVER_HPP
#define	SERVER_HPP

namespace server {
	class Server {
	public:
		virtual ~Server() noexcept = default;
		virtual void run() = 0;
		virtual bool is_running() const = 0;
		virtual void stop() = 0;
	};
}

#endif // SERVER_HPP