#include <opencv2/opencv.hpp>

int main() {
    // загружаем изображение
    cv::Mat image = cv::imread("C:/Users/gilyo/duck_and_beer.jfif");

    // проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // задаём размер изображения
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(), 1, 1);

    // линия
    cv::line(resizedImage, cv::Point(50, 50), cv::Point(300, 200), cv::Scalar(0, 255, 0), 2);

    // прямоугольник
    cv::rectangle(resizedImage, cv::Point(100, 100), cv::Point(250, 250), cv::Scalar(255, 0, 0), 2);

    // круг
    cv::circle(resizedImage, cv::Point(200, 150), 50, cv::Scalar(0, 0, 255), 2);

    // эллипс 
    cv::ellipse(resizedImage, cv::Point(300, 150), cv::Size(100, 50), 30, 0, 360, cv::Scalar(255, 255, 0), 2);

    // многоугольник 
    std::vector<cv::Point> points = {
        cv::Point(150, 100), cv::Point(200, 50), cv::Point(250, 100),
        cv::Point(225, 150), cv::Point(175, 150)
    };
    cv::polylines(resizedImage, points, true, cv::Scalar(255, 0, 255), 2);

    // текст 
    cv::putText(resizedImage, "Duck drinks beer", cv::Point(50, 300),
        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);

    // отображаем изображение с нарисованными элементами
    cv::imshow("Drawn Elements", resizedImage);

    // ожидание нажатия клавиши перед закрытием окна
    cv::waitKey(0);

    return 0;
}
