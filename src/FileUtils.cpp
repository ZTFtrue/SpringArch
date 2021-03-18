#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <stack>
#include <list>

using namespace std;
namespace fs = std::filesystem;

class FileUtils
{
public:
    int writeFile(string content,string path)
    {
        ofstream myfile;
        myfile.open(path);
        myfile << content;
        myfile.close();
        return 0;
    }
    int readFile(std::list<string> *datas, string path)
    {
        string line;
        ifstream myfile(path);
        if (myfile.is_open())
        {
            while (getline(myfile, line))
            {

                int N = line.length();
                if (N == 0)
                {
                    continue;
                }
                datas->push_back(line);
            }
            myfile.close();
        }
        else
        {
            cout << "Unable to open file";
            return -1;
        }
        return 0;
    }

    int iteratorFolder(std::vector<std::string> *datas, string path)
    {
        for (auto &p : fs::directory_iterator(path))
        {

            fs::path dir = p.path();
            if (fs::is_directory(p.symlink_status()))
            {
                datas->push_back(dir.string());
            }
        }
        return 0;
    }
    int iteratorFile(std::vector<std::string> *datas, string path)
    {
        for (auto &p : fs::directory_iterator(path))
        {

            fs::path dir = p.path();
            if (!fs::is_directory(p.symlink_status()))
            {
                datas->push_back(dir.string());
            }
        }
        return 0;
    }
    /**
     * std::map<std::string, std::map<std::string, std::string>> zones  region city realpath
     * Aisa Shanghai /usr/share/zoneinfo/Asia/Shanghai
     **/
    int getTimeZone(std::map<string, string> *datas, string bootstrapPath)
    {
        std::string path = bootstrapPath + "/usr/share/zoneinfo";
        std::vector<std::string> regions;
        std::vector<std::string> zones;
        iteratorFolder(&regions, path);
        for (std::vector<std::string>::iterator it = regions.begin(); it != regions.end(); ++it)
        {
            std::vector<std::string> city;
            iteratorFile(&city, *it);
            for (std::vector<std::string>::iterator itCity = city.begin(); itCity != city.end(); ++itCity)
            {
                zones.push_back(*itCity);
            }
        }
        std::vector<std::string> regions2;
        iteratorFile(&regions2, path);
        for (std::vector<std::string>::iterator itRegions2 = regions2.begin(); itRegions2 != regions2.end(); ++itRegions2)
        {
            zones.push_back(*itRegions2);
        }
        for (std::vector<std::string>::iterator it = zones.begin(); it != zones.end(); ++it)
        {
            string value = *it;
            value = value.replace(0, bootstrapPath.size(), "");
            datas->insert(std::pair<string, string>(value, value));
        }
        return 0;
    }

    int getKeybords(std::map<string, string> *keymaps, string bootstrapPath)
    {
        string rootPath = "/usr/share/kbd/keymaps";
        string path = bootstrapPath + rootPath;
        std::stack<std::string> directorys;
        directorys.push(path);
        while (!directorys.empty())
        {
            string a = directorys.top() + "";
            directorys.pop();
            for (auto &p : fs::directory_iterator(a))
            {

                fs::path dir = p.path();
                if (fs::is_directory(p.symlink_status()))
                {
                    directorys.push(dir.string());
                }
                else
                {
                    string label = dir.string();
                    auto startDot = label.find_last_of("/");
                    string result = label.substr(startDot + 1, std::string::npos);
                    startDot = result.find_first_of(".");
                    if (startDot == std::string::npos)
                    {
                        continue;
                    }

                    string checks = result.substr(startDot, std::string::npos);
                    string value = result.substr(0, startDot);
                    label = label.replace(0, bootstrapPath.size() + rootPath.size(), "");
                    if (checks == ".map.gz")
                    {
                        keymaps->insert(std::pair<string, string>(label, value));
                    }
                }
            }
        }
        return 0;
    }
};
