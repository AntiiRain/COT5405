#include <iostream>
#include "graph_operations.h"
#include "graph_simulator.h"
#include <chrono> // 用于精确计时


// ----------------- HELPER FUNCTIONS -----------------

// Helper function to print connected components
void print_all_components(const string& title, const vector<list<int>>& components) {
  cout << "## " << title << " ##" << endl;
  cout << "Found " << components.size() << " connected components:" << endl;
  int i = 1;
  for (const auto& component : components) {
    cout << "  Component " << i++ << ": [ ";
    for (int node : component) {
      cout << node << " ";
    }
    cout << "]" << endl;
  }
  cout << "------------------------------------" << endl;
}

void print_paths(const string& title, const map<int, list<int>>& paths) {
  cout << "## " << title << " ##" << endl;
  for (const auto& pair : paths) {
    cout << "  Path to " << pair.first << ": [ ";
    for (int node : pair.second) {
      cout << node << " ";
    }
    cout << "]" << endl;
  }
  cout << "------------------------------------" << endl;
}



// NEW: Helper function to print the result of one_cycle()
void print_cycle(const string& title, const list<int>& cycle) {
  cout << "## " << title << " ##" << endl;
  if (cycle.empty()) {
    cout << "✅ Result: No cycle found." << endl;
  } else {
    cout << "✅ Result: Cycle found: [ ";
    for (int node : cycle) {
      cout << node << " ";
    }
    cout << "]" << endl;
  }
  cout << "------------------------------------" << endl;
}

void print_test_header(const string& title, int n) {
  cout << "\n--- " << title << " (n = " << n << ") ---" << endl;
}

/**
 * @brief 打印算法的运行时间
 * @param algo_name 算法名称
 * @param duration 持续时间 (毫秒)
 */
void print_timing(const string& algo_name, double duration_ms) {
  cout << "  - " << algo_name << " took: " << duration_ms << " ms" << endl;
}



// ----------------- MAIN FUNCTION -----------------

