#include <iostream>
#include<vector>
using namespace std;

int partition(vector<int> &nums, int left, int right);

#include <iostream>
#include <vector>
#include <algorithm> // for std::sort, std::min
#include <cmath>     // for sqrt, std::abs
#include <cfloat>    // for DBL_MAX
#include <random>    // for data generation
#include <chrono>    // for timing
#include <iomanip>   // for std::setprecision, std::fixed

// 点 结构体
struct Point {
  double x, y;
};

// 计算两点之间的欧几里得距离
double dist(Point p1, Point p2) {
  return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) +
      (p1.y - p2.y) * (p1.y - p2.y));
}

// 暴力解法：用于 n <= 3 的基准情况
double bruteForce(const std::vector<Point>& points, int left, int right) {
  double minDist = DBL_MAX;
  for (int i = left; i <= right; ++i) {
    for (int j = i + 1; j <= right; ++j) {
      minDist = std::min(minDist, dist(points[i], points[j]));
    }
  }
  return minDist;
}

// --- 2. 核心分治算法 ---
/**
 * @brief 核心递归函数
 * @param Px_sorted 按 x 坐标排序的 *当前子集*
 * @param Py_sorted 按 y 坐标排序的 *同一子集*
 * @return 子集中的最小距离
 */

double closestUtil(const std::vector<Point>& Px_sorted, const std::vector<Point>& Py_sorted) {
  int n = Px_sorted.size();

  // 1. Base Case
  if (n <= 3) {
    // Px_sorted 包含了所有的点，所以我们直接对它使用暴力解法
    return bruteForce(Px_sorted, 0, n - 1);
  }

  // 2 Divide
  int mid = n / 2;
  Point midPoint = Px_sorted[mid - 1]; // 左侧子集的最后一个点
  double midX = midPoint.x;

  // 分割 Px (O(n) 时间 - 实际是 O(1) 构造 + O(n) 复制)
  std::vector<Point> Qx(Px_sorted.begin(), Px_sorted.begin() + mid);
  std::vector<Point> Rx(Px_sorted.begin() + mid, Px_sorted.end());

  // ** 关键细节 (1): 在 O(n) 时间内分割 Py **
  // 我们不能对 Qx 和 Rx 重新排序 (那会变成 O(n log n))
  // 我们必须遍历 Py_sorted，根据 midX 将点分配到 Qy 和 Ry
  std::vector<Point> Qy, Ry;
  Qy.reserve(mid + 1);
  Ry.reserve(n - mid + 1);

  for (const auto& p : Py_sorted) {
    if (p.x <= midX) {
      Qy.push_back(p);
    } else {
      Ry.push_back(p);
    }
  }

  // 3. [Conquer] 解决
  double deltaL = closestUtil(Qx, Qy);
  double deltaR = closestUtil(Rx, Ry);
  double delta = std::min(deltaL, deltaR);

  // 4. [Combine] 合并
  // ** 关键细节 (2): O(n) 时间的条带扫描 **

  // a. 创建条带 S (已按y排序)
  // 遍历 Py_sorted (而不是 Px_sorted)，这样 S 自动保持y排序
  std::vector<Point> S;
  S.reserve(n); // 最坏情况是所有点都在条带内
  for (const auto& p : Py_sorted) {
    if (std::abs(p.x - midX) < delta) {
      S.push_back(p);
    }
  }

  // b. 扫描条带 S
  double minDistStrip = delta;
  for (size_t i = 0; i < S.size(); ++i) {
    // ** 关键细节 (3): 内部循环只运行常数次 **
    // 我们只检查 S[j].y 在 S[i].y + delta 范围内的点
    for (size_t j = i + 1; j < S.size() && (S[j].y - S[i].y < minDistStrip); ++j) {
      minDistStrip = std::min(minDistStrip, dist(S[i], S[j]));
    }
    // 当 (S[j].y - S[i].y >= minDistStrip) 时，内循环自动停止
    // (minDistStrip 可能会在循环中变小)
  }

  return std::min(delta, minDistStrip);
}



/**
 * @brief 算法的主入口 (Wrapper)
 * @param points 原始的点集
 * @return 集合中的最小距离
 */
double findClosestPair(std::vector<Point>& points) {
  if (points.size() < 2) {
    return DBL_MAX;
  }
  // [预处理] O(n log n)
  // 创建 Px (按 x 排序) 和 Py (按 y 排序)
  std::vector<Point> Px = points;
  std::vector<Point> Py = points;

  std::sort(Px.begin(), Px.end(), [](Point a, Point b) {
    return a.x < b.x;
  });
  std::sort(Py.begin(), Py.end(), [](Point a, Point b) {
    return a.y < b.y;
  });

  // 调用递归函数
  return closestUtil(Px, Py);
}







// --- 3. 实验验证框架 ---

int main() {
  // 设置随机数生成器
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 10000.0);

  std::vector<int> n_values = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000};

  std::cout << std::fixed << std::setprecision(8);
  std::cout << "N\t\tTime (ms)\t\tN*log(N)\t\tTime / (N*logN)\n";
  std::cout << "----------------------------------------------------------------------------\n";

  for (int n : n_values) {
    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
      points[i] = {dis(gen), dis(gen)};
    }

    // 计时开始
    auto start = std::chrono::high_resolution_clock::now();

    double min_dist = findClosestPair(points); // 运行分治算法

    // 计时结束
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    double n_log_n = (n == 0) ? 0 : (n * std::log(n)); // 避免 log(0)
    double constant = (n_log_n == 0) ? 0 : (elapsed.count() / n_log_n);

    std::cout << n << "\t\t"
              << elapsed.count() << "\t\t"
              << n_log_n << "\t\t"
              << constant << "\n";

     if (n <= 1000) {
         double bf_dist = bruteForce(points, 0, n - 1);
         std::cout << "  -> D&C: " << min_dist << ", BruteForce: " << bf_dist << std::endl;
     }
  }
  return 0;
}

