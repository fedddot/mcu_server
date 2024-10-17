#ifndef	INVENTORY_HPP
#define	INVENTORY_HPP

#include <vector>
namespace manager {
	template <typename Tid, typename Tstored>
	class Inventory {
	public:
		virtual ~Inventory() noexcept = default;
		virtual void add(const Tid& id, Tstored *item) = 0;
		virtual Tstored *remove(const Tid& id) = 0;
		virtual bool contains(const Tid& id) const = 0;
		virtual Tstored& access(const Tid& id) = 0;
		virtual const Tstored& access(const Tid& id) const = 0;
		virtual const std::vector<Tid> ids() const = 0;
	};
}

#endif // INVENTORY_HPP