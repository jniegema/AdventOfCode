#include "StringToVector.h"
#include "FileIO.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <array>
#include <string>
#include <numeric>
#include <cassert>
#include <unordered_map>

struct FsItem {
    FsItem(std::string const& in_name) : name(in_name) {}
    virtual ~FsItem() = default;
    virtual int getSize() const = 0;
    virtual bool isDir() const = 0;
    virtual void listDirs(std::string space = "") const = 0;

    std::string getName() const { return name; }
    std::string name;
};

struct FsFile : public FsItem {
    FsFile(std::string const& in_name, int in_size) : FsItem(in_name), size(in_size) {}

    int getSize() const override { return size; }
    bool isDir() const override { return false; }
    
    void listDirs(std::string space = "") const override {
        std::cout << space << getName() << "  " << getSize() << std::endl;
    }

    int         size;
};

struct FsDir : public FsItem {

    FsDir(std::string const& in_name, FsDir* in_parent) : FsItem(in_name), parent(in_parent) {}

    int getSize() const override {
        int result = 0;
        for (auto const & curEntry : list) {
            result += curEntry.second->getSize();
        }
        return result;
    }

    bool isDir() const override { return true; };

    void listDirs(std::string space = "") const override {
        std::cout << space << getName() << "  " << getSize() << std::endl;
        for (auto const& curPair : list) {
            auto const& curEntry = curPair.second;
            curEntry->listDirs(space+" ");
        }
    }

    std::vector<FsFile> flattenSub() {
        std::vector<FsFile> result;
        result.emplace_back(name, getSize());
        for (auto const& curPair : list) {
            auto const& curEntry = curPair.second;
            if (curEntry->isDir()) {
                auto const& subdirs = dynamic_cast<FsDir*>(curEntry.get())->flattenSub();

                result.insert(result.end(), subdirs.begin(), subdirs.end());
            }
        }

        return result;
    }

    FsDir* parent;
    std::unordered_map<std::string, std::unique_ptr<FsItem> > list;
};

void main7()
{
    auto rawFile = loadFile("aoc_input_7.txt");

    FsDir root("/", nullptr);
   
    FsDir* root_ptr = &root;

    for (auto const& str : rawFile)
    {
        auto vec = stringToVector(str, ' ');

        // A command
        if (vec[0] == "$") {
            if (vec[1] == "cd") {
                const auto& str2 = vec[2];
                if (str2 == "..") {
                    root_ptr = root_ptr->parent;
                } else if (str2 == "/") {
                    root_ptr = &root;
                } else {
                    root_ptr = dynamic_cast<FsDir*>( (root_ptr->list[str2]).get() );
                }
            }
            if (vec[1] == "ls") { } ///< Ignored for now
        }

        if (vec[0] == "dir") {
            root_ptr->list[vec[1]] = std::make_unique<FsDir>(vec[1], root_ptr);
        }

        if (std::isdigit(vec[0][0])) {
            auto const size = std::stoi(vec[0]);
            root_ptr->list[vec[1]] = std::make_unique<FsFile>(vec[1], size);
        }
    }

    // === Tree is built, now query stuff ===

    auto flatDirList = root.flattenSub();

    int sum = 0;
    std::for_each(flatDirList.begin(), flatDirList.end(), [&](auto const & x) {if (x.getSize()<100000) sum += x.getSize(); });

    std::cout << "Result for Part I : " << sum << std::endl;
    

    int total_sum = root.getSize();
    int total_disk = 70000000;
    int requ_free  = 30000000;
    int cur_free = total_disk - total_sum;
    int target_to_free = requ_free - cur_free;

    std::sort(flatDirList.begin(), flatDirList.end(), [](auto& a, auto& b) { return a.getSize() < b.getSize(); });
    const auto result2 = std::lower_bound(flatDirList.begin(), flatDirList.end(), target_to_free, [](auto& a, auto& b) { return a.getSize() < b; });

    std::cout << "Result for Part II : " << result2->getSize() << std::endl;
}