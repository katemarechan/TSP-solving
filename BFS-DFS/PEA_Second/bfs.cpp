#include "BFS.h"
#include <queue>
#include <limits>
#include <algorithm>

int calculate_lower_bound(const std::vector<std::vector<int>>& matrix,
                          const std::vector<int>& path,
                          const std::set<int>& visited);

void BFS(const std::vector<std::vector<int>>& matrix,
         std::vector<int>& best_path,
         int& best_cost,
         std::vector<int>& path,
         std::set<int>& visited,
         int current_cost) {
    // Pobieramy rozmiar macierzy, czyli liczbę miast
    int n = matrix.size();

    // Inicjalizujemy kolejkę BFS
    // Każdy element w kolejce to para składająca się z:
    // 1) Ścieżki (vector<int>) i zestawu odwiedzonych miast (set<int>)
    // 2) Aktualnego kosztu (int)
    std::queue<std::pair<std::pair<std::vector<int>, std::set<int>>, int>> q;

    // Dodajemy początkowy stan do kolejki: pustą ścieżkę, zestaw odwiedzonych miast i początkowy koszt
    q.push({{path, visited}, current_cost});

    // Rozpoczynamy przetwarzanie kolejki
    while (!q.empty()) {
        // Pobieramy pierwszy element z kolejki (FIFO) i usuwamy go
        auto current = q.front();
        q.pop();

        // Rozpakowujemy stan z elementu kolejki
        std::vector<int> current_path = current.first.first;  // Obecna ścieżka
        std::set<int> current_visited = current.first.second; // Zestaw odwiedzonych miast
        int current_cost = current.second;                     // Koszt dotychczasowej ścieżki

        // Sprawdzamy, czy ścieżka zawiera wszystkie miasta (pełny cykl)
        if (current_path.size() == n) {
            // Jeśli tak, dodajemy koszt powrotu do miasta początkowego
            current_cost += matrix[current_path.back()][current_path[0]];

            // Jeśli ten koszt jest mniejszy niż najlepszy dotychczasowy koszt
            if (current_cost < best_cost) {
                // Aktualizujemy najlepszy koszt i najlepszą ścieżkę
                best_cost = current_cost;
                best_path = current_path;
            }
            // Kończymy przetwarzanie tego stanu, bo pełna ścieżka została znaleziona
            continue;
        }

        // Jeśli ścieżka nie zawiera wszystkich miast, rozważamy kolejne możliwe miasta
        for (int next_city = 0; next_city < n; ++next_city) {
            // Sprawdzamy, czy miasto nie zostało jeszcze odwiedzone
            if (current_visited.find(next_city) == current_visited.end()) {
                // Obliczamy nowy koszt, biorąc pod uwagę dodanie miasta do ścieżki
                int new_cost = current_cost + matrix[current_path.back()][next_city];

                // Tworzymy nową ścieżkę, dodając kolejne miasto
                std::vector<int> new_path = current_path;
                new_path.push_back(next_city);

                // Tworzymy nowy zestaw odwiedzonych miast, dodając nowe miasto
                std::set<int> new_visited = current_visited;
                new_visited.insert(next_city);

                // Obliczamy dolną granicę kosztu dla tej nowej ścieżki
                // Funkcja `calculate_lower_bound` szacuje minimalny koszt pozostałych krawędzi
                int lower_bound = calculate_lower_bound(matrix, new_path, new_visited);

                // Sprawdzamy, czy nowy koszt + dolna granica jest mniejszy niż dotychczasowy najlepszy koszt
                if (new_cost + lower_bound < best_cost) {
                    // Jeśli tak, dodajemy nowy stan do kolejki, aby go dalej przetwarzać
                    q.push({{new_path, new_visited}, new_cost});
                }
            }
        }
    }
}

