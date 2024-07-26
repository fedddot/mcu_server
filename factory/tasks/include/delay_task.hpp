#ifndef	DELAY_TASK_HPP
#define	DELAY_TASK_HPP

#include <functional>
#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tgpio_id>
	class DelayTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using DelayFunction = std::function<void(unsigned int)>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(int)>;
		
		DelayTask(const DelayFunction& delay_function, unsigned int delay_ms, const ReportCreator& report_ctor);
		DelayTask(const DelayTask& other) = delete;
		DelayTask& operator=(const DelayTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		const DelayFunction m_delay_function;
		const unsigned int m_delay_ms;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Tgpio_id>
	inline DelayTask<Tgpio_id>::DelayTask(const DelayFunction& delay_function, unsigned int delay_ms, const ReportCreator& report_ctor): m_delay_function(delay_function), m_delay_ms(delay_ms), m_report_ctor(report_ctor.clone()) {
		if (!m_delay_function) {
			throw std::invalid_argument("invalid delay function received");
		}
	}

	template <class Tgpio_id>
	inline mcu_server::Data *DelayTask<Tgpio_id>::execute() const {
		m_delay_function(m_delay_ms);
		return m_report_ctor->create(0);
	}
}
#endif // DELAY_TASK_HPP