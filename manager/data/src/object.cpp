#include "object.hpp"
#include <algorithm>
#include <stdexcept>

using namespace manager;

void Object::for_each(const std::function<void(const std::string&, const Data&)>& action) const {
	std::for_each(
		m_data.begin(),
		m_data.end(),
		[&](const auto& entry) {
			action(entry.first, *entry.second);
		}
	);
}

Object::DataContainer Object::copy_container(const Object& obj) {
	std::map<std::string, std::unique_ptr<Data>> data;
	obj.for_each(
		[&](const std::string& name, const Data& member)-> void {
			data.insert({ name, std::unique_ptr<Data>(member.clone()) });
		}
	);
	return data;
}

Object::Object(const Object& other) {
	m_data = copy_container(other);
}

Object& Object::operator=(const Object& other) {
	m_data = copy_container(other);
	return *this;
}

std::size_t Object::size() const {
	return m_data.size();
}

bool Object::contains(const std::string& name) const {
	return (m_data.end() != m_data.find(name));
}

void Object::throw_if_not_found(const std::string &name) const {
	if (!contains(name)) {
		throw std::invalid_argument("data object does not contain field " + name);
	}
}

Data& Object::access(const std::string &name) { 
	throw_if_not_found(name);
	auto iter = m_data.find(name);
	return std::ref(*(iter->second));
}

const Data& Object::access(const std::string& name) const {
	throw_if_not_found(name);
	auto iter = m_data.find(name);
	return std::ref(*(iter->second));
}

void Object::remove(const std::string& name) {
	throw_if_not_found(name);
	m_data.erase(m_data.find(name));
}

void Object::add(const std::string& name, const Data& data) {
	if (contains(name)) {
		throw std::invalid_argument("data object already contains field " + name);
	}
	m_data.insert({ name, std::unique_ptr<Data>(data.clone()) });
}

Object::Type Object::type() const {
	return Type::OBJECT;
}

Data *Object::clone() const {
	return new Object(*this);
}