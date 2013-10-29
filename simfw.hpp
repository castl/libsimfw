#ifndef __SIMFW_HPP__
#define __SIMFW_HPP__

#include "time.hpp"

#include <queue>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace simfw {

    template <class Time = TimeInPS>
    class Simulation;

    template <class Time>
    struct Message {
        typedef boost::function<void (Time)> Delivery;
        Time deliverAt;
        Delivery delivery;

        Message(Time deliverAt, Delivery delivery) :
            deliverAt(deliverAt),
            delivery(delivery) {
        }

        bool operator<(const Message& b) const {
            return this->deliverAt > b.deliverAt;
        }
    };

    template<class Time, class Content>
    class InputPort {
        friend class Simulation<Time>;

    protected:
        Simulation<Time>* simulation;

        InputPort(Simulation<Time>* simulation) :
            simulation(simulation) {
        }

        virtual void recieve(Time time, Content content) { }

    public:
        void deliverAt(Time time, Content msg) {
            assert(simulation != NULL);
            simulation->queue(
                Message<Time>(time, boost::bind(&InputPort::recieve, this, _1, msg)));
        }

        void deliverIn(Time time, Content msg) {
            assert(simulation != NULL);
            deliverAt(simulation->now() + time, msg);
        }

        void deliverNow(Content msg) {
            assert(simulation != NULL);
            deliverAt(simulation->now(), msg);
        }
    };

    template <class Time>
    class Simulation {
        Time _now;
        std::priority_queue< Message<Time> > messages;

        void processNext() {
            Message<Time> msg = messages.top();
            messages.pop();
            this->_now = msg.deliverAt;
            msg.delivery(this->_now);
        }

    public:
        Simulation() {

        }

        Time now() {
            return _now;
        }

        void queue(Message<Time> msg) {
            assert(msg.deliverAt >= this->_now);
            messages.push(msg);
        }

        void goForever() {
            while (messages.size() > 0) {
                this->processNext();
            }
        }

        void goUntil(Time until) {
            while (messages.size() > 0 && this->_now < until) {
                this->processNext();
            }
        }
    };
}

#endif // __SIMFW_HPP__