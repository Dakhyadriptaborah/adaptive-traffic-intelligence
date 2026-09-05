#ifndef TRAFFIC_ANALYZER_HPP
#define TRAFFIC_ANALYZER_HPP

#include "traffic_stats.hpp"

class TrafficAnalyzer {
public:
    explicit TrafficAnalyzer(double roadCapacity);

    TrafficStats analyze(int vehicleCount) const;

private:
    double roadCapacity;

    double calculateDensity(int vehicleCount) const;
    std::string classifyTraffic(double density) const;
    int calculateGreenTime(double density) const;
};

#endif
