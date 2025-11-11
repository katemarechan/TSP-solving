//
// Created by zart0 on 28/11/2024.
//

#ifndef PEA_BETTER_DFS_H
#define PEA_BETTER_DFS_H

#include <vector>
#include <set>

// Function prototypes
void DFS(const std::vector<std::vector<int>>& matrix,
         std::vector<int>& best_path,
         int& best_cost,
         std::vector<int>& path,
         std::set<int>& visited,
         int current_cost);

int calculate_lower_bound(const std::vector<std::vector<int>>& matrix,
                          const std::vector<int>& path,
                          const std::set<int>& visited);



#endif //PEA_BETTER_DFS_H
