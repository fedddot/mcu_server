#ifndef	OBJECT_HPP
#define	OBJECT_HPP

#include <functional>
#include <string>
#include <map>
#include <memory>

#include "data.hpp"

namespace data {
	class Object: public Data {
	public:
		Object() = default;
		Object(const Object& other);
		Object& operator=(const Object& other);

		virtual Type type() const override;
		virtual Data *clone() const override;

		std::size_t size() const;
		bool contains(const std::string& name) const;
		Data& access(const std::string &name);
		const Data& access(const std::string& name) const;
		void remove(const std::string& name);
		void add(const std::string& name, const Data& data);
		void for_each(const std::function<void(const std::string&, const Data&)>& action) const;
	private:
		using DataContainer = std::map<std::string, std::unique_ptr<Data>>;
		DataContainer m_data;
		void throw_if_not_found(const std::string& name) const;
		static DataContainer copy_container(const Object& obj);
	};
} // namespace data

#endif // OBJECT_HPP