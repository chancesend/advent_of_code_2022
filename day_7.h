//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENT_OF_CODE_2022_DAY_7_H
#define ADVENT_OF_CODE_2022_DAY_7_H

#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <stack>
#include <iostream>

namespace Day7 {

    class Dir {
    public:
        struct File
        {
            std::string name;
            int size;
        };
        using Files = std::vector<File>;
        using Dirs = std::vector<Dir>;

        Dir(const std::string& name, Dir* parent) : _name(name), _parent(parent) {}

        [[nodiscard]] int getSize() const
        {
            auto size = std::accumulate(_files.begin(), _files.end(), 0, [](int&& acc, const File& file){
                acc += file.size;
                return acc;
            });
            size += std::accumulate(_subdirs.begin(), _subdirs.end(), 0, [](int&& acc, const Dir& dir){
                acc += dir.getSize();
                return acc;
            });
            return size;
        }
        [[nodiscard]] const std::string& getName() const
        {
            return _name;
        }

        [[nodiscard]] const Dirs& subDirs() const
        {
            return _subdirs;
        }

        Dir& getParent()
        {
            return *_parent;
        }

        void mk(const std::string& name, int size)
        {
            _files.push_back(File{name, size});
        }

        Dir* mkDir(const std::string& name)
        {
            _subdirs.push_back(Dir(name, this));
            return &_subdirs.back();
        }

        Dir* subdir(const std::string& name)
        {
            if (name == "..")
            {
                return _parent;
            }

            auto subdir = std::find_if(_subdirs.begin(), _subdirs.end(), [name](Dir& dir){ return dir.getName() == name;});
            return &subdir[0];
        }


    private:
        std::string _name;
        Files _files;
        Dirs _subdirs;
        Dir* _parent;
    };

    class FileSystem {
    public:

        FileSystem() : _root(kRoot, nullptr), _curDir(&_root) {}

        Dir& dir(const std::string& name) {
            return *_root.subdir(name);
        }

        Dir& cd(const std::string& name)
        {
            if (name == kRoot)
            {
                _curDir = &_root;
                return *_curDir;
            }
            if (name == kParent)
            {
                _curDir = &_curDir->getParent();
                return *_curDir;
            }

            auto found = std::find_if(_curDir->subDirs().cbegin(), _curDir->subDirs().cend(), [name](const Dir& subdir){
                return subdir.getName() == name;
            });
            if (found == _curDir->subDirs().cend())
            {
                // New directory
                auto newDir = _curDir->mkDir(name);
                return *newDir;
            }
            else
            {
                _curDir = _curDir->subdir(name);
                return *_curDir;
            }
        }

        Dir* curdir() { return _curDir; }

    private:
        const std::string kRoot{"/"};
        const std::string kParent{".."};

        Dir _root;
        Dir* _curDir;
    };

    class DirectoryBrowser {
    public:
        using DirectorySize = std::pair<std::string, int>;
        using DirectorySizeList = std::vector<DirectorySize>;

        explicit DirectoryBrowser(const std::string& in) : _fileSystem() {
            parseStringData(in);
        }

        Dir& getRoot() { return _fileSystem.cd("/"); }


        DirectorySizeList getDirSizeList() {return getListOfDirectorySizes(getRoot());}

    private:
        const std::string kCD = "cd";
        const std::string kLS = "ls";

        using TerminalLineList = std::vector<std::string>;

        static DirectorySizeList getListOfDirectorySizes(const Dir& dir)
        {
            auto sizeList = std::accumulate(dir.subDirs().cbegin(), dir.subDirs().cend(), DirectorySizeList{{dir.getName(), dir.getSize()}},
                                            [](DirectorySizeList&& dsl, const Dir& sub){
                auto subDsl = getListOfDirectorySizes(sub);
                dsl.insert(dsl.end(), subDsl.cbegin(), subDsl.cend());
                return dsl;
            });
            return sizeList;
        }

