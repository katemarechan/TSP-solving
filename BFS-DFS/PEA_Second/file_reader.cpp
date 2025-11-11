#include "file_reader.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <limits>
#include <algorithm>

void configurate(const std::string &filename,
                 std::string &algorithm,
                 int &randomIteractions,
                 std::string &inputFile,
                 std::string &showSteps,
                 std::string &outputFile) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.find("INPUT_FILE:") != std::string::npos) {
            inputFile = line.substr(line.find(":") + 1);
            inputFile.erase(0, inputFile.find_first_not_of(" \t"));
        } else if (line.find("ALGORITHM:") != std::string::npos) {
            algorithm = line.substr(line.find(":") + 1);
            algorithm.erase(0, algorithm.find_first_not_of(" \t"));
        } else if (line.find("SHOW_STEPS:") != std::string::npos) {
            showSteps = line.substr(line.find(":") + 1);
            showSteps.erase(0, showSteps.find_first_not_of(" \t"));
        } else if (line.find("OUTPUT_FILE:") != std::string::npos) {
            outputFile = line.substr(line.find(":") + 1);
            outputFile.erase(0, outputFile.find_first_not_of(" \t"));
        }
    }
    file.close();
}

void readTSPFile(const std::string &filename,
                 std::string &type,
                 int &dimension,
                 std::string &format,
                 std::vector<std::vector<int>> &unresolvedGraphData) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    bool inEdgeWeightSection = false;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.find("TYPE:") != std::string::npos && type.empty()) {
            type = line.substr(line.find(":") + 1);
            type.erase(0, type.find_first_not_of(" \t"));
        } else if (line.find("DIMENSION:") != std::string::npos && dimension == 0) {
            std::istringstream iss(line.substr(line.find(":") + 1));
            iss >> dimension;
        } else if (line.find("FORMAT:") != std::string::npos && format.empty()) {
            format = line.substr(line.find(":") + 1);
            format.erase(0, format.find_first_not_of(" \t"));
        } else if (line.find("DATA_SECTION") != std::string::npos) {
            inEdgeWeightSection = true;
            continue;
        }

        if (inEdgeWeightSection) {
            if (line.find("EOF") != std::string::npos) {
                break;
            }

            std::vector<int> weightsRow;
            std::istringstream iss(line);
            int weight;

            while (iss >> weight) {
                weightsRow.push_back(weight);
            }

            unresolvedGraphData.push_back(weightsRow);
        }
    }
    file.close();
}

double euclideanDistance(int x1, int y1, int x2, int y2) {
    return std::floor(std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2)));
}

void resolveFormat(const std::string &type,
                   int dimension,
                   const std::string &format,
                   const std::vector<std::vector<int>> &unresolvedGraphData,
                   std::vector<std::vector<int>> &adjMatrix) {
    if (type == "TSP") {
        if (format == "EUC_2D") {
            adjMatrix.resize(dimension, std::vector<int>(dimension, 0));
            for (int i = 0; i < dimension; ++i) {
                for (int j = 0; j < dimension; ++j) {
                    if (i != j) {
                        adjMatrix[i][j] = euclideanDistance(
                                unresolvedGraphData[i][1], unresolvedGraphData[i][2],
                                unresolvedGraphData[j][1], unresolvedGraphData[j][2]);
                    } else {
                        adjMatrix[i][j] = 0;
                    }
                }
            }
        } else if (format == "FULL_MATRIX") {
            adjMatrix = unresolvedGraphData;
        }
    } else if (type == "ATSP") {
        adjMatrix = unresolvedGraphData;
    } else {
        std::cerr << "Invalid type" << std::endl;
    }
}

