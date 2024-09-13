#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include "include/list/XArrayList.h"
#include "include/util/Point.h"
#include "include/list/XArrayListDemo.h"

using namespace std;
namespace fs = std::filesystem;
vector<string> diffFiles;
string path = "ArrayTest/test/input/";
string path_output = "ArrayTest/test/output/";
string path_expected = "ArrayTest/test/expected/";
string path_logFile = "ArrayTest/test/log/";
int diffCount = 0;

struct Command {
    string operation;
    string value;
    int index;
    Command() : operation(""), value(""), index(-1) {}
};

void compareFile(const string& filename1, const string& filename2) {
    ifstream file1(filename1);
    ifstream file2(filename2);
    bool match = 1;

    string line1, line2;
    while (getline(file1, line1) && getline(file2, line2)) {
        if (line1 != line2) {
            cout << "Không khớp: " << line1 << " != " << line2 << endl;
            match = 0;
        }
    }

    if (getline(file1, line1) || getline(file2, line2)) {
        cout << "Số dòng không khớp" << endl;
        match = 0;
    }

    if (match) {
        cout << "Khớp" << endl;
    } else {
        diffFiles.push_back(filename1);
        diffCount++;
    }

}

Command parseCommand(const string& line) {
    Command cmd;
    istringstream iss(line);
    iss >> cmd.operation;

    if (cmd.operation == "ADD" || cmd.operation == "REMOVE" || 
        cmd.operation == "GET" || cmd.operation == "GETINDEX" || 
        cmd.operation == "ISCONTAIN") {
        string temp;
        while (iss >> temp) {
            if (temp.substr(0, 6) == "value:") {
                if (temp.substr(6, 5) == "Point") {
                    cmd.value = temp.substr(6);
                    iss >> temp;
                    cmd.value += " " + temp;
                    iss >> temp;
                    cmd.value += " " + temp;
                } else {
                    cmd.value = temp.substr(6);
                }
            } else if (temp.substr(0, 3) == "at:") {
                cmd.index = stoi(temp.substr(3));
            }
        }
    }

    return cmd;
}

template <typename T>
T parseValue(const string& value) {
    if constexpr (is_same_v<T, int>) {
        return stoi(value);
    } else if constexpr (is_same_v<T, float>) {
        return stof(value);
    } else if constexpr (is_same_v<T, string>) {
        return value;
    } else if constexpr (is_same_v<T, char>) {
        return value[0];
    } else if constexpr (is_same_v<T, Point>) {
        regex point_regex(R"(Point\(x=([\d.]+),\s*y=([\d.]+),\s*z=([\d.]+)\))");
        smatch match;
        if (regex_search(value, match, point_regex)) {
            float x = stof(match[1].str());
            float y = stof(match[2].str());
            float z = stof(match[3].str());
            return Point(x, y, z);
        } else {
            throw runtime_error("Invalid Point format: " + value);
        }
    } else {
        throw runtime_error("Unsupported type for parseValue");
    }
}

template <typename T>
void printListState(const XArrayList<T>& list, const string& operation) {
    cout << "List state " << operation << ": \n" ; 
}

template <typename T>
void processCommands(const vector<Command>& commands, const string& dataType) {
    try
    {
        XArrayList<T> list;

        for (const auto& cmd : commands) {
            printListState(list, "before " + cmd.operation + " " + cmd.value + (cmd.index != -1 ? " at " + to_string(cmd.index) : ""));
            list.println();
            if (cmd.operation == "ADD") {
                if (!cmd.value.empty()) {
                    if (cmd.index != -1) {
                        // ADD value:X at:Y
                        list.add(cmd.index, parseValue<T>(cmd.value));
                    } else {
                        // ADD value:X
                        list.add(parseValue<T>(cmd.value));
                    }
                }
            } else if (cmd.operation == "REMOVE") {
                if (cmd.index != -1) {
                    // REMOVE at:X
                    list.removeAt(cmd.index);
                } else if (!cmd.value.empty()) {
                    // REMOVE value:X
                    list.removeItem(parseValue<T>(cmd.value));
                }
            } else if (cmd.operation == "GET") {
                if (cmd.index != -1) {
                    // GET at:X
                    cout << list.get(cmd.index) << endl;
                }
            } else if (cmd.operation == "GETINDEX") {
                if (!cmd.value.empty()) {
                    // GETINDEX value:X
                    cout << list.indexOf(parseValue<T>(cmd.value)) << endl;
                }
            } else if (cmd.operation == "ISCONTAIN") {
                if (!cmd.value.empty()) {
                    // ISCONTAIN value:X
                    cout << (list.contains(parseValue<T>(cmd.value)) ? "true" : "false") << endl;
                }
            } else if (cmd.operation == "ISEMPTY") {
                cout << (list.empty() ? "true" : "false") << endl;
            } else if (cmd.operation == "GETSIZE") {
                cout << list.size() << endl;
            } else if (cmd.operation == "CLEAR") {
                list.clear();
            }
            printListState(list, "after " + cmd.operation);
            list.println();
        }
    }
    catch(const std::exception& e)
    {
        cout << e.what() << '\n';
    }
    
    
}

void processInputFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Không thể mở file: " << filename << endl;
        return;
    }

    string dataType;
    vector<Command> commands;

    string line;
    while (getline(file, line)) {
        if (line.substr(0, 5) == "TYPE:") {
            dataType = line.substr(5);
        } else if (!line.empty()) {
            commands.push_back(parseCommand(line));
        }
    }

    if (dataType == "int") {
        processCommands<int>(commands, dataType);
    } else if (dataType == "float") {
        processCommands<float>(commands, dataType);
    } else if (dataType == "str") {
        processCommands<string>(commands, dataType);
    } else if (dataType == "chr") {
        processCommands<char>(commands, dataType);
    } else if (dataType == "Point") {
        processCommands<Point>(commands, dataType);
    } else {
        cerr << "Không hỗ trợ kiểu dữ liệu: " << dataType << endl;
    }
}

void printUsage() {
    cout << "Cách sử dụng:" << endl;
    cout << "  a.exe            : Chạy tất cả các file input" << endl;
    cout << "  a.exe <N>        : Chạy file inputN.txt" << endl;
    cout << "  a.exe <N> <M>    : Chạy từ file inputN.txt đến inputM.txt" << endl;
}

string getNuminStr(string str) {
    string num = "";
    for (int i = 0; i < str.size(); i++) {
        if (isdigit(str[i])) {
            num += str[i];
        }
    }
    return num;
}

int main(int argc, char* argv[]) {
    xlistDemo1();
    xlistDemo2();
    xlistDemo3();
    xlistDemo4();
    return 0;
}