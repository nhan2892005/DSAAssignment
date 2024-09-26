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
#include <regex>

using namespace std;
namespace fs = std::filesystem;
int num_task = 16;

vector<vector<string>> expected_task (num_task, vector<string>(50, ""));
vector<vector<string>> output_task (num_task, vector<string>(50, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/Array/ArrayTestLog_Compare.txt";
    fstream file(log_file);
    if (!file.is_open()) {
        std::cout << "Cannot open file" << std::endl;
        return;
    }
    
    streambuf* stream_buffer_cout = cout.rdbuf();
    cout.rdbuf(file.rdbuf());

    fstream file1(filename1);
    fstream file2(filename2);

    // Store content of tasks in vector
    string line;
    int num = 1;
    int num_line = 0;
    while (getline(file1, line)) {
        // if line contains "Task", store it in expected_task
        if (regex_match(line, regex("Task [0-9]+.*"))) {
            num_line = 0;
            // Extract numbers from the line
            regex number_regex("[0-9]+");
            auto numbers_begin = sregex_iterator(line.begin(), line.end(), number_regex);
            auto numbers_end = sregex_iterator();

            for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
                std::smatch match = *i;
                num = std::stoi(match.str());
                break;  
            }
        }
        expected_task[num - 1][num_line] += line + '\n';
        num_line++;
    }

    num = 1;
    num_line = 0;
    while (getline(file2, line)) {
        // if line contains "Task", store it in output_task
        if (regex_match(line, regex("Task [0-9]+.*"))) {
            num_line = 0;
            // Extract numbers from the line
            regex number_regex("[0-9]+");
            auto numbers_begin = sregex_iterator(line.begin(), line.end(), number_regex);
            auto numbers_end = sregex_iterator();

            for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
                std::smatch match = *i;
                num = std::stoi(match.str());
                break;  
            }
        }
        output_task[num - 1][num_line] += line + '\n';
        num_line++;
    }

    // Compare content of tasks
    for (int task = 1; task < num_task; task++) {
        int count_diff = 0;
        if (output_task[task][0] == "") continue;
        doTasks.push_back(task);
        for (int line = 0; line < expected_task[task].size(); line++) {
            if (expected_task[task][line] != output_task[task][line]) {
                count_diff++;
                cout << "Task " << task << " - Line " << line << ":\n";
                cout << "Expected: " << expected_task[task][line];
                cout << "Output: " << output_task[task][line];
            }
        }
        if (count_diff > 0) {
            diffTasks.push_back(task);
        }
    }
    
    cout.rdbuf(stream_buffer_cout);
    float prop = (float)diffTasks.size() / (float)doTasks.size();
    if (prop == 0) {
        std::cout << "All tasks are correct" << std::endl;
    } else {
        std::cout << "Number of tasks to do: " << doTasks.size() << std::endl;
        std::cout << "Number of tasks with differences: " << diffTasks.size() << std::endl;
        std::cout << "Pass: " << 1 - prop << std::endl;
        for (int i = 0; i < diffTasks.size(); i++) {
            std::cout << "Task " << diffTasks[i] << std::endl;
        }
    }
}

enum CommandType {
    ADD, ADD_AT, REMOVE, REMOVE_AT, GET, SIZE, CLEAR, CONTAINS, INDEX_OF, EMPTY, TO_STRING, PRINTLN
};

// * Strategy Test: 
// * 1. Test Constructor, Copy Constructor, Assignment Operator
// * 2. Test add, addAt, remove, removeAt
// * 3. Test get, size, clear, contains, indexOf, empty
// * 4. Test with pointer
// * 4. Test with out of range index
// * 5. Test with Iterator

template <class T>
using Vector = XArrayList<T>;

void test1() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    // Copy Constructor
    std::cout << "Copy Constructor" << std::endl;
    Vector<int> list2(list);
    std::cout<< "List 2" << std::endl;
    list2.println();
    std::cout << '\n';

    std::cout << "List 2 after add" << std::endl;
    list2.add(50);
    list2.add(60);
    list2.println();
    std::cout << '\n';

    // Assignment Operator
    std::cout << "Assignment Operator" << std::endl;
    Vector<int> list3 = list;
    std::cout << "List 3" << std::endl;
    list3.println();
    std::cout << '\n';

    std::cout << "List 3 after add" << std::endl;
    list3.add(100);
    list3.add(200);
    list3.add(300);
    list3.println();
    std::cout << '\n';

    std::cout << "Summary" << std::endl;
    list.println();
    list2.println();
    list3.println();
}

