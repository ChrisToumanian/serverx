#ifndef READER_H
#define READER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class Reader
{
public:
    static std::vector<std::string> get_file_lines(std::string filename);
    static std::vector<std::string> split(std::string str, std::string delimiter);
    static std::string join(std::vector<std::string> arr);
    static std::string join(std::vector<std::string> arr, int first_element);
    static std::string join(std::vector<std::string> arr, int first_element, int last_element);
    static void rtrim(std::string &str);
    static int to_int(std::string &str);
	static bool get_bool(std::string &str);
};

#endif
