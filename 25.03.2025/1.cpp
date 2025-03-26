#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

int main() {
    // Загрузка изображений
    cv::Mat img1 = cv::imread("image1.png", cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread("image2.png", cv::IMREAD_COLOR);
    if (img1.empty() || img2.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображения!" << std::endl;
        return -1;
    }

    // Обнаружение ключевых точек и вычисление дескрипторов с ORB
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    // Сопоставление дескрипторов с использованием BFMatcher
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> knnMatches;
    matcher.knnMatch(descriptors1, descriptors2, knnMatches, 2);

    // Применение Ratio Test для фильтрации совпадений 
    const float ratio_thresh = 0.75f;
    std::vector<cv::DMatch> goodMatches;
    for (const auto& match : knnMatches) {
        if (match[0].distance < ratio_thresh * match[1].distance) {
            goodMatches.push_back(match[0]);
        }
    }

    // Ищем матрицу гомографии с использованием метода RANSAC
    std::vector<cv::Point2f> points1, points2;
    for (const auto& match : goodMatches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }

    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC);  // Матрица гомографии, RANSAC отфильтровывет выбросы 
    std::cout << "Матрица гомографии:\n" << H << std::endl;

    // Визуализация соответствий
    cv::Mat imgMatches;
    cv::drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, imgMatches,
        cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    cv::imshow("Совпадения ключевых точек", imgMatches);

    // Трансформация второго изображения
    cv::Mat result;
    cv::warpPerspective(img2, result, H, cv::Size(img1.cols * 2, img1.rows));
    img1.copyTo(result(cv::Rect(0, 0, img1.cols, img1.rows)));   // Объединение изображений

    // Визуализация
    cv::imshow("Сшитая панорама", result);
    cv::waitKey(0);
    return 0;
}
