#include "timer.hpp"
#include <cstdio>

using namespace simfw;

uint64_t deliveries;

int main(void) {
    typedef simfw::TimeInPS Time;

    Simulation<Time>* sim = new Simulation<Time>();
    Timer<Time> a(sim, 0.001, [] (uint64_t i) { 
        deliveries += 1;
        return true;
    });

    Timer<Time> b(sim, 0.0001, [] (uint64_t i) { 
        deliveries += 1;
        return true;
    });

    Timer<Time> c(sim, 0.00003, [] (uint64_t i) { 
        deliveries += 1;
        return true;
    });

    Timer<Time> d(sim, 0.0002, [] (uint64_t i) { 
        deliveries += 1;
        return true;
    });

    deliveries = 0;
    double start = host_time(); 
    sim->goUntil(100.0);
    double end = host_time();
    double runtime = end - start;
    printf("Runtime: %le\n", runtime);
    printf("Deliveries / second: %le\n", deliveries / runtime);
}