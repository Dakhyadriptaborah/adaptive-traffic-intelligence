#include "traffic_analyzer.hpp"

#include <algorithm>

TrafficAnalyzer::TrafficAnalyzer(double roadCapacity)
    : roadCapacity(roadCapacity) {
}

double TrafficAnalyzer::calculateDensity(
    int vehicleCount
) const {

    if (roadCapacity <= 0.0) {
        return 0.0;
    }

    const double density =
        (static_cast<double>(vehicleCount)
         / roadCapacity) * 100.0;

    return std::clamp(density, 0.0, 100.0);
}

std::string TrafficAnalyzer::classifyTraffic(
    double density
) const {

    if (density < 25.0) {
        return "LOW";
    }

    if (density < 60.0) {
        return "MEDIUM";
    }

    if (density < 80.0) {
        return "HIGH";
    }

    return "SEVERE";
}

int TrafficAnalyzer::calculateGreenTime(
    double density
) const {

    constexpr int MIN_GREEN_TIME = 20;
    constexpr int MAX_GREEN_TIME = 90;

    const int greenTime =
        MIN_GREEN_TIME +
        static_cast<int>(
            (density / 100.0) *
            (MAX_GREEN_TIME - MIN_GREEN_TIME)
        );

    return std::clamp(
        greenTime,
        MIN_GREEN_TIME,
        MAX_GREEN_TIME
    );
}

TrafficStats TrafficAnalyzer::analyze(
    int vehicleCount
) const {

    const double density =
        calculateDensity(vehicleCount);

    return {
        vehicleCount,
        density,
        classifyTraffic(density),
        calculateGreenTime(density)
    };
}
