#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Загружаем изображение
    Mat image = imread("C:/Users/gilyo/duck_and_beer.jfif");
    if (image.empty()) {
        cout << "Ошибка загрузки изображения!" << endl;
        return -1;
    }

    // Преобразование в различные цветовые пространства
    Mat image_hsv, image_lab, image_yuv, image_xyz, image_gray;
    cvtColor(image, image_hsv, COLOR_BGR2HSV);
    cvtColor(image, image_lab, COLOR_BGR2Lab);
    cvtColor(image, image_yuv, COLOR_BGR2YUV);
    cvtColor(image, image_xyz, COLOR_BGR2XYZ);
    cvtColor(image, image_gray, COLOR_BGR2GRAY);

    // Отображение изображений
    imshow("Original (BGR)", image);
    imshow("HSV", image_hsv);
    imshow("Lab", image_lab);
    imshow("YUV", image_yuv);
    imshow("XYZ", image_xyz);
    imshow("Grayscale", image_gray);

    // Ожидание нажатия клавиши перед выходом
    waitKey(0);
    return 0;
}
