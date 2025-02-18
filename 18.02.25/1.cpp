#include <opencv2/opencv.hpp>

int main() {
    // загружаем изображение
    cv::Mat image = cv::imread("C:/Users/gilyo/duck_and_beer.jfif");

    // провер€ем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "ќшибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // определ€ем размеры
    int width = image.cols;
    int height = image.rows;
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    // раздел€ем изображение на 4 части
    cv::Mat topLeft = image(cv::Rect(0, 0, halfWidth, halfHeight)).clone();  // Ѕез изменений
    cv::Mat topRight = image(cv::Rect(halfWidth, 0, halfWidth, halfHeight)).clone();
    cv::Mat bottomLeft = image(cv::Rect(0, halfHeight, halfWidth, halfHeight)).clone();
    cv::Mat bottomRight = image(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight)).clone();

    // примен€ем обработку
    cv::bitwise_not(topRight, topRight);  // инверси€ цветов

    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_BGR2GRAY); // „Ѕ-верси€
    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_GRAY2BGR); // преобразуем обратно в 3 канала

    bottomRight.setTo(cv::Scalar(0, 255, 0)); // заменить на зеленый

    // объедин€ем части обратно
    cv::Mat topHalf, bottomHalf, finalImage;
    cv::hconcat(topLeft, topRight, topHalf);  // соедин€ем верхние части
    cv::hconcat(bottomLeft, bottomRight, bottomHalf);  // соедин€ем нижние части
    cv::vconcat(topHalf, bottomHalf, finalImage);  // объедин€ем верх и низ

    // отображаем оригинальное и обработанное изображение
    cv::imshow("Original Image", image);
    cv::imshow("Processed Image", finalImage);

    // ожидание нажати€ клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
