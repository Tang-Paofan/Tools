#include <iostream>
#include <Eigen/Dense>
#include <chrono>

Eigen::Vector4d PlaneFitting(const std::vector<Eigen::Vector3d> &plane_pts)
{
    Eigen::Vector3d center = Eigen::Vector3d::Zero();
    for (const auto &pt : plane_pts)
    {
        center += pt / plane_pts.size();
    }

    Eigen::MatrixXd A(plane_pts.size(), 3);
    for (int i = 0; i < plane_pts.size(); i++)
    {
        A(i, 0) = plane_pts[i][0] - center[0];
        A(i, 1) = plane_pts[i][1] - center[1];
        A(i, 2) = plane_pts[i][2] - center[2];
    }

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinV);
    auto a = svd.matrixV()(0, 2);
    auto b = svd.matrixV()(1, 2);
    auto c = svd.matrixV()(2, 2);
    auto d = -(a * center[0] + b * center[1] + c * center[2]);
    return Eigen::Vector4d(a, b, c, d);
}

int main()
{
    std::vector<Eigen::Vector3d> points;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        float x, y, z; // 在0-1内取随机数
        x = double(rand()) / RAND_MAX * 10;
        y = double(rand()) / RAND_MAX * 10;
        z = double(rand()) / RAND_MAX * 10;
        points.emplace_back(x, y, z);
    }

    auto t_begin = std::chrono::steady_clock::now();
    auto plane = PlaneFitting(points);
    auto t_end = std::chrono::steady_clock::now();
    auto t_count = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_begin).count();
    std::cout << plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2];
}