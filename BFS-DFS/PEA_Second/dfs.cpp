#include "dfs.h"


// Algorytm przeszukiwania w głąb (DFS) dla problemu TSP
void DFS(const std::vector<std::vector<int>>& matrix,
         std::vector<int>& best_path,
         int& best_cost,
         std::vector<int>& path,
         std::set<int>& visited,
         int current_cost) {
    // Pobieramy rozmiar macierzy, czyli liczbę miast
    int n = matrix.size();

    // Sprawdzamy, czy odwiedzono wszystkie miasta (pełny cykl)
    if (path.size() == n) {
        // Jeśli tak, dodajemy koszt powrotu do miasta początkowego
        current_cost += matrix[path.back()][path[0]];

        // Jeśli ten koszt jest mniejszy niż najlepszy dotychczasowy koszt
        if (current_cost < best_cost) {
            // Aktualizujemy najlepszy koszt i najlepszą ścieżkę
            best_cost = current_cost;
            best_path = path; // Zapisujemy nową najlepszą ścieżkę
        }
        // Zakończenie przetwarzania tej gałęzi (ścieżka pełna)
        return;
    }

    // Iterujemy przez wszystkie możliwe miasta, które mogą być kolejnym przystankiem
    for (int next_city = 0; next_city < n; ++next_city) {
        // Sprawdzamy, czy miasto nie zostało jeszcze odwiedzone
        if (visited.find(next_city) == visited.end()) {
            // Obliczamy nowy koszt, biorąc pod uwagę dodanie miasta do ścieżki
            int new_cost = current_cost + matrix[path.back()][next_city];

            // Dodajemy miasto do ścieżki
            path.push_back(next_city);
            visited.insert(next_city); // Oznaczamy miasto jako odwiedzone

            // Obliczamy dolną granicę kosztu dla tej nowej ścieżki
            // Funkcja `calculate_lower_bound` szacuje minimalny koszt pozostałych krawędzi
            int lower_bound = calculate_lower_bound(matrix, path, visited);

            // Sprawdzamy, czy warto kontynuować eksplorację tej gałęzi
            if (new_cost + lower_bound < best_cost) {
                // Rekurencyjnie wywołujemy DFS dla kolejnych miast
                DFS(matrix, best_path, best_cost, path, visited, new_cost);
            }

            // Cofamy zmiany, by umożliwić eksplorację innych gałęzi
            visited.erase(next_city); // Usuwamy miasto z odwiedzonych
            path.pop_back(); // Usuwamy miasto z bieżącej ścieżki
        }
    }
}


int calculate_lower_bound(const std::vector<std::vector<int>>& matrix,
                          const std::vector<int>& path,
                          const std::set<int>& visited);
