//
// Created by Ryan Avery on 12/6/2022.
//

#ifndef ADVENTOFCODE2022_DAY_7_H
#define ADVENTOFCODE2022_DAY_7_H

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
        using DirContents = std::vector<std::string>;

        Dir(const std::string& name, Dir* parent) : _name(name), _parent(parent) {}

        int getSize() const
        {
            int size = 0;
            for(const auto& file: _files)
            {
                size += file.size;
            }
            for (const auto& dir: _subdirs)
            {
                size += dir.getSize();
            }
            return size;
        }
        std::string getName() const
        {
            return _name;
        }

        Dirs& subDirs()
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
            return;
        }
        Dir* mkDir(const std::string& name)
        {
            _subdirs.push_back(Dir(name, this));
            return &_subdirs.back();
        }
        Dir* cd(const std::string& name)
        {
            if (name == "..")
            {
                return _parent;
            }
            else
            {
                auto subdir = std::find_if(_subdirs.begin(), _subdirs.end(), [name](Dir& dir){ return dir.getName() == name;});
                return &subdir[0];
            }
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

        Dir* dir(const std::string& name) {
            return _root.cd(name);
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
            auto found = std::find_if(_curDir->subDirs().begin(), _curDir->subDirs().end(), [name](Dir& subdir){
                return subdir.getName() == name;
            });
            if (found == _curDir->subDirs().end())
            {
                // New directory
                auto newDir = _curDir->mkDir(name);
                return *newDir;
            }
            else
            {
                _curDir = _curDir->cd(name);
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

        DirectoryBrowser(const std::string& in) : _fileSystem() {
            parseStringData(in);
        }

        Dir& getRoot() { return _fileSystem.cd("/"); }


        DirectorySizeList getDirSizeList() {return getListOfDirectorySizes(getRoot());}

    private:
        const std::string kCD = "cd";
        const std::string kLS = "ls";

        using TerminalLineList = std::vector<std::string>;

        DirectorySizeList getListOfDirectorySizes(Dir& dir)
        {
            int size = 0;
            DirectorySizeList sizeList;
            sizeList.push_back({dir.getName(), dir.getSize()});
            for(auto sub = dir.subDirs().begin(); sub != dir.subDirs().end(); ++sub)
            {
                auto dsl = getListOfDirectorySizes(*sub);
                sizeList.insert(sizeList.end(), dsl.begin(), dsl.end());
            }
            return std::move(sizeList);
        }

        void parseStringData(const std::string& inData) {
            TerminalLineList list;
            auto ss = std::stringstream{inData};

            for (std::string line; std::getline(ss, line, '\n');) {
                list.push_back(line);
            }
            // Now we have a list of commands/responses
            for (auto line = list.begin(); line != list.end(); line++)
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

            return;
        }

        bool getCommand(const std::string& line, std::string& cmd)
        {
            const auto promptSize = kPrompt.length();
            const auto possiblePrompt = line.substr(0, promptSize);
            auto isCmd = (possiblePrompt == kPrompt);
            if (isCmd)
                cmd = line.substr(kPrompt.length() + 1);
            return isCmd;
        }

        TerminalLineList getCmdResponse(TerminalLineList::iterator line, TerminalLineList::iterator end)
        {
            TerminalLineList response;
            line++; // Move past the cmd
            while ((line != end) && line->substr(0, kPrompt.length()) != kPrompt)
            {
                response.push_back(*line++);
            }
            return response;
        }

        void parseCommand(const std::string& cmdLine, TerminalLineList response)
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

        void parseLsCommand(TerminalLineList response)
        {
            const std::string kDir = "dir";
            for (const auto& line: response)
            {
                if (line.find(kDir) != std::string::npos)
                {
                    std::string name = line.substr(kDir.length() + 1);
                    _fileSystem.curdir()->mkDir(name);
                }
                else
                {   // Is a file
                    auto sizeStr = line.substr(0, line.find(" "));
                    auto size = std::stoi(sizeStr);
                    auto name = line.substr(line.find(" "));
                    _fileSystem.curdir()->mk(name, size);
                }
            }
        }

        const std::string kPrompt = "$";
        FileSystem _fileSystem;
    };
    int getSizeOfDir(DirectoryBrowser::DirectorySizeList dsl, const std::string& name)
    {
        auto found = std::find_if(dsl.begin(), dsl.end(), [name](DirectoryBrowser::DirectorySize& dir){
            return dir.first == name;
        });
        if (found != dsl.end())
            return found->second;
        return 0;
    }

    int getSizeOfDirectoriesAtMost(DirectoryBrowser::DirectorySizeList dsl, int maxSize)
    {
        auto size = std::accumulate(dsl.begin(), dsl.end(), 0,
                                    [maxSize](int acc, DirectoryBrowser::DirectorySize& dir) -> int {
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

        auto dir = std::find_if(dsl.begin(), dsl.end(), [extraSpaceNeeded](DirectoryBrowser::DirectorySize& dsl){
            return dsl.second > extraSpaceNeeded;
        });

        if (dir != dsl.end())
            return dir->second;
        return 0;
    }

}

#endif //ADVENTOFCODE2022_DAY_7_H
