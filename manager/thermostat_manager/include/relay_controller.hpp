#ifndef RELAY_CONTROLLER_HPP
#define RELAY_CONTROLLER_HPP

namespace manager {
    class RelayController {
    public:
        virtual ~RelayController() noexcept = default;
        virtual void set_relay_state(const bool state) = 0;
    };
}

#endif // RELAY_CONTROLLER_HPP