int main() {
//  cout << "--- Running connected_components() Simulations ---" << endl;
//
//  // 1. Create a graph with a simple cycle
//  Graph g_cycle = create_n_cycle(5);
//  cout << "\nTesting a 5-node cycle graph:" << endl;
//  g_cycle.print_graph();
//  vector<list<int>> components1 = g_cycle.connected_components();
//  print_all_components("5-Node Cycle Test", components1);
//
//  // 2. Create an acyclic graph (a forest) with multiple components
//  Graph g_multi_acyclic;
//  g_multi_acyclic.add_edge(0, 1); g_multi_acyclic.add_edge(1, 0);
//  g_multi_acyclic.add_edge(1, 2); g_multi_acyclic.add_edge(2, 1);
//  g_multi_acyclic.add_edge(3, 4); g_multi_acyclic.add_edge(4, 3);
//  g_multi_acyclic.add_edge(5, 5); // Isolated node
//  cout << "\nTesting a multi-component acyclic graph:" << endl;
//  g_multi_acyclic.print_graph();
//  vector<list<int>> components2 = g_multi_acyclic.connected_components();
//  print_all_components("Multi-Component Acyclic Test", components2);
//
//  // 3. Create an empty graph (acyclic)
//  Graph g_empty = create_empty_graph(4);
//  cout << "\nTesting a 4-node empty graph:" << endl;
//  g_empty.print_graph();
//  vector<list<int>> components3 = g_empty.connected_components();
//  print_all_components("Empty Graph Test", components3);
//
//
//  // =======================================================
//  //             ✅ NEW: one_cycle() Test Cases
//  // =======================================================
//  cout << "\n\n--- Running one_cycle() Simulations ---" << endl;
//
//  // Test A: Using the 5-node cycle graph (SHOULD find a cycle)
//  cout << "\n[Test A] Testing on the 5-node cycle graph..." << endl;
//  list<int> cycle_result1 = g_cycle.one_cycle();
//  print_cycle("5-Node Cycle Test", cycle_result1);
//
//  // Test B: Using the multi-component acyclic graph (should NOT find a cycle)
//  cout << "\n[Test B] Testing on the acyclic multi-component graph..." << endl;
//  list<int> cycle_result2 = g_multi_acyclic.one_cycle();
//  print_cycle("Acyclic Multi-Component Test", cycle_result2);
//
//  // Test C: A more complex graph with branches and a cycle (SHOULD find a cycle)
//  Graph g_complex_cycle;
//  g_complex_cycle.add_edge(0, 1); g_complex_cycle.add_edge(1, 0);
//  g_complex_cycle.add_edge(1, 2); g_complex_cycle.add_edge(2, 1);
//  g_complex_cycle.add_edge(2, 3); g_complex_cycle.add_edge(3, 2); // Dead-end branch
//  g_complex_cycle.add_edge(1, 4); g_complex_cycle.add_edge(4, 1);
//  g_complex_cycle.add_edge(4, 5); g_complex_cycle.add_edge(5, 4);
//  g_complex_cycle.add_edge(5, 0); g_complex_cycle.add_edge(0, 5); // This edge creates the cycle 0-1-4-5-0
//  cout << "\n[Test C] Testing on a complex graph with a cycle:" << endl;
//  g_complex_cycle.print_graph();
//  list<int> cycle_result3 = g_complex_cycle.one_cycle();
//  print_cycle("Complex Cycle Test", cycle_result3);
//
//  // Test D: Using the empty graph (should NOT find a cycle)
//  cout << "\n[Test D] Testing on the 4-node empty graph..." << endl;
//  list<int> cycle_result4 = g_empty.one_cycle();
//  print_cycle("Empty Graph Test", cycle_result4);
//
//
//  cout << "\n\n--- Running shortest_paths() Simulations ---" << endl;
//
//  Graph g;
//  g.add_edge(0, 1); g.add_edge(1, 0);
//  g.add_edge(1, 2); g.add_edge(2, 1);
//  g.add_edge(0, 3); g.add_edge(3, 0);
//  g.add_edge(3, 2); g.add_edge(2, 3);
//  g.add_edge(2, 4); g.add_edge(4, 2);
//  g.add_edge(5, 6); g.add_edge(6, 5); // Unreachable component
//
//  cout << "\nTesting shortest paths from source 0:" << endl;
//  g.print_graph();
//  map<int, list<int>> paths = g.shortest_paths(0);
//  print_paths("Shortest Paths from 0", paths);
//
//  /* 预期输出:
//  ## Shortest Paths from 0 ##
//    Path to 0: [ 0 ]
//    Path to 1: [ 1 0 ]
//    Path to 2: [ 2 1 0 ]  (或 [ 2 3 0 ]，取决于遍历顺序, 但路径长度必须是3)
//    Path to 3: [ 3 0 ]
//    Path to 4: [ 4 2 1 0 ] (或 [ 4 2 3 0 ])
//    (不应包含 5 和 6 的路径)
//  */

  vector<int> sizes = {10, 100, 1000, 5000};
  // 对于完全图，规模不能太大，否则边数会爆炸，运行时间极长
  vector<int> complete_graph_sizes = {10, 50, 100, 500, 1000};

  cout << "========================================================" << endl;
  cout << "       Start systematic performance testing of graph algorithms" << endl;
  cout << "========================================================" << endl;

  // -----------------------------------------------------------------
  // 实验一：n-cycle (n元环图)
  // -----------------------------------------------------------------
  cout << "\n\n******************** experiment 1: N-Cycle Graph ********************" << endl;
  for (int n : sizes) {

    print_test_header("N-Cycle Graph", n);
    Graph g = create_n_cycle(n);

    // 1. 测试 connected_components
    auto start = chrono::high_resolution_clock::now();
    vector<list<int>> components = g.connected_components();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_ms = end - start;
    print_timing("connected_components()", duration_ms.count());

    // 2. 测试 one_cycle
    start = chrono::high_resolution_clock::now();
    list<int> cycle = g.one_cycle();
    end = chrono::high_resolution_clock::now();
    duration_ms = end - start;
    print_timing("one_cycle()", duration_ms.count());

    // 3. 测试 shortest_paths (从节点 0 开始)
    start = chrono::high_resolution_clock::now();
    map<int, list<int>> paths = g.shortest_paths(0);
    end = chrono::high_resolution_clock::now();
    duration_ms = end - start;
    print_timing("shortest_paths(0)", duration_ms.count());
//    if(n == 5000)    cin.get();
  }

  // -----------------------------------------------------------------
  // 实验二：Complete Graph (完全图)
  // -----------------------------------------------------------------
  cout << "\n\n******************** experiment 2: Complete Graph ********************" << endl;
  for (int n : complete_graph_sizes) {
    print_test_header("Complete Graph", n);
    Graph g = create_complete_graph(n);

    // 1. 测试 connected_components
    auto start = chrono::high_resolution_clock::now();
    g.connected_components();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_ms = end - start;
    print_timing("connected_components()", duration_ms.count());

    // 2. 测试 one_cycle
    start = chrono::high_resolution_clock::now();
    g.one_cycle();
    end = chrono::high_resolution_clock::now();
    duration_ms = end - start;
    print_timing("one_cycle()", duration_ms.count());

    // 3. 测试 shortest_paths (从节点 0 开始)
    start = chrono::high_resolution_clock::now();
    g.shortest_paths(0);
    end = chrono::high_resolution_clock::now();
    duration_ms = end - start;
    print_timing("shortest_paths(0)", duration_ms.count());
//    if(n == 1000)    cin.get();
  }


  // -----------------------------------------------------------------
  // 实验三：Empty Graph (空图)
  // -----------------------------------------------------------------
  cout << "\n\n******************** experiment 3: Empty Graph ********************" << endl;
  for (int n : sizes) {
    print_test_header("Empty Graph", n);
    Graph g = create_empty_graph(n);

    // 1. 测试 connected_components
    auto start = chrono::high_resolution_clock::now();
    g.connected_components();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_ms = end - start;
    print_timing("connected_components()", duration_ms.count());

    // 2. 测试 one_cycle
    start = chrono::high_resolution_clock::now();
    g.one_cycle();
    end = chrono::high_resolution_clock::now();
    duration_ms = end - start;
    print_timing("one_cycle()", duration_ms.count());

    // 3. 测试 shortest_paths (从节点 0 开始)
    start = chrono::high_resolution_clock::now();
    g.shortest_paths(0);
    end = chrono::high_resolution_clock::now();
    duration_ms = end - start;
    print_timing("shortest_paths(0)", duration_ms.count());
//    if(n == 5000)    cin.get();
  }

  cout << "\n\n========================================================" << endl;
  cout << "             performance testing end" << endl;
  cout << "========================================================" << endl;


  cout << "\n--- 准备进行峰值内存测量 ---" << endl;
  cout << "正在创建用于内存测试的最大图 (n=1000 的完全图)..." << endl;

  // 重新创建你的程序将处理的最大的图。
  // 对于大多数情况, 完全图会比稀疏图占用更多内存。
  Graph g_for_memory_test = create_complete_graph(1000);

  cout << "\n>>> 最大图已创建，程序现在暂停。" << endl;
  cout << ">>> 请打开任务管理器, 找到本程序 (例如 hw1.exe) 并记录其内存占用。" << endl;
  cout << ">>> 记录数值后, 在此窗口按 Enter 键即可退出程序。" << endl;

  // 这行代码会等待用户按 Enter 键
//  cin.get();


  return 0;
}