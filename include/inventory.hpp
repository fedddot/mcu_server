#ifndef	INVENTORY_HPP
#define	INVENTORY_HPP

namespace server {
	template <class Tkey, class Titem>
	class Inventory {
	public:
		virtual ~Inventory() noexcept = default;
		virtual void put(const Tkey& key, Titem *item) = 0;
		virtual Titem *pull(const Tkey& key) = 0;
		virtual Titem *access(const Tkey& key) const = 0;
		virtual bool contains(const Tkey& key) const = 0;
	};
}
#endif // INVENTORY_HPP