#include "timer.hpp"
#include <cstdio>

using namespace simfw;

int main(void) {
    typedef simfw::TimeInPS Time;

    Simulation<Time>* sim = new Simulation<Time>();
    Timer<Time> a(sim, 0.001, [] (uint64_t i) { 
        printf("a %lu\n", i);
        return i < 5;
    });

    Timer<Time> b(sim, 0.0001, [] (uint64_t i) { 
        printf("b %lu\n", i);
        return i < 2;
    });

    sim->goForever();
}