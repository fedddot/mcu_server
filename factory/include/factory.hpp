#ifndef	FACTORY_HPP
#define	FACTORY_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"
#include "task_creator.hpp"

namespace mcu_factory {
	class Factory: public server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> {
	public:
		Factory() = default;
		Factory(const Factory& other);
		Factory& operator=(const Factory& other) = delete;

		server::Task<server::Data *(void)> *create(const server::Data& data) const override;
		server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *clone() const override;
		void add_task_creator(const TaskCreator& creator);
	private:
		std::vector<std::unique_ptr<TaskCreator>> m_ctors;
	};

	inline Factory::Factory(const Factory& other) {
		for (const auto& ctor: other.m_ctors) {
			m_ctors.push_back(std::unique_ptr<TaskCreator>(ctor->clone_task_creator()));
		}
	}

	inline server::Task<server::Data *(void)> *Factory::create(const server::Data& data) const {
		for (const auto& ctor_ptr: m_ctors) {
			if (!(ctor_ptr->is_creatable(data))) {
				continue;
			}
			return ctor_ptr->create(data);
		}
		throw std::invalid_argument("increatable data received");
	}
	
	inline server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *Factory::clone() const {
		return new Factory(*this);
	}

	inline void Factory::add_task_creator(const TaskCreator& creator) {
		m_ctors.push_back(std::unique_ptr<TaskCreator>(creator.clone_task_creator()));
	}
}
#endif // FACTORY_HPP