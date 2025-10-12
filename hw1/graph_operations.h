//
// Created by lennard on 25-10-11.
//

#ifndef HW1__GRAPH_OPERATIONS_H_
#define HW1__GRAPH_OPERATIONS_H_
#include <vector>
#include "map"
using  namespace  std;
class Graph {
 public:
  Graph();
  void add_edge(int u, int v);
  void delete_edge(int u, int v);

 private:
  map<int, vector<int>> adj_list;
};

#endif //HW1__GRAPH_OPERATIONS_H_