void test2() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    list.add(5, 100);
    std::cout << "Added 100 at index 5" << std::endl;
    list.println();
    std::cout << '\n';

    list.add(0, 200);
    std::cout << "Added 200 at index 0" << std::endl;
    list.println();
    std::cout << '\n';

    list.add(list.size() - 1, 300);
    std::cout << "Added 300 at index 12" << std::endl;
    list.println();
    std::cout << '\n';
}

void test3() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';


    // Remove at middle
    std::cout << "Removed item at index 5: ";
    std::cout << list.removeAt(5) << std::endl;
    list.println();
    std::cout << '\n';

    // Remove at start
    std::cout << "Removed item at index 0";
    std::cout << list.removeAt(0) << std::endl;
    list.println();
    std::cout << '\n';

    // Remove at end
    int tmp = list.removeAt(list.size() - 1);
    std::cout << "Removed item at index 8";
    std::cout << "Removed item: " << tmp << std::endl;
    list.println();
    std::cout << '\n';
}

void test4() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    // Remove item
    bool can_remove = list.removeItem(5);
    std::cout << (can_remove ? "Removed item 5" : "Cannot remove item 5") << std::endl;
    list.println();
    std::cout << '\n';

    can_remove = list.removeItem(0);
    std::cout << (can_remove ? "Removed item 0" : "Cannot remove item 0") << std::endl;
    list.println();
    std::cout << '\n';

    can_remove = list.removeItem(15);
    std::cout << (can_remove ? "Removed item 15" : "Cannot remove item 15") << std::endl;
    list.println();
    std::cout << '\n';
}

void test5() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    std::cout << "Item at index 5: " << list.get(5) << std::endl;
    std::cout << "Item at index 0: " << list.get(0) << std::endl;
    std::cout << "Item at index 9: " << list.get(9) << std::endl;
}

void test6() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    std::cout << "Size: " << list.size() << std::endl;
    list.clear();
    std::cout << "Cleared list" << std::endl;
    list.println();
    std::cout << "Size: " << list.size() << std::endl;
}

void test7() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    std::cout << (list.contains(5) ? "List contain 5" : "List doesn't contain 5") << std::endl;
    std::cout << "at index: " << list.indexOf(5) << std::endl;

    std::cout << (list.contains(15) ? "List contain 15" : "List doesn't contain 15") << std::endl;
    std::cout << "at index: " << list.indexOf(15) << std::endl;
}

void test8() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
    list.clear();
    std::cout << "Cleared list" << std::endl;
    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
}

void test9() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    std::cout << "List: " << list.toString() << std::endl;
}

// * test with pointer
// print function pointer
string print(int*& item) {
    return to_string(*item);
}

void test10() {
    // with out equal function and print function
    Vector<int*> list(&Vector<int*>::free);
    std::cout << "Created list with int*" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(new int(i));
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    std::cout << "Item at index 5: " << *list.get(5) << std::endl;
    std::cout << "Item at index 0: " << *list.get(0) << std::endl;
    std::cout << "Item at index 9: " << *list.get(9) << std::endl;
}

// Continue with pointer
// define equal function
bool equal(int*& lhs, int*& rhs) {
    return *lhs == *rhs;
}
void test11() {
    // with print function
    Vector<int*> list(&Vector<int*>::free);
    std::cout << "Created list with int*" << std::endl;
    list.println(&print);
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(new int(i));
    }
    std::cout << "Added 10 items" << std::endl;
    list.println(&print);
    std::cout << '\n';

    std::cout << "Size: " << list.size() << std::endl;
    list.clear();
    std::cout << "Cleared list" << std::endl;
    list.println(&print);
    std::cout << "Size: " << list.size() << std::endl;
}

void test12() {
    // with equal function
    Vector<int*> list(&Vector<int*>::free, &equal);
    std::cout << "Created list with int*" << std::endl;
    list.println(&print);
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(new int(i));
    }
    std::cout << "Added 10 items" << std::endl;
    list.println(&print);
    std::cout << '\n';

    std::cout << "Item at index 5: " << *list.get(5) << std::endl;
    std::cout << "Item at index 0: " << *list.get(0) << std::endl;
    std::cout << "Item at index 9: " << *list.get(9) << std::endl;
}

