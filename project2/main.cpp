#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono> // 用于计时
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int INF = 1e9;

// ==========================================
// Network Flow / Dinic Algorithm Implementation
// ==========================================

struct Edge {
  int to;
  int capacity;
  int flow;
  int reverse_edge;
};

class NurseScheduler {
 private:
  int num_nurses;
  int num_shifts;
  int source;
  int sink;
  int num_nodes;
  vector<vector<Edge>> adj;

 public:
  NurseScheduler(int n, int m) : num_nurses(n), num_shifts(m) {
    source = 0;
    sink = n + m + 1;
    num_nodes = n + m + 2;
    adj.resize(num_nodes);
  }

  void addEdge(int u, int v, int cap) {
    Edge forward = {v, cap, 0, (int)adj[v].size()};
    Edge backward = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(forward);
    adj[v].push_back(backward);
  }

  // 构建图
  void buildGraph(const vector<int>& nurse_caps, const vector<int>& shift_reqs, const vector<vector<int>>& availability) {
    // Source -> Nurses
    for (int i = 0; i < num_nurses; ++i) {
      addEdge(source, i + 1, nurse_caps[i]);
    }
    // Nurses -> Shifts
    for (int i = 0; i < num_nurses; ++i) {
      for (int j = 0; j < num_shifts; ++j) {
        if (availability[i][j] == 1) {
          addEdge(i + 1, num_nurses + 1 + j, 1);
        }
      }
    }
    // Shifts -> Sink
    for (int j = 0; j < num_shifts; ++j) {
      addEdge(num_nurses + 1 + j, sink, shift_reqs[j]);
    }
  }

  bool bfs(vector<int>& level) {
    fill(level.begin(), level.end(), -1);
    level[source] = 0;
    queue<int> q;
    q.push(source);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (const auto& e : adj[u]) {
        if (e.capacity - e.flow > 0 && level[e.to] == -1) {
          level[e.to] = level[u] + 1;
          q.push(e.to);
        }
      }
    }
    return level[sink] != -1;
  }

  int dfs(int u, int pushed, vector<int>& ptr, vector<int>& level) {
    if (pushed == 0 || u == sink) return pushed;
    for (int& cid = ptr[u]; cid < adj[u].size(); ++cid) {
      auto& e = adj[u][cid];
      if (level[u] + 1 != level[e.to] || e.capacity - e.flow == 0) continue;
      int tr = dfs(e.to, min(pushed, e.capacity - e.flow), ptr, level);
      if (tr == 0) continue;
      e.flow += tr;
      adj[e.to][e.reverse_edge].flow -= tr;
      return tr;
    }
    return 0;
  }

  int getMaxFlow() {
    int flow = 0;
    vector<int> level(num_nodes);
    while (bfs(level)) {
      vector<int> ptr(num_nodes, 0);
      while (int pushed = dfs(source, INF, ptr, level)) {
        flow += pushed;
      }
    }
    return flow;
  }
};

// ==========================================
// Test Utilities
// ==========================================

void runManualTest(string testName, int n, int m, vector<int> caps, vector<int> reqs, vector<vector<int>> avail, bool expectedResult) {
  cout << "Test Case: " << testName << "... ";
  NurseScheduler ns(n, m);
  ns.buildGraph(caps, reqs, avail);
  int maxFlow = ns.getMaxFlow();

  int totalDemand = 0;
  for(int r : reqs) totalDemand += r;

  bool result = (maxFlow == totalDemand);
  if (result == expectedResult) {
    cout << "[PASS] (Flow: " << maxFlow << "/" << totalDemand << ")" << endl;
  } else {
    cout << "[FAIL] (Expected " << expectedResult << ", Got " << result << ")" << endl;
  }
}

void runPerformanceExperiment() {
  cout << "\n=== Running Performance Analysis ===" << endl;
  cout << "N(Nurses),M(Shifts),TotalNodes,Time(microseconds)" << endl; // CSV Header

  // 随机数生成器
  random_device rd;
  mt19937 gen(rd());


    int n = k * 50;  // 50, 100, ..., 1000
    int m = k * 20;  // 20, 40, ..., 400  // 我们逐渐增大数据规模
    // Step: 每次增加 50 个护士和 20 个班次
    for (int k = 1; k <= 20; ++k) {

    // 生成随机数据
    vector<int> caps(n);
    vector<int> reqs(m);
    vector<vector<int>> avail(n, vector<int>(m));

    // 随机 Capacity (1-3)
    uniform_int_distribution<> cap_dist(1, 3);
    for(int& c : caps) c = cap_dist(gen);

    // 随机 Demand (1-2)
    uniform_int_distribution<> req_dist(1, 2);
    for(int& r : reqs) r = req_dist(gen);

    // 随机 Availability (50% 概率有空)
    uniform_int_distribution<> avail_dist(0, 1);
    for(int i=0; i<n; ++i)
      for(int j=0; j<m; ++j)
        avail[i][j] = avail_dist(gen);

    // --- 计时开始 ---
    auto start = high_resolution_clock::now();

    NurseScheduler ns(n, m);
    ns.buildGraph(caps, reqs, avail);
    ns.getMaxFlow();

    auto stop = high_resolution_clock::now();
    // --- 计时结束 ---

    auto duration = duration_cast<microseconds>(stop - start);
    int totalNodes = n + m + 2;

    // 输出 CSV 格式数据
    cout << n << "," << m << "," << totalNodes << "," << duration.count() << endl;
  }
  cout << "=== End of Experiment ===" << endl;
}

int main() {
  // 1. 运行手动测试用例 (验证正确性)
  cout << "=== Correctness Verification ===" << endl;

  // Case 1: 简单可行 (2 Nurse, 2 Shifts)
  // Nurse A(2 caps), B(2 caps). Shift 1(1 req), 2(1 req). All avail.
  runManualTest("Simple Feasible", 2, 2, {2, 2}, {1, 1}, {{1,1}, {1,1}}, true);

  // Case 2: 简单不可行 (需求过大)
  // Nurse A(1 cap). Shift 1(2 req).
  runManualTest("Demand > Capacity", 1, 1, {1}, {2}, {{1}}, false);

  // Case 3: 简单不可行 (可用性不匹配)
  // Nurse A(1 cap) only avail for Shift 1. Shift 2 needs 1 person.
  runManualTest("Availability Mismatch", 1, 2, {1}, {0, 1}, {{1, 0}}, false);

  // 2. 运行性能实验 (获取画图数据)
  runPerformanceExperiment();

  return 0;
}