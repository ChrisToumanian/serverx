#include "reader.h"
#include <iostream>

std::vector<std::string> Reader::get_file_lines(std::string filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);

    for (std::string line; std::getline(file, line); )
    {
        lines.push_back(line);
    }

    return lines;
}
    
std::vector<std::string> Reader::split(std::string str, std::string delimiter)
{
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;

    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);

    return result;
}

std::string Reader::join(std::vector<std::string> arr)
{
    std::string str = "";
    for (int i = 0; i < arr.size(); i++)
        str += arr[i] + " ";
    return str;
}

std::string Reader::join(std::vector<std::string> arr, int first_element)
{
    std::string str = "";
    for (int i = first_element; i < arr.size(); i++)
        str += arr[i] + " ";
    return str;
}

std::string Reader::join(std::vector<std::string> arr, int first_element, int last_element)
{
    std::string str = "";
    for (int i = first_element; i < last_element + 1; i++)
        str += arr[i] + " ";
    return str;
}

void Reader::rtrim(std::string &str)
{
    str.erase(str.find_last_not_of(" \n\r\t") + 1);
}

int Reader::to_int(std::string &str)
{
    if (str != "")
    {
        return std::stoi(str);
    }
    return 0;
}

bool Reader::get_bool(std::string &str)
{
	if (str == "true")
		return true;
	else
		return false;
}
