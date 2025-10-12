//
// Created by lennard on 25-10-11.
//

#include "graph_operations.h"
Graph::Graph() {

}
void Graph::add_edge(int u, int v) {
  adj_list[u].push_back(v);
}

void Graph::delete_edge(int u, int v) {
  for(int i = 0; i< adj_list[u].size(); i++){
    if(adj_list[u][i] == v){
      adj_list[u].erase(adj_list[u].begin() + i);
      break;
    }
  }

}

//  auto it = adj_list.find(u);
//
//  // Only proceed if vertex u exists
//  if (it != adj_list.end()) {
//    // 'it->second' is the vector of neighbors
//    vector<int>& neighbors = it->second;
//    auto new_end = std::remove(neighbors.begin(), neighbors.end(), v);
//
//    // Erase the elements from the new logical end to the actual end.
//    neighbors.erase(new_end, neighbors.end());
//  }
//}