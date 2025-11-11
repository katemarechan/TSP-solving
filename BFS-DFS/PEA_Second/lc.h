//
// Created by zart0 on 28/11/2024.
//

#ifndef PEA_BETTER_LC_H
#define PEA_BETTER_LC_H

#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <functional>
#include <limits>

// Deklaracja funkcji dolnej granicy kosztu
int calculate_lower_bound(const std::vector<std::vector<int>>& matrix,
                          const std::vector<int>& path,
                          const std::set<int>& visited);

// Deklaracja funkcji Lowest Cost Algorithm
void lowest_cost_first_search(const std::vector<std::vector<int>>& matrix,
                              std::vector<int>& best_path,
                              int& best_cost,
                              std::vector<int>& path,
                              std::set<int>& visited,
                              int current_cost);


#endif //PEA_BETTER_LC_H
