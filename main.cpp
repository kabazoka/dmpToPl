#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct POINT
{
    float posX;
    float posY;
};

struct MACRO
{
    string macroType;
    string macroName;
    POINT position;
    string orientation;
};

vector<string> splitByPattern(string content, const string& pattern);
string &trim(string &str);

int dbu = 2000;
unordered_map<string, MACRO> macroMap;

void readPL(const string& filename)
{
    ifstream file;
    string in_line;
    vector<string> content_array;
    stringstream ss1, ss2;
    MACRO macro;
    file.open(filename);
    getline(file, in_line);//ucla pl
    getline(file, in_line);//unit microns
    content_array = splitByPattern(in_line, " ");
    ss1 << content_array[3];
    ss1 >> dbu;
    while(getline(file, in_line))
    {
        content_array = splitByPattern(in_line, " ");
        macro.macroName = content_array[0];
        ss1 << content_array[1];
        ss1 >> macro.position.posX;
        ss2 << content_array[2];
        ss2 >> macro.position.posY;
        macro.orientation = content_array[4];
        macroMap.insert(pair<string, MACRO>(macro.macroName, macro));
    }
}

void readDMP(const string& filename)
{

}

void outputPL(const string& filename)
{

}

int main(int argc, char* argv[])
{

    string current_exec_name = argv[0]; // Name of the current exec program

    vector<string> all_args(argv, argv + argc);

    readPL(all_args[1]);
    readDMP(all_args[2]);
    outputPL(all_args[1]);

    return 0;
}

//splitting string by pattern
vector<string> splitByPattern(string content, const string& pattern)
{
    vector<string> words;
    size_t pos = 0;
    trim(content);
    while ((pos = content.find(pattern)) != string::npos)
    {
        string word = content.substr(0, pos);
        words.push_back(trim(word));
        content.erase(0, pos + pattern.length());
    }
    words.push_back(trim(content));
    return words;
}

//remove the blank spaces of the front and rear ends
string &trim(string &str)
{
    if (str.empty())
    {
        return str;
    }
    int str_length;
    int start = 0;
    int space_end = str.find_first_not_of(" ");
    str.erase(start, space_end);
    int space_start = (str.find_last_not_of(" ") + 1);
    str_length = str.length();
    str.erase(space_start, str_length);

    return str;
}