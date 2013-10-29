#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "simfw.hpp"
#include <stdint.h>

namespace simfw {

    template<class Time>
    class Timer : InputPort<Time, uint64_t> {
        Time every;
        boost::function<bool(uint64_t)> func;

    protected:
        Timer(Simulation<Time>* sim, Time every) :
            InputPort<Time, uint64_t>(sim),
            every(every) {

        }

    public:
        Timer(Simulation<Time>* sim, Time every, boost::function<bool(uint64_t)> func):
            InputPort<Time, uint64_t>(sim),
            every(every),
            func(func) {
            this->deliverIn(every, 1);
        }

    protected:
        virtual bool ding(uint64_t i) {
            return func(i);
        };

        virtual void recieve(Time time, uint64_t i) {
            if (ding(i))
                this->deliverIn(every, i + 1);
        }
    };
}

#endif // __TIMER_HPP__