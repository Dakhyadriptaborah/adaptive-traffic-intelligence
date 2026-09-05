#include <opencv2/opencv.hpp>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "traffic_analyzer.hpp"

namespace {

constexpr double ROAD_CAPACITY = 30.0;

// Minimum contour area used to remove small noise.
constexpr double MIN_OBJECT_AREA = 500.0;

// Minimum dimensions for a detected region.
constexpr int MIN_OBJECT_WIDTH = 20;
constexpr int MIN_OBJECT_HEIGHT = 20;

} // namespace

int main() {

    const std::string videoPath =
        "data/traffic.mp4";

    cv::VideoCapture video(videoPath);

    if (!video.isOpened()) {

        std::cerr
            << "Error: Could not open traffic video: "
            << videoPath
            << '\n';

        return 1;
    }

    TrafficAnalyzer analyzer(ROAD_CAPACITY);

    // Background subtraction model.
    cv::Ptr<cv::BackgroundSubtractor>
        backgroundSubtractor =
            cv::createBackgroundSubtractorMOG2(
                500,
                16,
                true
            );

    cv::Mat frame;
    cv::Mat foregroundMask;

    std::cout
        << "Adaptive Traffic Intelligence System\n"
        << "======================================\n";

    std::cout
        << "Press ESC to terminate.\n\n";

    while (video.read(frame)) {

        /*
         * Step 1:
         * Extract moving regions from the traffic video.
         */
        backgroundSubtractor->apply(
            frame,
            foregroundMask
        );

        /*
         * Step 2:
         * Remove small noise using morphological
         * operations.
         */
        const cv::Mat kernel =
            cv::getStructuringElement(
                cv::MORPH_RECT,
                cv::Size(5, 5)
            );

        cv::morphologyEx(
            foregroundMask,
            foregroundMask,
            cv::MORPH_OPEN,
            kernel
        );

        cv::morphologyEx(
            foregroundMask,
            foregroundMask,
            cv::MORPH_CLOSE,
            kernel
        );

        /*
         * Step 3:
         * Find connected moving regions.
         */
        std::vector<std::vector<cv::Point>>
            contours;

        cv::findContours(
            foregroundMask,
            contours,
            cv::RETR_EXTERNAL,
            cv::CHAIN_APPROX_SIMPLE
        );

        int detectedObjects = 0;

        /*
         * Step 4:
         * Filter regions that are too small to
         * represent meaningful traffic objects.
         */
        for (const auto& contour : contours) {

            const double area =
                cv::contourArea(contour);

            if (area < MIN_OBJECT_AREA) {
                continue;
            }

            const cv::Rect boundingBox =
                cv::boundingRect(contour);

            if (boundingBox.width <
                    MIN_OBJECT_WIDTH ||
                boundingBox.height <
                    MIN_OBJECT_HEIGHT) {

                continue;
            }

            ++detectedObjects;

            /*
             * Bounding boxes are used only for
             * visual validation of the prototype.
             */
            cv::rectangle(
                frame,
                boundingBox,
                cv::Scalar(0, 255, 0),
                2
            );
        }

        /*
         * Step 5:
         * Analyze the detected traffic.
         */
        const TrafficStats stats =
            analyzer.analyze(
                detectedObjects
            );

        /*
         * Step 6:
         * Keep analysis separate from visualization.
         *
         * Results are printed to the terminal
         * instead of being rendered on the video.
         */
        std::cout
            << "Objects: "
            << stats.vehicleCount
            << " | Density: "
            << std::fixed
            << std::setprecision(1)
            << stats.density
            << "% | Congestion: "
            << stats.congestionLevel
            << " | Recommended Green: "
            << stats.recommendedGreenTime
            << " sec"
            << '\n';

        /*
         * Step 7:
         * Display the processed video.
         */
        cv::imshow(
            "Adaptive Traffic Intelligence",
            frame
        );

        /*
         * Press ESC to stop.
         */
        const int key =
            cv::waitKey(30);

        if (key == 27) {
            break;
        }
    }

    video.release();

    cv::destroyAllWindows();

    std::cout
        << "\nTraffic analysis terminated.\n";

    return 0;
}
