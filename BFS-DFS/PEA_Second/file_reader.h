//
// Created by zart0 on 28/11/2024.
//

#ifndef PEA_BETTER_FILE_READER_H
#define PEA_BETTER_FILE_READER_H


#include <string>
#include <vector>
#include <set>

void configurate(const std::string &filename,
                 std::string &algorithm,
                 int &randomIteractions,
                 std::string &inputFile,
                 std::string &showSteps,
                 std::string &outputFile);

void readTSPFile(const std::string &filename,
                 std::string &type,
                 int &dimension,
                 std::string &format,
                 std::vector<std::vector<int>> &unresolvedGraphData);

double euclideanDistance(int x1, int y1, int x2, int y2);

void resolveFormat(const std::string &type,
                   int dimension,
                   const std::string &format,
                   const std::vector<std::vector<int>> &unresolvedGraphData,
                   std::vector<std::vector<int>> &adjMatrix);


#endif //PEA_BETTER_FILE_READER_H
