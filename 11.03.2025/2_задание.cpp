#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

std::string detectShape(const std::vector<cv::Point>& contour) {
    double peri = arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.027 * peri, true);

    int vertices = approx.size();

    if (vertices == 3) return "Triangle";
    if (vertices == 4) {
        Rect rect = boundingRect(approx);
        double aspectRatio = (double)rect.width / rect.height;
        return (aspectRatio > 0.9 && aspectRatio < 1.1) ? "Square" : "Rectangle";
    }

    double area = contourArea(contour);
    double circularity = (4 * CV_PI * area) / (peri * peri);

    Rect boundingBox = boundingRect(contour);
    double aspectRatio = (double)boundingBox.width / boundingBox.height;

    if (vertices > 6 && circularity > 0.8 && aspectRatio > 0.5 && aspectRatio < 2.0)
        return "Circle";

    return "Unknown";
}

// Функция для выбора цвета в зависимости от фигуры
Scalar getColorByShape(const std::string& shape) {
    if (shape == "Triangle") return Scalar(0, 0, 0); // черный
    if (shape == "Rectangle") return Scalar(255, 0, 0); // Синий
    if (shape == "Square") return Scalar(0, 255, 0); // Зелёный
    if (shape == "Circle") return Scalar(255, 0, 255); // Фиолетовый
    return Scalar(0, 0, 255); // Красный для "Unknown"
}

int main() {
    VideoCapture cap("video.mp4");
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open video file!" << endl;
        return -1;
    }

    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);

    VideoWriter videoWriter("output.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(frame_width, frame_height));
    if (!videoWriter.isOpened()) {
        cerr << "Error: Cannot create output video file!" << endl;
        return -1;
    }

    while (true) {
        Mat frame, gray, blurred, thresh;
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurred, Size(5, 5), 0);
        threshold(blurred, thresh, 225, 255, THRESH_BINARY_INV); // Инверсия для отделения фигур

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double area = contourArea(contour);
            if (area < 5 || area > 0.5 * frame.rows * frame.cols) continue;

            string shape = detectShape(contour);
            Scalar color = getColorByShape(shape);

            drawContours(frame, vector<vector<Point>>{contour}, -1, color, 2);
            Moments m = moments(contour);
            if (m.m00 != 0) {
                int cx = (int)(m.m10 / m.m00);
                int cy = (int)(m.m01 / m.m00);
                putText(frame, shape, Point(cx, cy), FONT_HERSHEY_SIMPLEX, 0.5, color, 2);
            }
        }

        videoWriter.write(frame); // Запись кадра в файл
        imshow("Detected Shapes", frame);
        if (waitKey(30) == 27) break; // ESC для выхода
    }

    cap.release();
    videoWriter.release();
    destroyAllWindows();
    cout << "Video saved as output.mp4" << endl;
    return 0;
}
