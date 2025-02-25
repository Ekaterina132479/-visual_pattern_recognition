#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Загружаем изображение
    cv::Mat src = cv::imread("C:/Users/gilyo/duck_and_beer.jfif");
    if (src.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // Копируем изображение для линий и окружностей
    cv::Mat image_lines = src.clone();
    cv::Mat image_circles = src.clone();

    // 1. Преобразование в оттенки серого
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // 2. Обнаружение линий методом преобразования Хафа
    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150, 3);
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(edges, lines, 1, CV_PI / 180, 150);

    // Рисуем линии на image_lines
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(image_lines, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    // 3. Сглаживание перед поиском окружностей
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

    // 4. Обнаружение окружностей методом HoughCircles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows / 8, 100, 50, 0, 0);

    // Рисуем окружности на image_circles
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Vec3i c = circles[i];
        cv::Point center(c[0], c[1]);
        int radius = c[2];
        cv::circle(image_circles, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA); // Центр
        cv::circle(image_circles, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA); // Окружность
    }

    // 5. Вывод изображений
    cv::imshow("Линии", image_lines);
    cv::imshow("Окружности", image_circles);

    cv::waitKey(0);
    return 0;
}
