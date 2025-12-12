//
// Created by lennard on 2025-11-23.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono> // For timing
#include <iomanip>

using namespace std;

class LargestZeroSquare {
 private:
  int M; // Rows
  int N; // Cols

  vector<vector<uint8_t>> matrix;

 public:
  LargestZeroSquare() {}

  // Setup method: Generates a random boolean matrix
  void setup(int rows, int cols) {
    M = rows;
    N = cols;
    matrix.assign(M, vector<uint8_t>(N));

    // Fill with random 0s and 1s
    // To make it interesting, let's say 50% chance of being 0.
    for (int i = 0; i < M; ++i) {
      for (int j = 0; j < N; ++j) {
        matrix[i][j] = rand() % 2;
      }
    }
  }

  void solve() {
    // 1. Define DP Table

    vector<vector<int>> dp(M, vector<int>(N, 0));

    int maxK = 0; // Max side length found
    int maxI = -1; // Bottom-right row index of the best square
    int maxJ = -1; // Bottom-right col index of the best square

    // 2. DP Loop (Iterative)
    for (int i = 0; i < M; ++i) {
      for (int j = 0; j < N; ++j) {
        // If the element is 1 (non-zero), it breaks the square of 0s.
        if (matrix[i][j] == 1) {
          dp[i][j] = 0;
        }
        else {
          // The element is 0.
          // Base case: First row or first column
          if (i == 0 || j == 0) {
            dp[i][j] = 1;
          } else {
            // Bellman-Ford Equation / Recurrence:
            // The size is limited by the smallest square formed by:
            // Top, Left, and Top-Left neighbors.
            dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
          }
        }

        // Update Global Maximum if we found a bigger square
        if (dp[i][j] > maxK) {
          maxK = dp[i][j];
          maxI = i;
          maxJ = j;
        }
      }
    }



    cout << "Max Square Size (k): " << maxK << " x " << maxK << endl;

    if (maxK > 0) {
      int startRow = maxI - maxK + 1;
      int startCol = maxJ - maxK + 1;
      cout << "Location (Top-Left): [" << startRow << ", " << startCol << "]" << endl;
      cout << "Location (Bottom-Right): [" << maxI << ", " << maxJ << "]" << endl;
    } else {
      cout << "No zero sub-matrix found." << endl;
    }
  }

  // Helper to estimate memory usage in bytes
  long long getMemoryUsage() {
    long long inputSize = M * N * sizeof(uint8_t);
    long long dpSize = M * N * sizeof(int);
    return inputSize + dpSize;
  }
};

int main() {
  srand(time(0));
  LargestZeroSquare solver;

  cout << "=== Problem 2: Largest Zero Sub-matrix Experiments ===" << endl;

  // Defined Experiment Datasets
  // Pairs of {rows, cols}
  vector<pair<int, int>> experiments = {
      {10, 10},
      {10, 100},
      {10, 1000},
      {100, 1000},
      {1000, 1000}
  };

  cout << left << setw(15) << "Dataset"
       << setw(15) << "Time(s)"
       << setw(20) << "Est Memory(KB)" << endl;
  cout << string(50, '-') << endl;

  for (auto& dim : experiments) {
    int r = dim.first;
    int c = dim.second;

    solver.setup(r, c);

    auto start = chrono::high_resolution_clock::now();


    cout << "\nRunning " << r << "x" << c << "..." << endl;
    solver.solve();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Output stats
    double memKB = solver.getMemoryUsage() / 1024.0;

    cout << ">>> RESULT: " << r << "x" << c << " | "
         << elapsed.count() << "s | "
         << memKB << " KB" << endl;
  }

  return 0;
}