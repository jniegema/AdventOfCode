#pragma once

#include <string>
#include <vector>
#include <fstream>

inline std::vector<std::string> loadFile(std::string const& filename) {
    std::ifstream txtfile(filename);
    std::string str;

    std::vector<std::string> rawFile; // (std::istream_iterator<std::string>(file1), {});
    while (std::getline(txtfile, str))
    {
        rawFile.push_back(str);
    }

    return rawFile;
}