// continue with pointer
// test all function for list
void test13() {
    // with equal function
    Vector<int*> list(&Vector<int*>::free, &equal);
    std::cout << "Created list with int*" << std::endl;
    list.println(&print);
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(new int(i));
    }
    std::cout << "Added 10 items" << std::endl;
    list.println(&print);
    std::cout << '\n';

    list.add(5, new int(100));
    std::cout << "Added 100 at index 5" << std::endl;
    list.println(&print);
    std::cout << '\n';

    list.add(0, new int(200));
    std::cout << "Added 200 at head" << std::endl;
    list.println(&print);
    std::cout << '\n';

    list.add(list.size() - 1, new int(300));
    std::cout << "Added 300 at tail" << std::endl;
    list.println(&print);
    std::cout << '\n';

    std::cout << "Removed item at index 5: ";
    int* tmp = list.removeAt(5);
    std::cout << *tmp << std::endl;
    delete tmp;
    list.println(&print);
    std::cout << '\n';

    std::cout << "Removed item at index 0: ";
    tmp = list.removeAt(0);
    std::cout << *tmp << std::endl;
    delete tmp;
    list.println(&print);
    std::cout << '\n';

    tmp = list.removeAt(list.size() - 1);
    std::cout << "Removed item at tail: ";
    std::cout << *tmp << std::endl;
    delete tmp;
    list.println(&print);
    std::cout << '\n';

    int* p = new int(5);
    std::cout << (list.contains(p) ? "List contain 5" : "List doesn't contain 5") << std::endl;
    std::cout << "at index: " << list.indexOf(p) << std::endl;

    delete p;
    p = new int(15);
    std::cout << (list.contains(p) ? "List contain 15" : "List doesn't contain 15") << std::endl;
    std::cout << "at index: " << list.indexOf(p) << std::endl;
    delete p;

    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
    list.clear();
    std::cout << "Cleared list" << std::endl;
    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
}

string Point2str(Point& point) {
    stringstream os;
    os << "(" << point.getX() << ", " << point.getY() << ", " << point.getZ() << ")";
    return os.str();
}
// test with class Point
void test14() {
    Vector<Point> list(0, &Point::pointEQ);
    std::cout << "Created list with Point" << std::endl;
    list.println();
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(Point(1, i, i * i));
    }
    std::cout << "Added 10 items" << std::endl;
    list.println();
    std::cout << '\n';

    // Add at head
    std::cout << "Added 100 at head" << std::endl;
    list.add(0, Point(1, 100, 100 * 100));
    list.println();
    std::cout << '\n';
    // Add at tail
    std::cout << "Added 200 at tail" << std::endl;
    list.add(list.size() - 1, Point(1, 200, 200 * 200));
    list.println();
    std::cout << '\n';
    // Add at middle
    std::cout << "Added 300 at index 5" << std::endl;
    list.add(5, Point(1, 300, 300 * 300));
    list.println();
    std::cout << '\n';

    // Remove at head
    list.removeAt(0);
    std::cout << "Removed item at head" << std::endl;
    list.println();
    std::cout << '\n';
    // Remove at tail
    list.removeAt(list.size() - 1);
    std::cout << "Removed item at tail" << std::endl;
    list.println();
    std::cout << '\n';
    // Remove at middle
    list.removeAt(5);
    std::cout << "Removed item at index 5" << std::endl;
    list.println();
    std::cout << '\n';

    // Remove item
    bool can_remove = list.removeItem(Point(1, 5, 25));
    std::cout << (can_remove ? "Removed item (1, 5, 25)" : "Cannot remove item (1, 5, 25)") << std::endl;
    list.println();

    can_remove = list.removeItem(Point(1, 0, 0));
    std::cout << (can_remove ? "Removed item (1, 0, 0)" : "Cannot remove item (1, 0, 0)") << std::endl;
    list.println();

    can_remove = list.removeItem(Point(1, 15, 225));
    std::cout << (can_remove ? "Removed item (1, 15, 225)" : "Cannot remove item (1, 15, 225)") << std::endl;
    list.println();
    std::cout << '\n';

    // Get item
    std::cout << "Item at index 5: " << Point2str(list.get(5)) << std::endl;
    std::cout << "Item at index 0: " << Point2str(list.get(0)) << std::endl;

    // Contains
    Point p(1, 5, 25);
    std::cout << (list.contains(p) ? "List contain (1, 5, 25)" : "List doesn't contain (1, 5, 25)") << std::endl;
    std::cout << "at index: " << list.indexOf(p) << std::endl;

    Point p1(1, 15, 225);
    std::cout << (list.contains(p1) ? "List contain (1, 15, 225)" : "List doesn't contain (1, 15, 225)") << std::endl;
    std::cout << "at index: " << list.indexOf(p1) << std::endl;

    // Index of
    std::cout << "Index of (1, 5, 25): " << list.indexOf(p) << std::endl;
    std::cout << "Index of (1, 15, 225): " << list.indexOf(p1) << std::endl;

    // Empty
    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
    list.clear();
    std::cout << "Cleared list" << std::endl;
    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
}

