#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

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
    string fixed;
};

vector<string> splitByPattern(string content, const string& pattern);
string &trim(string &str);

int dbu = 2000;
std::multimap<string, MACRO> macroMap;

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
        if(content_array.size() >= 6)
            macro.fixed = content_array[5];
        macroMap.insert(pair<string, MACRO>(macro.macroName, macro));
    }
}

void readDMP(const string& filename)
{
    ifstream file;
    string in_line;
    vector<string> content_array;
    stringstream ss1, ss2;
    file.open(filename);
    getline(file, in_line); //version
    getline(file, in_line); //design case01
    getline(file, in_line); //unit distance
    while (getline(file, in_line))
    {
        if (in_line.find("COMPONENTS") != string::npos)
        {
            MACRO macro;
            content_array = splitByPattern(in_line, " ");
            int compNum;
            ss1 << content_array[1];
            ss1 >> compNum;
            for (int i = 0; i < compNum; i++)
            {
                getline(file, in_line);
                content_array = splitByPattern(in_line, " ");
                string compName = content_array[1];
                macro.macroName = compName;
                macro.macroType = content_array[2];
                getline(file, in_line);
                content_array = splitByPattern(in_line, " ");
                //macro.placeType = content_array[1];
                ss1 << content_array[3];
                ss1 >> macro.position.posX;
                ss2 << content_array[4];
                ss2 >> macro.position.posY;
                macro.orientation = content_array[6];
                macroMap.insert(pair<string, MACRO>(compName, macro));
            }
        }
        
    }

}

void outputPL(const string& filename)
{
    ofstream ofs;
    ofs.open(filename);
    if (!ofs.is_open())
    {
        cout << "Failed to open file.\n";
    }
    else
    {
        ofs << "UCLA pl 1.0\n";
        ofs << "# Unit MICRONS: " << dbu << "\n\n" ;
        for (const auto& macro : macroMap)
        {
            ofs << macro.second.macroName << (int)macro.second.position.posX 
            << " " << (int)macro.second.position.posY << " : " 
            << macro.second.orientation << macro.second.fixed << endl;
        }
        ofs << "\n\n\n";
        
        ofs.close();
    }
}

int main(int argc, char* argv[])
{

    string current_exec_name = argv[0]; // Name of the current exec program

    vector<string> all_args(argv, argv + argc);

    readPL(all_args[1]);
    readDMP(all_args[2]);
    outputPL(all_args[3]);

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