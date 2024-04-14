#include <iostream>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <locale>
#include <fstream>
#include <string>

using namespace std;

bool isFileEmpty(const string& filename) {
    try {
        // Get the file size
        uintmax_t fileSize = filesystem::file_size(filename);
        return fileSize == 0;
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Error checking file size: " << e.what() << endl;
        return false;
    }
}

string left_trim(const string& str) {
    int numStartSpaces = 0;
    for (char ch : str) {
        if (!isspace(ch))
            break;
        numStartSpaces++;
    }
    return str.substr(numStartSpaces);
}

// Right trim the given string
string right_trim(const string& str) {
    int numEndSpaces = 0;
    for (int i = str.length() - 1; i >= 0; i--) {
        if (!isspace(str[i]))
            break;
        numEndSpaces++;
    }
    return str.substr(0, str.length() - numEndSpaces);
}

string trim(string& str) {
    return right_trim(left_trim(str));
}

bool stob(string str) {
    return str != "0";
}

string getCurrentDate() {
    time_t now = time(0);
    string currentDate = ctime(&now);
    return trim(currentDate);
}