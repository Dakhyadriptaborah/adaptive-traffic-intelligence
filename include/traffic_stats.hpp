#ifndef TRAFFIC_STATS_HPP
#define TRAFFIC_STATS_HPP

#include <string>

struct TrafficStats {
    int vehicleCount;
    double density;
    std::string congestionLevel;
    int recommendedGreenTime;
};

#endif