        void parseStringData(const std::string& inData) {
            TerminalLineList list;
            auto ss = std::stringstream{inData};

            for (std::string line; std::getline(ss, line, '\n');) {
                list.push_back(line);
            }
            // Now we have a list of commands/responses
            auto line = list.begin();
            while(++line != list.end())
            {
                std::string cmd;
                if (getCommand(*line, cmd))
                {
                    auto response = getCmdResponse(line, list.end());
                    std::advance(line, response.size());  // Move past the response
                    parseCommand(cmd, response);
                }
            }
            // Reset to root
            _fileSystem.cd("/");
        }

        static bool getCommand(const std::string& line, std::string& cmd)
        {
            const auto promptSize = kPrompt.length();
            const auto possiblePrompt = line.substr(0, promptSize);
            auto isCmd = (possiblePrompt == kPrompt);
            if (isCmd)
                cmd = line.substr(kPrompt.length() + 1);
            return isCmd;
        }

        static TerminalLineList getCmdResponse(TerminalLineList::iterator line, TerminalLineList::iterator end)
        {
            TerminalLineList response;
            line++; // Move past the cmd
            while ((line != end) && line->substr(0, kPrompt.length()) != kPrompt)
            {
                response.push_back(*line++);
            }
            return response;
        }

        void parseCommand(const std::string& cmdLine, const TerminalLineList& response)
        {
            auto cmd = cmdLine.substr(0, kCD.length());
            if (cmd == kCD)
            {
                auto dir = cmdLine.substr(kCD.length()+1);
                _fileSystem.cd(dir);
            }
            if (cmd == kLS)
            {
                parseLsCommand(response);
            }
        }

        void parseLsCommand(const TerminalLineList& response)
        {
            const std::string kDir{"dir"};
            for (const auto& line: response)
            {
                if (line.find(kDir) != std::string::npos)
                {   // Is a dir
                    auto name = line.substr(kDir.length() + 1);
                    _fileSystem.curdir()->mkDir(name);
                }
                else
                {   // Is a file
                    auto sizeStr = line.substr(0, line.find(' '));
                    auto size = std::stoi(sizeStr);
                    auto name = line.substr(line.find(' '));
                    _fileSystem.curdir()->mk(name, size);
                }
            }
        }

        static const std::string kPrompt;
        FileSystem _fileSystem;
    };

    const std::string DirectoryBrowser::kPrompt = "$";

    int getSizeOfDir(DirectoryBrowser::DirectorySizeList dsl, const std::string& name)
    {
        auto found = std::find_if(dsl.cbegin(), dsl.cend(), [name](const DirectoryBrowser::DirectorySize& dir){
            return dir.first == name;
        });
        if (found != dsl.end())
            return found->second;
        return 0;
    }

    int getSizeOfDirectoriesAtMost(const DirectoryBrowser::DirectorySizeList& dsl, int maxSize)
    {
        auto size = std::accumulate(dsl.cbegin(), dsl.cend(), 0,
                                    [maxSize](int acc, const DirectoryBrowser::DirectorySize& dir) -> int {
                                        if (dir.second <= maxSize)
                                            acc += dir.second;
                                        return acc;
                                    });

        return size;
    }

    int getDirSizeToDeleteToReclaimSpace(DirectoryBrowser::DirectorySizeList dsl, int extraSpaceNeeded)
    {
        // Order the list for dirs by size
        std::sort(dsl.begin(), dsl.end(),
                  [](DirectoryBrowser::DirectorySize& rsl, DirectoryBrowser::DirectorySize& lsl){
            return rsl.second < lsl.second;
        });

        auto dir = std::find_if(dsl.cbegin(), dsl.cend(), [extraSpaceNeeded](const DirectoryBrowser::DirectorySize& dsl){
            return dsl.second > extraSpaceNeeded;
        });

        if (dir != dsl.end())
            return dir->second;
        return 0;
    }

}

#endif //ADVENT_OF_CODE_2022_DAY_7_H
