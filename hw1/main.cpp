

#include <iostream>
#include "graph_operations.h" // Include your Graph class definition

int main() {
  // 1. Create a Graph object
  Graph my_graph;
  std::cout << "Graph created." << std::endl;

  // 2. Add some edges to form a simple graph
  // Let's make a graph like: 1 -> 2, 1 -> 3, 2 -> 3, 3 -> 4
  std::cout << "\nAdding edges: 1->2, 1->3, 2->3, 3->4" << std::endl;
  my_graph.add_edge(1, 2);
  my_graph.add_edge(1, 3);
  my_graph.add_edge(2, 3);
  my_graph.add_edge(3, 4);

  // 3. Print the graph to see its current state
  std::cout << "\nCurrent graph structure:" << std::endl;
  my_graph.print_graph();

  // 4. Test the delete_edge function
  std::cout << "\nDeleting edge 1 -> 3..." << std::endl;
  my_graph.delete_edge(1, 3);

  // 5. Print the graph again to verify the edge was removed
  std::cout << "\nGraph structure after deleting edge:" << std::endl;
  my_graph.print_graph();

  // 6. Test deleting a non-existent edge
  std::cout << "\nAttempting to delete non-existent edge 1 -> 4..." << std::endl;
  my_graph.delete_edge(1, 4);
  std::cout << "\nGraph structure (should be unchanged):" << std::endl;
  my_graph.print_graph();

  std::vector<int> visitlist;
  vector<list<int>> res;
  res=my_graph.connected_components();
  for(auto i :res){
    for(auto j :i){
      cout<<j;
    }
  }
  return 0;

}