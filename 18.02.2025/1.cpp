#include <opencv2/opencv.hpp>

int main() {
    // загружаем изображение
    cv::Mat image = cv::imread("C:/Users/gilyo/duck_and_beer.jfif");

    // проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // определяем размеры
    int width = image.cols;
    int height = image.rows;
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    // создаем пустые матрицы для каждой части изображения
    cv::Mat topLeft(halfHeight, halfWidth, image.type());
    cv::Mat topRight(halfHeight, halfWidth, image.type());
    cv::Mat bottomLeft(halfHeight, halfWidth, image.type());
    cv::Mat bottomRight(halfHeight, halfWidth, image.type());

    // копируем части изображения с помощью copyTo
    image(cv::Rect(0, 0, halfWidth, halfHeight)).copyTo(topLeft);       // левый верхний угол
    image(cv::Rect(halfWidth, 0, halfWidth, halfHeight)).copyTo(topRight); // правый верхний угол
    image(cv::Rect(0, halfHeight, halfWidth, halfHeight)).copyTo(bottomLeft); // левый нижний угол
    image(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight)).copyTo(bottomRight); // правый нижний угол

    // применяем обработку
    cv::bitwise_not(topRight, topRight);  // инверсия цветов

    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_BGR2GRAY); // ЧБ-версия
    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_GRAY2BGR); // преобразуем обратно в 3 канала

    bottomRight.setTo(cv::Scalar(0, 255, 0)); // заменить на зеленый

    // объединяем части обратно
    cv::Mat topHalf, bottomHalf, finalImage;
    cv::hconcat(topLeft, topRight, topHalf);  // соединяем верхние части
    cv::hconcat(bottomLeft, bottomRight, bottomHalf);  // соединяем нижние части
    cv::vconcat(topHalf, bottomHalf, finalImage);  // объединяем верх и низ

    // отображаем оригинальное и обработанное изображение
    cv::imshow("Original Image", image);
    cv::imshow("Processed Image", finalImage);

    // ожидание нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
