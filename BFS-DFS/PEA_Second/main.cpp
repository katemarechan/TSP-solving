#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <utility>
#include <limits.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cmath>
#include <chrono>
#include "bfs.h"
#include "dfs.h"
#include "lc.h"
#include "file_reader.h"

using namespace std;

const string CONFIGURATION_FILE = "config.txt";

double PCFreq = 0;          // Częstotliwość zegara
__int64 counterStart = 0.0; // Początek licznika

void StartCounter() // Funkcja rozpoczynająca pomiar czasu
{
    LARGE_INTEGER freq;
    if (!QueryPerformanceFrequency(&freq))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(freq.QuadPart) / 1000.0;

    QueryPerformanceCounter(&freq);
    counterStart = freq.QuadPart;
}


double GetCounter()
{
    LARGE_INTEGER freq;
    QueryPerformanceCounter(&freq);
    return double(freq.QuadPart - counterStart) / PCFreq;
}

// Obliczanie dolnej granicy kosztu w algorytmie
int calculate_lower_bound(const vector<vector<int>>& matrix, const vector<int>& path, const set<int>& visited) {
    int n = matrix.size(); // Liczba miast (rozmiar macierzy kosztów).
    int lower_bound = 0;   // Dolna granica kosztu, początkowo ustawiona na 0.

    // Iteracja przez każde miasto, aby znaleźć minimalny koszt krawędzi wychodzących.
    for (int i = 0; i < n; ++i) {
        // Sprawdź, czy miasto 'i' nie zostało jeszcze odwiedzone.
        if (visited.find(i) == visited.end()) {
            int min_edge = INT_MAX; // Minimalny koszt krawędzi dla miasta 'i', początkowo ustawiony na maksymalną wartość.

            // Przejrzyj wszystkie możliwe krawędzie wychodzące z miasta 'i'.
            for (int j = 0; j < n; ++j) {
                // Sprawdź, czy miasto 'j' nie zostało odwiedzone i jest różne od 'i'.
                if (i != j && visited.find(j) == visited.end()) {
                    // Znajdź minimalny koszt krawędzi z 'i' do 'j'.
                    min_edge = min(min_edge, matrix[i][j]);
                }
            }

            // Dodaj minimalny koszt krawędzi z miasta 'i' do dolnej granicy.
            lower_bound += min_edge;
        }
    }

    // Zwróć obliczoną dolną granicę kosztu.
    return lower_bound;
}


pair<int, vector<int>> branch_and_bound_tsp(const vector<vector<int>>& matrix, string search_method, double &time) {
    int n = matrix.size(); // Liczba miast w problemie.
    int best_cost = INT_MAX; // Inicjalizacja najlepszego kosztu jako maksymalnej możliwej wartości.
    vector<int> best_path;   // Wektor przechowujący najlepszą ścieżkę.

    // Inicjalizacja początkowego stanu: startujemy z miasta 0.
    vector<int> path = {0};      // Aktualna ścieżka zawierająca tylko miasto początkowe.
    set<int> visited = {0};     // Zbiór odwiedzonych miast, zawiera na razie tylko miasto początkowe.
    int current_cost = 0;       // Aktualny koszt ścieżki.

    StartCounter(); // Rozpoczęcie pomiaru czasu.

    // Wybór odpowiedniej metody przeszukiwania na podstawie parametru 'search_method'.
    if (search_method == "DFS") {
        // Wywołanie funkcji przeszukiwania w głąb (DFS) dla problemu TSP.
        DFS(matrix, best_path, best_cost, path, visited, current_cost);
    } else if (search_method == "BFS") {
        // Wywołanie funkcji przeszukiwania wszerz (BFS) dla problemu TSP.
        //BFS(matrix, best_path, best_cost, path, visited, current_cost);
    } else if (search_method == "LC") {
        // Wywołanie funkcji przeszukiwania metodą najniższego kosztu (Lowest Cost First).
        lowest_cost_first_search(matrix, best_path, best_cost, path, visited, current_cost);
    }

    // Rejestracja czasu wykonania algorytmu.
    time = GetCounter();

    // Zwrócenie wyniku w postaci pary (najlepszy koszt, najlepsza ścieżka).
    return {best_cost, best_path};
}