// Test with Point*

bool PointPtrEQ(Point*& lhs, Point*& rhs) {
    return Point::pointEQ(*lhs, *rhs);
}

string PointPtr2str(Point*& point) {
    return Point2str(*point);
}

void removePointPtr(Point* point) {
    delete point;
}

void test15() {
    Vector<Point*> list(&Vector<Point*>::free, &PointPtrEQ);
    std::cout << "Created list with Point*" << std::endl;
    list.println(&PointPtr2str);
    std::cout << '\n';

    for (int i = 0; i < 10; i++) {
        list.add(new Point(1, i, i * i));
    }
    std::cout << "Added 10 items" << std::endl;
    list.println(&PointPtr2str);
    std::cout << '\n';

    // Add at head
    std::cout << "Added 100 at head" << std::endl;
    list.add(0, new Point(1, 100, 100 * 100));
    list.println(&PointPtr2str);
    std::cout << '\n';
    // Add at tail
    std::cout << "Added 200 at tail" << std::endl;
    list.add(list.size() - 1, new Point(1, 200, 200 * 200));
    list.println(&PointPtr2str);
    std::cout << '\n';
    // Add at middle
    std::cout << "Added 300 at index 5" << std::endl;
    list.add(5, new Point(1, 300, 300 * 300));
    list.println(&PointPtr2str);
    std::cout << '\n';

    // Remove at head
    Point* tmp = list.removeAt(0);
    std::cout << "Removed item at head" << std::endl;
    list.println(&PointPtr2str);
    delete tmp;
    std::cout << '\n';
    // Remove at tail
    tmp = list.removeAt(list.size() - 1);
    std::cout << "Removed item at tail" << std::endl;
    list.println(&PointPtr2str);
    delete tmp;
    std::cout << '\n';
    // Remove at middle
    tmp = list.removeAt(5);
    std::cout << "Removed item at index 5" << std::endl;
    list.println(&PointPtr2str);
    delete tmp;
    std::cout << '\n';

    // Remove item
    Point* p = new Point(1, 5, 25);
    bool can_remove = list.removeItem(p, &removePointPtr);
    std::cout << (can_remove ? "Removed item (1, 5, 25)" : "Cannot remove item") << std::endl;
    list.println(&PointPtr2str);
    if (!can_remove) {
        delete p;
    }

    p = new Point(1, 0, 0);
    can_remove = list.removeItem(p, &removePointPtr);
    std::cout << (can_remove ? "Removed item (1, 0, 0)" : "Cannot remove item") << std::endl;
    list.println(&PointPtr2str);
    if (!can_remove) {
        delete p;
    }

    p = new Point(1, 15, 225);
    can_remove = list.removeItem(p, &removePointPtr);
    std::cout << (can_remove ? "Removed item (1, 15, 225)" : "Cannot remove item") << std::endl;
    list.println(&PointPtr2str);
    std::cout << '\n';
    if (!can_remove) {
        delete p;
    }

    // Get item
    std::cout << "Item at index 5: " << PointPtr2str(list.get(5)) << std::endl;
    std::cout << "Item at index 0: " << PointPtr2str(list.get(0)) << std::endl;

    // Contains
    p = new Point(1, 5, 25);
    std::cout << (list.contains(p) ? "List contain (1, 5, 25)" : "List doesn't contain (1, 5, 25)") << std::endl;
    std::cout << "at index: " << list.indexOf(p) << std::endl;
    delete p;

    p = new Point(1, 15, 225);
    std::cout << (list.contains(p) ? "List contain (1, 15, 225)" : "List doesn't contain (1, 15, 225)") << std::endl;
    std::cout << "at index: " << list.indexOf(p) << std::endl;
    delete p;

    // Empty
    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
    list.clear();
    std::cout << "Cleared list" << std::endl;
    std::cout << (list.empty() ? "List is empty" : "List is not empty") << std::endl;
}

