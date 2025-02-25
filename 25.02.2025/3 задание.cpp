#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Загружаем изображение
    cv::Mat src = cv::imread("C:/Users/gilyo/duck_and_beer.jfif");
    if (src.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // Преобразуем изображение в HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // Определяем диапазон цвета (жёлтый)
    cv::Scalar lower_yellow(20, 100, 100);
    cv::Scalar upper_yellow(30, 255, 255);

    // Создаём бинарную маску
    cv::Mat mask;
    cv::inRange(hsv, lower_yellow, upper_yellow, mask);

    // Применяем маску к оригинальному изображению
    cv::Mat result;
    cv::bitwise_and(src, src, result, mask);

    // Отображаем результаты
    cv::imshow("Оригинал", src);
    cv::imshow("Маска", mask);
    cv::imshow("Результат", result);

    cv::waitKey(0);
    return 0;
}
