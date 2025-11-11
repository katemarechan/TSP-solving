#ifndef BFS_H
#define BFS_H

#include <vector>
#include <set>

void BFS(const std::vector<std::vector<int>>& matrix,
         std::vector<int>& best_path,
         int& best_cost,
         std::vector<int>& path,
         std::set<int>& visited,
         int current_cost);

#endif // BFS_H
