#include <opencv2/opencv.hpp>

int main() {
    // ��������� �����������
    cv::Mat image = cv::imread("C:/Users/gilyo/duck_and_beer.jfif");

    // ���������, ������� �� ��������� �����������
    if (image.empty()) {
        std::cerr << "������: �� ������� ��������� �����������!" << std::endl;
        return -1;
    }

    // ���������� �������
    int width = image.cols;
    int height = image.rows;
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    // ��������� ����������� �� 4 �����
    cv::Mat topLeft = image(cv::Rect(0, 0, halfWidth, halfHeight)).clone();  // ��� ���������
    cv::Mat topRight = image(cv::Rect(halfWidth, 0, halfWidth, halfHeight)).clone();
    cv::Mat bottomLeft = image(cv::Rect(0, halfHeight, halfWidth, halfHeight)).clone();
    cv::Mat bottomRight = image(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight)).clone();

    // ��������� ���������
    cv::bitwise_not(topRight, topRight);  // �������� ������

    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_BGR2GRAY); // ��-������
    cv::cvtColor(bottomLeft, bottomLeft, cv::COLOR_GRAY2BGR); // ����������� ������� � 3 ������

    bottomRight.setTo(cv::Scalar(0, 255, 0)); // �������� �� �������

    // ���������� ����� �������
    cv::Mat topHalf, bottomHalf, finalImage;
    cv::hconcat(topLeft, topRight, topHalf);  // ��������� ������� �����
    cv::hconcat(bottomLeft, bottomRight, bottomHalf);  // ��������� ������ �����
    cv::vconcat(topHalf, bottomHalf, finalImage);  // ���������� ���� � ���

    // ���������� ������������ � ������������ �����������
    cv::imshow("Original Image", image);
    cv::imshow("Processed Image", finalImage);

    // �������� ������� ������� ����� ��������� ����
    cv::waitKey(0);

    return 0;
}
