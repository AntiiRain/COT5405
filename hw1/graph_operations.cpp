//
// Created by lennard on 25-10-11.
//

#include <iostream>
#include "graph_operations.h"
#include "queue"
#include "unordered_set"
#include "list"
#include <algorithm>
#include <limits>

//distance vertex
using Edge = std::pair<int, int>;
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



map<int, list<int>> Graph::shortest_paths(int source) {
  map<int, int> distances;         // 存储从 source 到各点的最短距离
  map<int, int> predecessor;       // 存储各点在最短路径上的前一个节点
  map<int, list<int>> result_paths; // 最终返回的路径图

  // 1. 初始化
  for (const auto& pair : adj_list) {
    distances[pair.first] = numeric_limits<int>::max(); // 所有距离设为无穷大
  }
  distances[source] = 0; // 源点到自己的距离为 0

  // 优先队列，存储 {距离, 顶点}，并按距离从小到大排序
  priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
  pq.push({0, source});

  // 2. Dijkstra 主循环
  while (!pq.empty()) {
    int u = pq.top().second;
    int dist_u = pq.top().first;
    pq.pop();

    // 如果我们已经找到了到 u 的一条更短路径，就跳过这个旧的条目
    if (dist_u > distances[u]) {
      continue;
    }

    // 遍历 u 的所有邻居
    if (adj_list.count(u)) {
      for (int v : adj_list.at(u)) {
        // 权重为 1
        if (distances[u] + 1 < distances[v]) {
          distances[v] = distances[u] + 1;
          predecessor[v] = u; // 记录 v 的前驱是 u
          pq.push({distances[v], v});
        }
      }
    }
  }

  // 3. 重建路径
  // 遍历所有节点，如果节点可达，则从后往前构建路径
  for (const auto& pair : distances) {
    int v = pair.first;
    int dist = pair.second;

    if (dist != numeric_limits<int>::max()) { // 如果节点 v 是可达的
      list<int> path;
      int current = v;
      while (predecessor.count(current)) {
        path.push_front(current);
        current = predecessor[current];
      }
      path.push_front(source); // 最后加入源点

      // 作业要求是 v -> source 的路径，我们构建的是 source -> v 的路径
      // 但作业又说 "sp[v] is the list of vertices on a path from v back to source"
      // 这意味着从 v 开始，所以我们的构建方式是对的。
      // 让我们再仔细读一下 "A path in our case consists of a list of vertices and should include both v and the source."
      // "sp[v] is the list of vertices on a path from v back to source" -> [v, p1, p2, ..., source]

      // 让我们按要求重建路径
      result_paths[v].clear();
      current = v;
      while (predecessor.count(current)) {
        result_paths[v].push_back(current);
        current = predecessor[current];
      }
      result_paths[v].push_back(source);
    }
  }
  // 特殊处理源点本身
  result_paths[source] = {source};

  return result_paths;
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
                             vector<int>& path, list<int>& result_cycle) {
  visited.insert(u);
  path.push_back(u);

  if (adj_list.count(u)) {
    for (int v : adj_list.at(u)) {
      if (v == p) {
        continue;
      }

      auto it = find(path.begin(), path.end(), v);
      if (it != path.end()) {
        // Found a potential cycle. Reconstruct it first.
        list<int> potential_cycle;
        potential_cycle.assign(it, path.end());
        potential_cycle.push_back(v);

        if (potential_cycle.size() >= 3) {
          result_cycle = potential_cycle; // It's a valid cycle!
          return true;
        }
        // If not, it's a self-loop or 2-node loop. Ignore it and continue.
      }

      if (visited.find(v) == visited.end()) {
        if (dfs_helper_cycle(v, u, visited, path, result_cycle)) {
          return true;
        }
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