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

// Inplace can be faster but this is often more convenient
inline std::string replaceString(const std::string& subject, const std::string& search, const std::string& replace)
{
    std::string result = subject;
    replaceStringInPlace(result, search, replace);
    return result;
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

// Remove all occurence of "search" in target
inline std::string removeAll(const std::string& target, const std::string& search)
{
    std::string result = target;
    std::string::size_type pos = result.find(search, 0);
    while (pos != std::string::npos) {
        result.erase(pos, search.length());
        pos = result.find(search, pos);
    }
    return result;
}


inline std::string trimString(const std::string& target, const char toTrim = ' ')
{
    std::string result = target;
    result.erase(0, result.find_first_not_of(toTrim));
    result.erase(result.find_last_not_of(toTrim) + 1);
    return result;
}

inline std::string trimString(const std::string& target, const std::string& charsToTrim)
{
    std::string result = target;
    result.erase(0, result.find_first_not_of(charsToTrim));
    result.erase(result.find_last_not_of(charsToTrim) + 1);
    return result;
}

inline std::vector<std::string> stringToVectorTrimmed(std::string const& s, char delimiter, const std::string& charsToTrim=" \t") {
    std::vector<std::string> result;
    std::string token;

    std::stringstream ss(s);
    while (std::getline(ss, token, delimiter)) {
        result.push_back(trimString(token, charsToTrim));
    }
    return result;
}
