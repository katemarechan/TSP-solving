#include <iostream>
#include <vector>
#include <iomanip>
#include <list>
#include <set>
#include <utility>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cmath>
#include <limits>

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
int calculateCost(vector<vector<int>> &adjMatrix, int dimension, vector<int> &route)
{

    int totalCost = 0;
    for (int i = 0; i < dimension - 1; ++i)
    {
        totalCost += adjMatrix[route[i]][route[i + 1]];
    }
    totalCost += adjMatrix[route[dimension - 1]][route[0]];
    return totalCost;
}
void bruteForce(int dimension, vector<vector<int>> adjMatrix, vector<int> &theBestRoute, int &totalCost1, double &executionTime, string &showSteps)
{

    vector<int> cities(dimension-1);
    int minCost = INT_MAX;
    vector<int> bestRoute;
    StartCounter();
    // for (int i = 1; i < dimension; ++i)
    // {
    //     cities.push_back(i);
    // }
    iota(cities.begin(), cities.end(), 1);
    do
    {
        vector<int> tour = {0};
        tour.insert(tour.end(), cities.begin(), cities.end());
        
        int currentCost = calculateCost(adjMatrix, dimension, tour);

        if (currentCost < minCost)
        {
            minCost = currentCost;
            bestRoute = tour;
        }
        if (showSteps == "ON")
        {
            cout << endl
                 << "The lowest cost by now: " << minCost << endl;
            cout << "Analized route: " << endl;
            for (int city : bestRoute)
            {
                cout << city << ", ";
            }
            cout << endl;
        }
    } while (next_permutation(cities.begin(), cities.end()));
    executionTime = GetCounter();
    bestRoute.push_back(0);
    theBestRoute = bestRoute;
    totalCost1 = minCost;
}
void nearestNeighborTSP(const vector<vector<int>> &adjMatrix, int dimension, vector<int> &theBestRoute, int &totalCost1, double &executionTime, string &showSteps)
{
    vector<bool> visited(dimension, false);
    visited[0] = true;
    vector<int> tour;
    int totalDistance = 0;
    int currentCity = 0;
    StartCounter();
    tour.push_back(currentCity);

    for (int i = 1; i < dimension; i++)
    {
        int nearestCity = -1;
        int minDistance = numeric_limits<int>::max();

        for (int j = 0; j < dimension; j++)
        {
            if (!visited[j] && adjMatrix[currentCity][j] < minDistance)
            {
                nearestCity = j;
                minDistance = adjMatrix[currentCity][j];
            }
        }

        tour.push_back(nearestCity);
        visited[nearestCity] = true;
        totalDistance += minDistance;
        currentCity = nearestCity;
        if (showSteps == "ON")
        {
            cout << "The lowest cost by now: " << totalDistance << endl;
            cout << "Analized route: " << endl;
            for (int city : tour)
            {
                cout << city << ", ";
            }
        }
    }

    totalDistance += adjMatrix[currentCity][0];
    tour.push_back(0);

    executionTime = GetCounter();

    theBestRoute = tour;
    totalCost1 = totalDistance;
}
void randomAlgorithmTSP(vector<vector<int>> &adjMatrix, int dimension, int iterations, vector<int> &theBestRoute, int &totalCost1, double &executionTime, string &showSteps)
{

    vector<int> bestTour(dimension);
    iota(bestTour.begin(), bestTour.end(), 0);
    int bestDistance = numeric_limits<int>::max();
    vector<int> tour = bestTour;
    int distance;
    StartCounter();
    for (int i = 0; i < iterations; i++)
    {
        
        random_shuffle(tour.begin() + 1, tour.end());

        distance = calculateCost(adjMatrix, dimension, tour);

        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestTour = tour;
        }
        if (showSteps == "ON")
        {
            cout << "The lowest cost by now: " << bestDistance << endl;
            cout << "Analyzed route: " << endl;
            for (int city : bestTour)
            {
                cout << city << ", ";
            }
        }
    }

    executionTime = GetCounter();

    theBestRoute = bestTour;
    totalCost1 = bestDistance;
}

void configurate(const string &filename, string &algorithm, int &randomIteractions, string &inputFile, string &showSteps, string &outputFile)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {

        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.find("INPUT_FILE:") != string::npos)
        {
            inputFile = line.substr(line.find(":") + 1);
            inputFile.erase(0, inputFile.find_first_not_of(" \t"));
        }
        if (line.find("ALGORITHM:") != string::npos)
        {
            algorithm = line.substr(line.find(":") + 1);
            algorithm.erase(0, algorithm.find_first_not_of(" \t"));
        }
        if (line.find("RANDOM_ITERATIONS:") != string::npos && algorithm == "RANDOM")

        {
            string randomIteractionsStr;
            randomIteractionsStr = line.substr(line.find(":") + 1);
            randomIteractionsStr.erase(0, randomIteractionsStr.find_first_not_of(" \t"));
            randomIteractions = stoi(randomIteractionsStr);
        }
        if (line.find("SHOW_STEPS:") != string::npos)
        {
            showSteps = line.substr(line.find(":") + 1);
            showSteps.erase(0, showSteps.find_first_not_of(" \t"));
        }
        if (line.find("OUTPUT_FILE:") != string::npos)
        {
            outputFile = line.substr(line.find(":") + 1);
            outputFile.erase(0, outputFile.find_first_not_of(" \t"));
        }
    }
    file.close();
}

