#include "lc.h"

// Implementacja algorytmu Lowest Cost First Search
void lowest_cost_first_search(const std::vector<std::vector<int>>& matrix,
                              std::vector<int>& best_path,
                              int& best_cost,
                              std::vector<int>& path,
                              std::set<int>& visited,
                              int current_cost) {
    int n = matrix.size(); // Liczba miast w macierzy.

    // Kolejka priorytetowa do przechowywania stanów. Porównanie odbywa się według kosztu (od najmniejszego).
    std::priority_queue<std::pair<int, std::pair<std::vector<int>, std::set<int>>>,
            std::vector<std::pair<int, std::pair<std::vector<int>, std::set<int>>>>,
            std::greater<std::pair<int, std::pair<std::vector<int>, std::set<int>>>>> pq;

    // Dodanie początkowego stanu do kolejki (koszt = 0, ścieżka = startowa, odwiedzone = startowe miasto).
    pq.push({current_cost, {path, visited}});

    // Dopóki kolejka nie jest pusta, przetwarzamy stany.
    while (!pq.empty()) {
        // Pobranie stanu o najniższym koszcie z kolejki.
        auto current = pq.top();
        pq.pop();

        std::vector<int> current_path = current.second.first;   // Aktualna ścieżka.
        std::set<int> current_visited = current.second.second; // Zbiór odwiedzonych miast.
        int current_cost = current.first;                 // Aktualny koszt.

        // Jeśli odwiedzono wszystkie miasta:
        if (current_path.size() == n) {
            // Dodaj koszt powrotu do miasta początkowego, aby zamknąć cykl.
            current_cost += matrix[current_path.back()][current_path[0]];

            // Jeśli koszt cyklu jest lepszy niż obecny najlepszy koszt, aktualizujemy najlepszy wynik.
            if (current_cost < best_cost) {
                best_cost = current_cost;
                best_path = current_path;
            }
            continue; // Przetwarzamy kolejny stan.
        }

        // Rozważamy wszystkie sąsiednie miasta, które jeszcze nie zostały odwiedzone.
        for (int next_city = 0; next_city < n; ++next_city) {
            if (current_visited.find(next_city) == current_visited.end()) {
                // Obliczamy koszt przejścia do następnego miasta.
                int new_cost = current_cost + matrix[current_path.back()][next_city];

                // Tworzymy nową ścieżkę, dodając kolejne miasto.
                std::vector<int> new_path = current_path;
                new_path.push_back(next_city);

                // Aktualizujemy zbiór odwiedzonych miast.
                std::set<int> new_visited = current_visited;
                new_visited.insert(next_city);

                // Obliczamy dolną granicę kosztu dla tej ścieżki.
                int lower_bound = calculate_lower_bound(matrix, new_path, new_visited);

                // Jeśli koszt ścieżki z dolną granicą jest mniejszy od najlepszego kosztu, dodajemy stan do kolejki.
                if (new_cost + lower_bound < best_cost) {
                    pq.push({new_cost, {new_path, new_visited}});
                }
            }
        }
    }

    // Odwrócenie najlepszej ścieżki (pomijając pierwsze miasto) w celu poprawy kolejności.
    std::reverse(best_path.begin() + 1, best_path.end());
}
