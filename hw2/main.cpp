#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono> // For timing
#include <map>
#include <iomanip>

using namespace std;

class WeightedCommonSubstring {
private:
    double weights[26]; // Weights for A-Z
    double delta;       // Mismatch penalty
    string s1;
    string s2;


    void initEnglishFrequencies() {
        // Frequencies: A, B, C, ..., Z
        double freqs[] = {
            8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.2,
            0.8, 4.0, 2.4, 6.7, 7.5, 1.9, 0.1, 6.0, 6.3, 9.1,
            2.8, 1.0, 2.4, 0.2, 2.0, 0.1
        };
        for (int i = 0; i < 26; ++i) {
            weights[i] = freqs[i];
        }
    }

    // Helper: Initialize weights for Scenario 1 (all 1.0)
    void initUniformWeights() {
        for (int i = 0; i < 26; ++i) {
            weights[i] = 1.0;
        }
    }

public:
    WeightedCommonSubstring() {}

    // Setup method for different scenarios
    void setup(int scenario, double d_val, string str1, string str2) {
        s1 = str1;
        s2 = str2;
        delta = d_val;

        if (scenario == 1) {
            initUniformWeights();
        } else {
            initEnglishFrequencies();
        }
    }


    void solve() {
      int n = s1.length();
      int m = s2.length();

      // 1. Define DP Table

      vector<vector<double>> dp(n + 1, vector<double>(m + 1, 0.0));

      cout << "Running DP..." << endl;

      double maxScore = 0.0;
      int endIdx1 = 0; // 记录最大分数的结束位置 (s1中的索引 1-based)
      int endIdx2 = 0; // 记录最大分数的结束位置 (s2中的索引 1-based)

      // 2. DP Loop (Iterative, No Recursion)
      for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {

          double charWeight = weights[s1[i-1] - 'A'];

          double currentVal = 0.0;

          // Bellman-Ford Equation Logic for "No Gaps" (Weighted Substring)
          if (s1[i-1] == s2[j-1]) {
            // 匹配 (Match)
            currentVal = dp[i-1][j-1] + charWeight;
          } else {
            // 不匹配 (Mismatch) - 扣除 delta
            currentVal = dp[i-1][j-1] - delta;
          }

          // 如果分数变为负数，重置为 0 (Local Alignment Property)
          if (currentVal < 0) {
            dp[i][j] = 0.0;
          } else {
            dp[i][j] = currentVal;
          }

          // 更新全局最大值
          if (dp[i][j] > maxScore) {
            maxScore = dp[i][j];
            endIdx1 = i;
            endIdx2 = j;
          }
        }
      }

      cout << "Max Score Found: " << maxScore << endl;


      string extractedSubStr1 = "";
      string extractedSubStr2 = "";

      int currI = endIdx1;
      int currJ = endIdx2;


      while (currI > 0 && currJ > 0 && dp[currI][currJ] > 0) {
        extractedSubStr1 += s1[currI - 1];
        extractedSubStr2 += s2[currJ - 1];
        currI--;
        currJ--;
      }

      // 因为是向回遍历的，所以字符串是反的，需要反转回来
      reverse(extractedSubStr1.begin(), extractedSubStr1.end());
      reverse(extractedSubStr2.begin(), extractedSubStr2.end());

      cout << "Optimal Substring from S1: " << extractedSubStr1 << endl;
      cout << "Optimal Substring from S2: " << extractedSubStr2 << endl;
      cout << "------------------------------------------------" << endl;
    }

};

// ============================================================
// Experiment Drivers
// ============================================================

string generateRandomString(int length) {
    string s = "";
    for (int i = 0; i < length; ++i) {
        s += (char)('A' + rand() % 26);
    }
    return s;
}

int main() {
    srand(time(0));
    WeightedCommonSubstring solver;

    cout << "=== Problem 1: Weighted Approximate Common Substring ===" << endl;

    // --- SCENARIO 1 ---
    // w_l = 1, delta = 10
    cout << "\n--- Scenario 1 (Uniform Weights, Delta=10) ---" << endl;
    // Example input from assignment
    string s1_ex = "ABCAABCAA";
    string s2_ex = "ABBCAACCBBBBBB";

    solver.setup(1, 10.0, s1_ex, s2_ex);

    auto start = chrono::high_resolution_clock::now();
    solver.solve();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken: " << elapsed.count() << "s" << endl;


    // --- SCENARIO 2 ---
    // w_l = Frequency, Delta varies
    cout << "\n--- Scenario 2 (Frequency Weights, Varying Delta) ---" << endl;

    // Generate larger synthetic data for experiment
    string bigS1 = generateRandomString(1000);
    string bigS2 = generateRandomString(1000);

    // Weights range from ~0.1 to 12.7.
    // Delta should vary between smallest (0.1) and largest (12.7).
    // We need 10 intermediate values.
    double minDelta = 0.1;
    double maxDelta = 12.7;
    double step = (maxDelta - minDelta) / 9.0;

    for (int i = 0; i < 10; ++i) {
        double currentDelta = minDelta + (i * step);
        cout << "\nExperiment " << (i+1) << " with Delta = " << currentDelta << endl;

        solver.setup(2, currentDelta, bigS1, bigS2);

        start = chrono::high_resolution_clock::now();
        solver.solve();
        end = chrono::high_resolution_clock::now();
        elapsed = end - start; // Time in seconds

        // You can print memory usage here if you add a memory tracking utility,
        // or just estimate it as O(N*M) in your report.
    }

    return 0;
}