// Out of range

void test16() {
    Vector<int> list;
    std::cout << "Created list" << std::endl;
    list.println();
    std::cout << '\n';
    std::cout << "Current Size: " << list.size() << std::endl;

    try {
        std::cout << "Add at index 1" << std::endl;
        list.add(1, 100);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }

    try {
        std::cout << "removeAt index 1" << std::endl;
        list.removeAt(1);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }

    try {
        std::cout << "Get index 1" << std::endl;
        list.get(1);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }

    try {
        std::cout << "Get Index of 100" << std::endl;
        list.get(list.indexOf(100));
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }

    try {
        std::cout << "Remove at index -1" << std::endl;
        list.removeAt(-1);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }

    for (int i = 0; i < 10; i++) {
        list.add(i);
    }

    try {
        std::cout << "Remove at index 10" << std::endl;
        list.removeAt(10);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }   

    try {
        std::cout << "Remove at index 15" << std::endl;
        list.removeAt(15);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }

    try {
        std::cout << "Remove at -1" << std::endl;
        list.removeAt(-1);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range" << std::endl;
    }
}



void printUsage() {
    std::cout << "Usage: exe_file [OPTIONS] [TASK]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  ?help: show help" << std::endl;
    std::cout << "  demo: run demo" << std::endl;
    std::cout << "  !test: run all test" << std::endl;
    std::cout << "  !test [task]: run specific test" << std::endl;
    std::cout << "  !test [start_task] [end_task] : run test from start_task to end_task" << std::endl;
    std::cout << "This test has 15 tasks" << std::endl;
}

void runDemo() {
    xlistDemo1();
    xlistDemo2();
    xlistDemo3();
    xlistDemo4();
}

// pointer function to store 15 test
void (*testFuncs[])() = {
    test1, 
    test2, 
    test3, 
    test4, 
    test5, 
    test6, 
    test7, 
    test8, 
    test9, 
    test10, 
    test11, 
    test12, 
    test13, 
    test14, 
    test15,
    test16
};

// ! NOTES: in function removeItem from original source
// * Param removeItemData in removeItem (line 40) 
// *             doesn't use pass by reference
// *             => can't free pointer 
int main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (string(argv[1]) == "?help") {
            printUsage();
            return 0;
        }
        else
        if (string(argv[1]) == "demo") {
            runDemo();
            return 0;
        }
        else if (string(argv[1]) == "test")
        {
            string folder = "TestLog/Array";
            string path = "ArrayTestLog_NhanOutput.txt";
            string output = "ArrayTestLog_YourOutput.txt";
            fstream file(folder + "/" + output);
            if (!file.is_open()) {
                fs::create_directory(folder);
                std::cout << "Create folder " << fs::absolute(folder) << std::endl;
                file.open(folder + "/" + output);
            }
            //change cout to file
            streambuf* stream_buffer_cout = cout.rdbuf();
            cout.rdbuf(file.rdbuf());

            if (string(argv[1]) == "test") {
                if (argc == 2) {
                    for (int i = 0; i < num_task; i++) {
                        std::cout << "Task " << i + 1 << "---------------------------------------------------" <<std::endl;
                        testFuncs[i]();
                    }
                } else
                if (argc == 3) {
                    int task = stoi(argv[2]);
                    if (task >= 1 && task <= num_task) {
                        std::cout << "Task " << task << "---------------------------------------------------" <<std::endl;
                        testFuncs[task - 1]();
                    } else {
                        std::cout << "Task not found" << std::endl;
                    }
                } else
                if (argc == 4) {
                    int start = stoi(argv[2]);
                    int end = stoi(argv[3]);
                    if (start >= 1 && start <= num_task && end >= 1 && end <= num_task && start <= end) {
                        for (int i = start - 1; i < end; i++) {
                            std::cout << "Task " << i + 1 << "---------------------------------------------------" <<std::endl;
                            testFuncs[i]();
                        }
                    } else {
                        std::cout << "Task not found or you enter error" << std::endl;
                    }
                }
            }

            // Restore cout
            cout.rdbuf(stream_buffer_cout);
            compareFile(folder + "/" + path, folder + "/" + output);
        }
    }
    return 0;
}