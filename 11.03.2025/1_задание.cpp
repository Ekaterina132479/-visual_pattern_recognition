#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap("video.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Ошибка: не удалось открыть видеофайл!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Улучшение контрастности и яркости
        cv::Mat enhanced;
        frame.convertTo(enhanced, -1, 1.2, 30); // Контраст 1.2, яркость +30

        cv::Mat blurred, gray, edges;
        cv::GaussianBlur(enhanced, blurred, cv::Size(5, 5), 1.2);
        cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, 75, 200); // Оптимизированные границы

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double peri = cv::arcLength(contour, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contour, approx, 0.02 * peri, true);

            if (approx.size() == 4 && cv::contourArea(approx) > 1000 && cv::isContourConvex(approx)) {
                cv::Rect boundingBox = cv::boundingRect(approx);
                cv::Mat roi = gray(boundingBox);
                double meanIntensity = cv::mean(roi)[0];

                if (meanIntensity > 200) { // Убеждаемся, что квадрат белый
                    cv::drawContours(enhanced, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 255, 0), 2);

                    // Подпись в центре квадрата
                    cv::Point center(boundingBox.x + boundingBox.width / 2, boundingBox.y + boundingBox.height / 2);
                    cv::putText(enhanced, "Square", center, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
                }
            }
        }

        cv::imshow("Видео", enhanced);
        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
