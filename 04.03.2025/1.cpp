#include <opencv2/opencv.hpp>
#include <iostream>

// Функция для определения типа фигуры
std::string getShapeName(std::vector<cv::Point> contour) {
    // Аппроксимация контура, чтобы определить количество углов
    std::vector<cv::Point> approx;
    double peri = cv::arcLength(contour, true);  // Периметр контура
    cv::approxPolyDP(contour, approx, 0.02 * peri, true);  // Аппроксимация

    int vertices = approx.size();  // Количество углов

    // Определение фигуры по количеству вершин
    if (vertices == 3) {
        return "triangle";  // Треугольник
    }
    else if (vertices == 4) {
        return "square";  // Квадрат
    }
    else {
        return "circle";  // Окружность 
    }
}

int main() {
    // Загрузка изображения
    cv::Mat src = cv::imread("C:/Users/gilyo/figure.png");
    if (src.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // Преобразование в оттенки серого
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // Применение Гауссова размытия для уменьшения шума
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);

    // Обнаружение границ методом Canny
    cv::Mat edges;
    cv::Canny(blurred, edges, 10, 50);

    // Поиск контуров
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Создание изображения для отрисовки контуров
    cv::Mat contourOutput = cv::Mat::zeros(src.size(), CV_8UC3);

    // Обработка каждого контура
    for (size_t i = 0; i < contours.size(); i++) {
        // Определение фигуры
        std::string shape = getShapeName(contours[i]);

        // Рисуем контур
        cv::drawContours(contourOutput, contours, static_cast<int>(i), cv::Scalar(255, 0, 0), 2);

        // Вычисляем центр фигуры для подписи
        cv::Moments M = cv::moments(contours[i]);
        if (M.m00 != 0) {
            int cx = static_cast<int>(M.m10 / M.m00);
            int cy = static_cast<int>(M.m01 / M.m00);

            // Выводим название фигуры
            cv::putText(contourOutput, shape, cv::Point(cx - 20, cy), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        }
    }

    // Отображение изображений
    cv::imshow("Исходное изображение", src);
    cv::imshow("Контуры", contourOutput);
    cv::waitKey(0);

    return 0;
}


