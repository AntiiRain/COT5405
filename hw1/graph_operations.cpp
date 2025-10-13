//
// Created by lennard on 25-10-11.
//

#include <iostream>
#include "graph_operations.h"
#include "queue"
#include "unordered_set"
#include "list"
#include <algorithm>
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

void Graph::print_graph() const {
  if (adj_list.empty()) {
    std::cout << "Graph is empty." << std::endl;
    return;
  }
  // The 'const' here means this function promises not to change the object
  for (auto const& [vertex, neighbors] : adj_list) {
    std::cout << "Vertex " << vertex << " -> [ ";
    for (int neighbor : neighbors) {
      std::cout << neighbor << " ";
    }
    std::cout << "]" << std::endl;
  }
}

vector<list<int>> Graph::connected_components(){
  vector<list<int>> all_components;
  unordered_set<int> visited;
  //Iteration this adj vertex
  for(auto &pair: adj_list){
    if(visited.count(pair.first)) continue;
    list<int> current_component{pair.first};
    visited.insert(pair.first);
    //go to recursion of this vertex
    dfs_helper_cc(pair.first, visited, current_component);

    all_components.push_back(current_component);
  }

  return all_components;
}


void Graph::dfs_helper_cc(int u, unordered_set<int>& visited, list<int>& current_component){
  for(auto vertex:adj_list[u]){
    //skip if visited
    if(visited.count(vertex)) continue;
    current_component.push_back((vertex));
    visited.insert((vertex));
    dfs_helper_cc(vertex,visited,current_component);
//    cout<< "this connected components end";
  }
}


list<int> Graph::one_cycle() {
  unordered_set<int> visited;
  list<int> result_cycle;
  //Iteration this adj vertex
  for(auto &pair: adj_list){
    int u = pair.first;
    //if vertex connect it self
    if(visited.count(pair.first)) continue;
    vector<int> current_path;

    if (dfs_helper_cycle(u, -1, visited, current_path, result_cycle)) {
      return result_cycle; // 找到环，立刻返回
    }
  }
  return {};
}


bool Graph::dfs_helper_cycle(int u, int p, unordered_set<int>& visited,
                             vector<int>& path, list<int>& result_cycle){
  visited.insert(u);
  path.push_back(u);
  for(int vertex:adj_list[u]){
    if (vertex == p) {
      continue;
    }

    auto it = find(path.begin(), path.end(), vertex);
    if (it != path.end()) {
      result_cycle.assign(it, path.end());
      result_cycle.push_back(vertex); // 闭合环
      if (result_cycle.size() >= 3) {
        result_cycle = result_cycle; // It's a valid cycle!
        return true;
      }
    }
    if (visited.find(vertex) == visited.end()) {
      if (dfs_helper_cycle(vertex, u, visited, path, result_cycle)) {
        return true; // 如果下层调用找到了环, 立刻向上传递 true
      }
    }
  }
  path.pop_back();
  return false;
}




vector<int> Graph::graph_BFS(int start_vertex){
  std::vector<int> res;
  std::map<int,bool> visited;
  //visit que
  std::queue<int> que;
  que.push(start_vertex);
  while(!que.empty()){
    //get first vertx form visit que
    int current = que.front();
    que.pop();
    //make it visted
    visited[current] = true;

    res.push_back(current);
    //put his neighbor to visit que
    for(int neighbor :adj_list[current]){
      //push vertx if it haven't been visit
      if(visited.find(neighbor) == visited.end()){
        que.push(neighbor);
      }
    }
  }
  return res;
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