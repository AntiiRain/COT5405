//
// Created by lennard on 25-10-11.
//

#ifndef HW1__GRAPH_OPERATIONS_H_
#define HW1__GRAPH_OPERATIONS_H_
#include <vector>
#include "map"
#include "list"
#include "unordered_set"

using  namespace  std;
class Graph {
 public:
  Graph();

  void add_edge(int u, int v);
  void delete_edge(int u, int v);
  void print_graph() const;
  vector<int> graph_BFS(int start_vertex);
  vector<list<int>>  connected_components();
  list<int> one_cycle();
  map<int, list<int>> shortest_paths(int source);

 private:
  map<int, vector<int>> adj_list;
  void dfs_helper_cc(int u, unordered_set<int>& visted, list<int>& current_component);
  bool dfs_helper_cycle(int u, int p, unordered_set<int>& visited,
                        vector<int>& path, list<int>& result_cycle);

};

#endif //HW1__GRAPH_OPERATIONS_H_
