#ifndef __SIMFW_HPP__
#define __SIMFW_HPP__

#include "time.hpp"

#include <queue>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace simfw {

    template <class Time = TimeInPS>
    class Simulation;

    template<class Time>
    class RawInputPort {
    public:
        virtual void recieveRaw(Time time, void* content) = 0;
        virtual void discardRaw(void* content) = 0;
    };

    template <class Time>
    struct Message {
        Time deliverAt;
        RawInputPort<Time>* port;
        void* content;

        Message(Time deliverAt, RawInputPort<Time>* port, void* content) :
            deliverAt(deliverAt),
            port(port),
            content(content) {
        }

        void discard() {
            port->discardRaw(content);
        }

        void delivery(Time time) {
            port->recieveRaw(time, content);
        }

        bool operator<(const Message& b) const {
            return this->deliverAt > b.deliverAt;
        }
    };


    template<class Time, class Content>
    class InputPort : public RawInputPort<Time> {
        friend class Simulation<Time>;

    protected:
        Simulation<Time>* simulation;

        InputPort(Simulation<Time>* simulation) :
            simulation(simulation) {
        }

        virtual void recieveRaw(Time time, void* content) {
            Content* realContent = (Content*) content;
            this->recieve(time, *realContent);
            delete realContent;
        }

        virtual void discardRaw(void* content) {
            Content* realContent = (Content*) content;
            delete realContent;
        }

        virtual void recieve(Time time, Content content) { }

    public:
        void deliverAt(Time time, Content msg) {
            assert(simulation != NULL);
            Content *cptr = new Content;
            *cptr = msg;
            simulation->queue(
                Message<Time>(time, this, cptr));
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
        Simulation() :
            _now(0.0) {
        }

        void flushMessages() {
            while (!messages.empty()) {
                auto msg = messages.top();
                messages.pop();
                msg.discard();
            }
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