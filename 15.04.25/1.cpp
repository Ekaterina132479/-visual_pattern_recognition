#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Загружаем каскады Хаара для распознавания лиц, глаз и улыбок
    cv::CascadeClassifier face_cascade, eyes_cascade, smile_cascade;
    if (!face_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml") ||
        !eyes_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_eye.xml") ||
        !smile_cascade.load("C:/opencv/build/etc/haarcascades/haarcascade_smile.xml")) {
        std::cerr << "Ошибка: не удалось загрузить один из классификаторов!" << std::endl;
        return -1;
    }

    // Открываем видеофайл
    cv::VideoCapture cap("ZUA.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Ошибка: не удалось открыть видео!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        // Переводим кадр в оттенки серого и улучшаем контраст
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        // Обнаружение лиц (синий прямоугольник)
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, cv::Size(80, 80));

        // Для каждого найденного лица
        for (const auto& face : faces) {
            // Отрисовка прямоугольника вокруг лица
            cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);

            // Область лица для последующего анализа глаз и улыбки
            cv::Mat faceROI = gray(face);

            // Обнаружение глаз в пределах лица (зелёный круг)
            std::vector<cv::Rect> eyes;
            eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 10, 0, cv::Size(40, 40));
            for (const auto& eye : eyes) {
                cv::Point center(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2);
                cv::circle(frame, center, eye.width / 2, cv::Scalar(0, 255, 0), 2);
            }

            // Обнаружение улыбки в пределах лица (жёлтый прямоугольник)
            std::vector<cv::Rect> smiles;
            smile_cascade.detectMultiScale(faceROI, smiles, 1.5, 15, 0, cv::Size(30, 30));
            for (const auto& smile : smiles) {
                cv::Rect smileRect(face.x + smile.x, face.y + smile.y, smile.width, smile.height);
                cv::rectangle(frame, smileRect, cv::Scalar(0, 255, 255), 2); 
            }
        }

        // Отображение результата
        cv::imshow("Распознавание лица, глаз и улыбки", frame);

        // Прерывание по нажатию клавиши
        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
