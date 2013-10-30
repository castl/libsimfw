#ifndef __TIME_HPP__
#define __TIME_HPP__

#include <sys/time.h>

namespace simfw {
    template <const unsigned long long ConvToSec>
    struct FixedPointTime {
        unsigned long long t;

        FixedPointTime() {
            this->t = 0;
        }

        FixedPointTime(double seconds) {
            this->t = seconds * ConvToSec;
        }

        FixedPointTime(unsigned long long t) {
            this->t = t;
        }
         
        double operator()() const {
            return this->seconds();
        }

        double seconds() const {
            return ((double)t) / ConvToSec;
        }

        FixedPointTime operator+(FixedPointTime t) {
            return FixedPointTime(this->t + t.t);
        }

        void operator+=(FixedPointTime t) {
            this->t += t.t;
        }

        bool operator<(FixedPointTime t) const {
            return this->t < t.t;
        }

        bool operator>(FixedPointTime t) const {
            return this->t > t.t;
        }

        bool operator<=(FixedPointTime t) const {
            return this->t <= t.t;
        }

        bool operator>=(FixedPointTime t) const {
            return this->t >= t.t;
        }

        bool operator==(FixedPointTime t) const {
            return this->t == t.t;
        }
    };

    typedef FixedPointTime<1000000000000> TimeInPS;

    double host_time() {
        struct timeval tv;
        gettimeofday(&tv, 0);
        return tv.tv_sec + (tv.tv_usec / 1000000.0);
    }
}

#endif // __TIME_HPP__