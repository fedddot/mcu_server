#ifndef	MCU_FACTORY_HPP
#define	MCU_FACTORY_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"
#include "task_creator.hpp"

namespace mcu_factory {
	class McuFactory: public server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> {
	public:
		McuFactory() = default;
		McuFactory(const McuFactory& other);
		McuFactory& operator=(const McuFactory& other) = delete;

		server::Task<server::Data *(void)> *create(const server::Data& data) const override;
		server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *clone() const override;
		void add_task_creator(const TaskCreator& creator);
	private:
		std::vector<std::unique_ptr<TaskCreator>> m_ctors;
	};

	inline McuFactory::McuFactory(const McuFactory& other) {
		for (const auto& ctor: other.m_ctors) {
			m_ctors.push_back(std::unique_ptr<TaskCreator>(ctor->clone_task_creator()));
		}
	}

	inline server::Task<server::Data *(void)> *McuFactory::create(const server::Data& data) const {
		for (const auto& ctor_ptr: m_ctors) {
			if (!(ctor_ptr->is_creatable(data))) {
				continue;
			}
			return ctor_ptr->create(data);
		}
		throw std::invalid_argument("increatable data received");
	}
	
	inline server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *McuFactory::clone() const {
		return new McuFactory(*this);
	}

	inline void McuFactory::add_task_creator(const TaskCreator& creator) {
		m_ctors.push_back(std::unique_ptr<TaskCreator>(creator.clone_task_creator()));
	}
}
#endif // MCU_FACTORY_HPP