void printAdjacencyMatrix(const vector<vector<int>> &adjMatrix)
{
    for (const auto &row : adjMatrix)
    {
        for (int weight : row)
        {
            cout << weight << " ";
        }
        cout << endl;
    }
}

void saveResults(string outputFileName, string type, string algorithm, vector<int> bestRoute, int minCost, double time)
{
    ofstream outputFile(outputFileName);
    if (outputFile.is_open())
    {
        outputFile << "Type, Algorithm Used, Best Tour, Minimum Distance, Time Of Execution\n";
        outputFile << type << ", ";
        outputFile << algorithm << ", ";
        for (int city : bestRoute)
        {
            outputFile << city << " ";
        }
        outputFile << ", " << minCost;
        outputFile << ", " << time << "ms"
                                      "\n";
        outputFile.close();
        cout << "Results saved to tsp_results.csv" << endl;
    }
    else
    {
        cerr << "Failed to write the results to the file." << endl;
    }
}

/*void calculateAverageTime(const vector<vector<int>>& matrix, const string& search_method, const string& outputFile) {
    double total_time = 0.0;
    const int numIterations = 50;

    // Run the calculations 50 times
    for (int i = 0; i < numIterations; ++i) {
        double time = 0.0;
        auto result = branch_and_bound_tsp(matrix, search_method, time);
        total_time += time;
    }

    // Calculate average time
    double average_time = total_time / numIterations;

    // Write the average time to a file
    ofstream outFile(outputFile, ios::app);
    if (outFile.is_open()) {
        outFile << "Average Time for " << numIterations << " iterations: " << average_time << " ms\n";
        outFile.close();
    } else {
        cerr << "Unable to open file: " << outputFile << endl;
    }
}*/


int main() {
    std::vector<std::vector<int>> adjMatrix; // Macierz sąsiedztwa
    std::vector<int> theBestRoute;           // Najlepsza trasa
    int totalCost1 = std::numeric_limits<int>::max(); // Inicjalizacja dużą wartością
    std::string inputFile;                   // Nazwa pliku wejściowego
    std::string type;                        // Typ problemu (np. TSP, ATSP)
    int dimension = 0;                       // Liczba miast
    std::string format;                      // Format danych
    std::vector<std::vector<int>> unresolvedGraphData; // Nieprzetworzone dane grafu
    std::string algorithm;                   // Wybrany algorytm
    int randomIterations;                    // Liczba iteracji (jeśli używane)
    std::string showSteps;                   // Opcja wyświetlania kroków
    std::string outputFile;                  // Plik wyjściowy
    double executionTime;                    // Czas wykonania algorytmu
    int start_node = 0;                      // Początkowe miasto

    srand(static_cast<unsigned>(time(0))); // Inicjalizacja generatora losowego

    // Wczytanie konfiguracji z pliku
    configurate(CONFIGURATION_FILE, algorithm, randomIterations, inputFile, showSteps, outputFile);
    // Wczytanie danych TSP
    readTSPFile(inputFile, type, dimension, format, unresolvedGraphData);
    // Przetworzenie danych na macierz sąsiedztwa
    resolveFormat(type, dimension, format, unresolvedGraphData, adjMatrix);

    // Przygotowanie początkowych parametrów dla BFS
    std::vector<int> initial_path = {start_node};
    std::set<int> initial_visited = {start_node};

    // Uruchomienie algorytmu BFS
    auto start_time = std::chrono::high_resolution_clock::now();

    if (algorithm == "BFS") {
        BFS(adjMatrix, theBestRoute, totalCost1, initial_path, initial_visited, 0);
    } else if (algorithm == "DFS") {
        DFS(adjMatrix, theBestRoute, totalCost1, initial_path, initial_visited, 0);
    }else if (algorithm == "LC") { // Lowest Cost
        lowest_cost_first_search(adjMatrix, theBestRoute, totalCost1, initial_path, initial_visited, 0);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000000.0;

    // Zapis wyników do pliku
    saveResults(outputFile, type, algorithm, theBestRoute, totalCost1, executionTime);

    return 0;
}