void readTSPFile(const string &filename, string &type, int &dimension, string &format, vector<vector<int>> &unresolvedGraphData)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    string line;
    bool inEdgeWeightSection = false;

    while (getline(file, line))
    {

        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.find("TYPE:") != string::npos && type.empty())
        {
            type = line.substr(line.find(":") + 1);
            type.erase(0, type.find_first_not_of(" \t"));
        }
        else if (line.find("DIMENSION:") != string::npos && dimension == 0)
        {
            istringstream iss(line.substr(line.find(":") + 1));
            iss >> dimension;
        }
        else if (line.find("FORMAT:") != string::npos && format.empty())
        {
            format = line.substr(line.find(":") + 1);
            format.erase(0, format.find_first_not_of(" \t"));
        }
        else if (line.find("DATA_SECTION") != string::npos)
        {
            inEdgeWeightSection = true;
            continue;
        }

        if (inEdgeWeightSection)
        {
            if (line.find("EOF") != string::npos)
            {
                break;
            }

            vector<int> weightsRow;
            istringstream iss(line);
            int weight;

            while (iss >> weight)
            {
                weightsRow.push_back(weight);
            }

            unresolvedGraphData.push_back(weightsRow);
        }
    }

    file.close();
}
double euclideanDistance(int x1, int y1, int x2, int y2)
{
    return floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}
void resolveFormat(string type, int dimension, string format, vector<vector<int>> &unresolvedGraphData, vector<vector<int>> &adjMatrix)
{
    if (type == "TSP")
    {
        if (format == "EUC_2D")
        {
            adjMatrix.resize(dimension, vector<int>(dimension, 0));
            for (int i = 0; i < dimension; ++i)
            {
                for (int j = 0; j < dimension; ++j)
                {
                    if (i != j)
                    {

                        adjMatrix[i][j] = euclideanDistance(unresolvedGraphData[i][1], unresolvedGraphData[i][2],
                                                            unresolvedGraphData[j][1], unresolvedGraphData[j][2]);
                    }
                    else
                    {

                        adjMatrix[i][j] = 0;
                    }
                }
            }
        }
        else if (format == "FULL_MATRIX")
        {
            adjMatrix = unresolvedGraphData;
        }
        else if (format == "LOWER_DIAG_ROW")
        {
            vector<int> array1D(dimension * dimension);

            int index = 0;
            for (int i = 0; i < dimension; ++i)
            {
                for (int j = 0; j < dimension; ++j)
                {
                    array1D[index] = unresolvedGraphData[i][j];
                    index++;
                }
            }
        }
    }
    else if (type == "ATSP")
    {
        adjMatrix = unresolvedGraphData;
    }
    else
    {
        cerr << "Invalid type";
    }
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
        cout << "Results written to tsp_results.csv" << endl;
    }
    else
    {
        cerr << "Unable to open file for writing." << endl;
    }
}
int main()
{
    vector<vector<int>> adjMatrix;
    vector<int> theBestRoute;
    int totalCost1;
    string inputFile;
    string type;
    int dimension = 0;
    string format;
    vector<vector<int>> unresolvedGraphData;
    string algorithm;
    int randomIterations;
    string showSteps;
    string outputFile;
    double executionTime;
    srand(static_cast<unsigned>(time(0)));

    configurate(CONFIGURATION_FILE, algorithm, randomIterations, inputFile, showSteps, outputFile);
    readTSPFile(inputFile, type, dimension, format, unresolvedGraphData);
    // cout << inputFile << "    " << algorithm << endl;
    // cout << "Type: " << type << endl;
    // cout << "Dimension: " << dimension << endl;
    // cout << "Edge Weight Format: " << format << endl;
    resolveFormat(type, dimension, format, unresolvedGraphData, adjMatrix);
    // cout << "Adjacency Matrix:" << endl;
    // printAdjacencyMatrix(adjMatrix);
    if (algorithm == "BRUTE_FORCE")
        bruteForce(dimension, adjMatrix, theBestRoute, totalCost1, executionTime, showSteps);
    else if (algorithm == "NEAREST_NEIGHBOR")
        nearestNeighborTSP(adjMatrix, dimension, theBestRoute, totalCost1, executionTime, showSteps);
    else if (algorithm == "RANDOM")
        randomAlgorithmTSP(adjMatrix, dimension, randomIterations, theBestRoute, totalCost1, executionTime, showSteps);

    saveResults(outputFile, type, algorithm, theBestRoute, totalCost1, executionTime);
    // add random iterations conf
    //  ujednoliczenie zmiennych
    //  timers
    //  wybranie algorytmu

    return 0;
}
