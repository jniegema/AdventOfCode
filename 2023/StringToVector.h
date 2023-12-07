#pragma once

#include <string>
#include <vector>
#include <sstream>

inline int toInt(std::string const& s) { return stoi(s); }

inline void replaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

inline void removeBefore(std::string& subject, std::string const & removeBefore) {
    const size_t pos = subject.find(removeBefore); subject.erase(0, pos + 1);
}

inline std::string removeBefore(std::string const& subject, std::string const& removeBefore) {
    std::string result = subject;
    const size_t pos = result.find(removeBefore); result.erase(0, pos + 1);
    return result;
}


inline std::vector<std::string> stringToVector(std::string const& s, char delimiter) {
    std::vector<std::string> result;
    std::string token;

    std::stringstream ss(s);
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

inline std::vector<std::string> stringToVectorNoEmpty(std::string const& s, char delimiter) {
    std::vector<std::string> result = stringToVector(s,delimiter);
    result.erase(std::remove_if(result.begin(), result.end(), [](std::string const& s) { return s.empty(); }), result.end());
    return result;
}


inline std::vector<long long int> stringToVectorOfInt(std::string const& s, char delimiter) {
    std::vector<long long int> result;
    std::string token;

    std::stringstream ss(s);
    while (std::getline(ss, token, delimiter)) {
        if(token!="") result.push_back(std::stoll(token));
    }
    return result;
}

inline std::vector<char> stringToVectorOfChar(std::string const& s, char delimiter) {
    std::vector<char> result;
    std::string token;

    std::stringstream ss(s);
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token[0]);
    }
    return result;
}

