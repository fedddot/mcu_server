#ifndef	DELAY_TASK_HPP
#define	DELAY_TASK_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "delay.hpp"
#include "task.hpp"

namespace mcu_factory {
	class DelayTask: public server::Task<server::Data *(void)> {
	public:
		using DelayCreator = server::Creator<mcu_platform::Delay *(void)>;
		using ReportCreator = server::Creator<server::Data *(int)>;
		
		DelayTask(const DelayCreator& delay_ctor, unsigned int delay_ms, const ReportCreator& report_ctor);
		DelayTask(const DelayTask& other) = delete;
		DelayTask& operator=(const DelayTask& other) = delete;
		
		server::Data *execute() const override;
	private:
		const std::unique_ptr<DelayCreator> m_delay_ctor;
		const unsigned int m_delay_ms;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	inline DelayTask::DelayTask(const DelayCreator& delay_ctor, unsigned int delay_ms, const ReportCreator& report_ctor): m_delay_ctor(delay_ctor.clone()), m_delay_ms(delay_ms), m_report_ctor(report_ctor.clone()) {

	}

	inline server::Data *DelayTask::execute() const {
		std::unique_ptr<mcu_platform::Delay> delay(m_delay_ctor->create());
		delay->delay(m_delay_ms);
		return m_report_ctor->create(0);
	}
}
#endif // DELAY_TASK_HPP