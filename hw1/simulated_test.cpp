

#include <iostream>
#include "graph_operations.h" // Include your Graph class definition

#include <iostream>
#include "graph_operations.h"
#include "graph_simulator.h"
// 打印结果的辅助函数
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

int main() {
  cout << "--- Running Graph Algorithm Simulations ---" << endl;

  // 1. 测试 n-cycle 图 (一个连通分量)
  Graph g_cycle = create_n_cycle(5);
  cout << "\nTesting a 5-node cycle graph:" << endl;
  g_cycle.print_graph();
  vector<list<int>> components1 = g_cycle.connected_components();
  print_all_components("5-Node Cycle Test", components1);

  // 2. 测试一个包含多个连通分量的自定义图
  Graph g_multi;
  g_multi.add_edge(0, 1); g_multi.add_edge(1, 0);
  g_multi.add_edge(1, 2); g_multi.add_edge(2, 1);
  g_multi.add_edge(3, 4); g_multi.add_edge(4, 3);
  g_multi.add_edge(5, 5); // 孤立点
  cout << "\nTesting a multi-component graph:" << endl;
  g_multi.print_graph();
  vector<list<int>> components2 = g_multi.connected_components();
  print_all_components("Multi-Component Test", components2);

  // 3. 测试空图 (多个连通分量, 每个大小为1)
  Graph g_empty = create_empty_graph(4);
  cout << "\nTesting a 4-node empty graph:" << endl;
  g_empty.print_graph();
  vector<list<int>> components3 = g_empty.connected_components();
  print_all_components("Empty Graph Test", components3);

  return 0;
}



//int main() {
//  // 1. Create a Graph object
//  Graph my_graph;
//  std::cout << "Graph created." << std::endl;
//
//  // 2. Add some edges to form a simple graph
//  // Let's make a graph like: 1 -> 2, 1 -> 3, 2 -> 3, 3 -> 4
//  std::cout << "\nAdding edges: 1->2, 1->3, 2->3, 3->4" << std::endl;
//  my_graph.add_edge(1, 2);
//  my_graph.add_edge(1, 3);
//  my_graph.add_edge(2, 3);
//  my_graph.add_edge(3, 4);
//
//  // 3. Print the graph to see its current state
//  std::cout << "\nCurrent graph structure:" << std::endl;
//  my_graph.print_graph();
//
//  // 4. Test the delete_edge function
//  std::cout << "\nDeleting edge 1 -> 3..." << std::endl;
//  my_graph.delete_edge(1, 3);
//
//  // 5. Print the graph again to verify the edge was removed
//  std::cout << "\nGraph structure after deleting edge:" << std::endl;
//  my_graph.print_graph();
//
//  // 6. Test deleting a non-existent edge
//  std::cout << "\nAttempting to delete non-existent edge 1 -> 4..." << std::endl;
//  my_graph.delete_edge(1, 4);
//  std::cout << "\nGraph structure (should be unchanged):" << std::endl;
//  my_graph.print_graph();
//
//  std::vector<int> visitlist;
//  vector<list<int>> res;
//  res=my_graph.connected_components();
//  for(auto i :res){
//    for(auto j :i){
//      cout<<j;
//    }
//  }
//  return 0;
//
//}