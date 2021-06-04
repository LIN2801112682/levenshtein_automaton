#include "common/common.h"
#include <iostream>
#include <fstream>

std::vector<std::string>
readAllLines(const std::string &path)
{
    std::vector<std::string> lines;
    std::string line;
    std::ifstream ifs{path, std::ifstream::in};
    while (getline(ifs, line))
        lines.push_back(line);
    ifs.close();
    return lines;
}