#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include "hash/xMap.h"
#include "util/Point.h"
#include "hash/xMapDemo.h"
#include <regex>
using namespace std;
namespace fs = std::filesystem;
int num_task = 10;

vector<vector<string>> expected_task (num_task, vector<string>(1000, ""));
vector<vector<string>> output_task (num_task, vector<string>(1000, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/HashMap/HashMapTestLog_Compare.txt";
    fstream file(log_file, ios::out);
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
    for (int task = 0; task < num_task; task++) {
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
    PUT, GET, REMOVE, CONTAINS, SIZE, CLEAR, EMPTY, KEYS, VALUES, CLASHES, TO_STRING
};

void printUsage() {
    std::cout << "Usage: exe_file [OPTIONS] [TASK]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  ?help: show help" << std::endl;
    std::cout << "  demo: run demo" << std::endl;
    std::cout << "  test: run all test" << std::endl;
    std::cout << "  test [task]: run specific test" << std::endl;
    std::cout << "  test [start_task] [end_task] : run test from start_task to end_task" << std::endl;
    std::cout << "This test has 15 tasks" << std::endl;
}

int fake_hash(int& key, int size) {
    return abs(key % size);
}

int hash_char(char& key, int size) {
    return (int)(key - 'a') % size;
}

void test1() {
    // test put, get, size, empty
    xMap<int, string> hash(&fake_hash, 1);
    int key[] = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
    string value[] = {"five", "fifteen", "twenty-five", "thirty-five", "forty-five", "fifty-five", "sixty-five", "seventy-five", "eighty-five", "ninety-five"};
    for (int i = 0; i < 10; i++) {
        hash.put(key[i], value[i]);
    }
    hash.println();
    for (int i = 0; i < 10; i++) {
        try {
            string val = hash.get(key[i]);
            if (val != value[i]) {
                cout << "Different value at key " << key[i] << endl;
                cout << "Expected: " << value[i] << endl;
                std::cout << "Test 1: Fail" << std::endl;
                return;
            }
        } catch (std::exception& e) {
            cout << e.what() << endl;
        }
    }
    if (hash.size() != 10) {
        cout << "Size is not correct" << endl;
        cout << "Expected: 10" << endl;
        std::cout << "Test 1: Fail" << std::endl;
        return;
    }
    if (hash.empty()) {
        cout << "Hash map is empty" << endl;
        cout << "Expected: not empty" << endl;
        std::cout << "Test 1: Fail" << std::endl;
        return;
    }  
}

void test2() {
    // test put, remove(Key), get
    // test with pair (char, string)

    xMap<char, string> hash(&hash_char, 0.5);
    char key[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    string value[] = {"apple", "banana", "carrot", "dog", "elephant", "fish", "goat", "horse", "ice-cream", "jelly"};
    for (int i = 0; i < 10; i++) {
        hash.put(key[i], value[i]);
    }
    hash.println();
    for (int i = 0; i < 10; i++) {
        try {
            string val = hash.get(key[i]);
            if (val != value[i]) {
                cout << "Different value at key " << key[i] << endl;
                cout << "Expected: " << value[i] << endl;
                std::cout << "Test 2: Fail" << std::endl;
                return;
            }
        } catch (std::exception& e) {
            cout << e.what() << endl;
        }
    }

    // add more key-value pairs
    hash.put('k', "kite");
    hash.put('l', "lion");
    hash.put('m', "monkey");
    hash.put('n', "nest");

    // add with key was existed
    hash.put('c', "cat");
    hash.put('d', "deer");
    hash.put('e', "elephant");

    cout << "After adding more key-value pairs" << endl;
    hash.println();

    // remove key 'c', 'd', 'e'
    hash.remove('c');
    hash.remove('d');
    hash.remove('e');

    cout << "After removing key 'c', 'd', 'e'" << endl;
    hash.println();

    // remove key not existed
    try {
        hash.remove('c');
        hash.remove('d');
        hash.remove('e');
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
}

void test3() {
    // test put, get, remove(Key), remove(Key, Value)
    xMap<int, string> hash(&fake_hash);
    int paying[] = {10000, 13000, 17000, 20000, 23000, 27000, 30000, 33000, 37000, 40000};
    string name[] = {"Alice", "Bob", "Charlie", "David", "Eva", "Frank", "Grace", "Helen", "Ivy", "Jack"};
    for (int i = 0; i < 10; i++) {
        hash.put(paying[i], name[i]);
    }
    hash.println();
    for (int i = 0; i < 10; i++) {
        try {
            string val = hash.get(paying[i]);
            if (val != name[i]) {
                cout << "Different value at key " << paying[i] << endl;
                cout << "Expected: " << name[i] << endl;
                std::cout << "Test 3: Fail" << std::endl;
                return;
            }
        } catch (std::exception& e) {
            cout << e.what() << endl;
        }
    }
    
    // remove key 10000, 13000, 17000
    string removedName = hash.remove(10000);
    if (removedName != string("Alice")) {
        cout << "Different value at key 10000" << endl;
        cout << "Expected: Alice" << endl;
        std::cout << "Test 3: Fail" << std::endl;
        return;
    } else {
        cout << "Remove key 10000: " << removedName << endl;
    }
    removedName = hash.remove(13000);
    if (removedName != string("Bob")) {
        cout << "Different value at key 13000" << endl;
        cout << "Expected: Bob" << endl;
        std::cout << "Test 3: Fail" << std::endl;
        return;
    } else {
        cout << "Remove key 13000: " << removedName << endl;
    }
    removedName = hash.remove(17000);
    if (removedName != string("Charlie")) {
        cout << "Different value at key 17000" << endl;
        cout << "Expected: Charlie" << endl;
        std::cout << "Test 3: Fail" << std::endl;
        return;
    } else {
        cout << "Remove key 17000: " << removedName << endl;
    }

    cout << "After removing key 10000, 13000, 17000" << endl;
    hash.println();

    // put with existed key
    string name_put = hash.put(20000, "Eva");
    if (name_put != "David") {
        cout << "Different value at key 20000" << endl;
        cout << "Expected: David" << endl;
        std::cout << "Test 3: Fail" << std::endl;
        return;
    } else {
        cout << "Put key 20000: " << name_put << endl;
    }
    name_put = hash.put(23000, "Frank");
    if (name_put != "Eva") {
        cout << "Different value at key 23000" << endl;
        cout << "Expected: Eva" << endl;
        std::cout << "Test 3: Fail" << std::endl;
        return;
    } else {
        cout << "Put key 23000: " << name_put << endl;
    }
    name_put = hash.put(27000, "Grace");
    if (name_put != "Frank") {
        cout << "Different value at key 27000" << endl;
        cout << "Expected: Frank" << endl;
        std::cout << "Test 3: Fail" << std::endl;
        return;
    } else {
        cout << "Put key 27000: " << name_put << endl;
    }

    cout << "After adding key 20000, 23000, 27000 with existed key" << endl;
    hash.println();

    // remove key 20000, 23000, 27000 with value "Eva", "Frank", "Grace"
    hash.remove(20000, "Eva");
    hash.remove(23000, "Frank");
    hash.remove(27000, "Grace");

    cout << "After removing key 20000, 23000, 27000 with value 'Eva', 'Frank', 'Grace'" << endl;
    hash.println();

    // remove key with value not existed
    try {
        hash.remove(30000, "Helen");
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
}

void test4() {
    // test put, containsKey(Key), containsValue(Value)
    xMap<int, string> hash(&fake_hash);
    int key[] = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
    string value[] = {"five", "fifteen", "twenty-five", "thirty-five", "forty-five", "fifty-five", "sixty-five", "seventy-five", "eighty-five", "ninety-five"};
    for (int i = 0; i < 10; i++) {
        hash.put(key[i], value[i]);
    }
    hash.println();

    // contains key
    if (!hash.containsKey(5)) {
        cout << "Key 5 is not existed" << endl;
        cout << "Expected: existed" << endl;
        std::cout << "Test 4: Fail" << std::endl;
        return;
    } else {
        cout << "Key 5 is existed" << endl;
    }

    // contains value
    if (!hash.containsValue("fifty-five")) {
        cout << "Value fifty-five is not existed" << endl;
        cout << "Expected: existed" << endl;
        std::cout << "Test 4: Fail" << std::endl;
        return;
    } else {
        cout << "Value fifty-five is existed" << endl;
    }

    // contains key not existed
    if (hash.containsKey(10)) {
        cout << "Key 10 is existed" << endl;
        cout << "Expected: not existed" << endl;
        std::cout << "Test 4: Fail" << std::endl;
        return;
    } else {
        cout << "Key 10 is not existed" << endl;
    }

    // contains value not existed
    if (hash.containsValue("ten")) {
        cout << "Value ten is existed" << endl;
        cout << "Expected: not existed" << endl;
        std::cout << "Test 4: Fail" << std::endl;
        return;
    } else {
        cout << "Value ten is not existed" << endl;
    }
}

int hash_date(string& date, int size) {
    int sum = 0;
    for (int i = 0; i < date.length(); i++) {
        sum += (int)(date[i] - '0');
    }
    return sum % size;
}

bool compare_consumer(int& a, int& b) {
    return a == b;
}

bool day_in_month(string& a, string& b) {
    return a.substr(6, 2) == b.substr(6, 2);
}

string get_day(string& date) {
    //20240101 -> 01/01/2024
    return date.substr(6, 2) + "/" + date.substr(4, 2) + "/" + date.substr(0, 4);
}

string generateDate() {
    string date = "";
    int year = 2024;
    int month = 1 + rand() % 12;
    int day = 1 + rand() % 31;
    if (month < 10) {
        date += "0";
    }
    date += to_string(month);
    if (day < 10) {
        date += "0";
    }
    date += to_string(day);
    date = to_string(year) + date;
    return date;
}

void test5() {
    // test put, remove(Key), remove(Key, Value), clear, size, empty, getCapacity
    xMap<string, int> hash(&hash_date, 0.8f, compare_consumer, 0, day_in_month);
    string date[] = {"20240101", "20240102", "20240103", "20240104", "20240105", "20240106", "20240107", "20240108", "20240109", "20240110"};
    int amount_consumer[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    for (int i = 0; i < 10; i++) {
        hash.put(date[i], amount_consumer[i]);
    }
    hash.println();

    // remove key 20240101, 20240102, 20240103
    int amount = hash.remove("20240101");
    if (amount != 100) {
        cout << "Different value at key 20240101" << endl;
        cout << "Expected: 100" << endl;
        std::cout << "Test 5: Fail" << std::endl;
        return;
    } else {
        cout << "Remove key 20240101: " << amount << endl;
    }

    amount = hash.remove("20240102");
    if (amount != 200) {
        cout << "Different value at key 20240102" << endl;
        cout << "Expected: 200" << endl;
        std::cout << "Test 5: Fail" << std::endl;
        return;
    } else {
        cout << "Remove key 20240102: " << amount << endl;
    }

    // remove key not existed
    try {
        hash.remove("20240101");
        hash.remove("20240102");
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    // remove key 20240103, 20240104, 20240105 with value 300, 400, 500
    hash.remove("20240103", 300);
    hash.remove("20240104", 400);
    hash.remove("20240105", 500);

    cout << "After removing key 20240103, 20240104, 20240105 with value 300, 400, 500" << endl;
    hash.println();

    // remove key with value not existed
    try {
        hash.remove("20240106", 600);
        cout << "Remove key 20240106 with value 600" << endl;
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Size: " << hash.size() << endl;
    cout << "Empty: " << hash.empty() << endl;

    hash.clear();
    cout << "After clear" << endl;
    hash.println();
    cout << "Size: " << hash.size() << endl;
    cout << "Empty: " << hash.empty() << endl;

    int capacity = hash.getCapacity();
    for (int i = 0; i < capacity; i++) {
        string date = generateDate();
        hash.put(date, i);
    }
    cout << "After adding more key-value pairs" << endl;
    hash.println();
    cout << "Size: " << hash.size() << endl;
    cout << "Empty: " << hash.empty() << endl;
    cout << "Real Load Factor: " << (float)hash.size() / (float)capacity << endl;
}

string read_number(int num) {
    int number = num;
    string result = "";
    if (number == 0) {
        return "zero";
    }
    if (number < 0) {
        result += "negative ";
        number = -number;
    }
    if (number >= 1000000000) {
        result += read_number(number / 1000000000) + " billion ";
        number %= 1000000000;
    }
    if (number >= 1000000) {
        result += read_number(number / 1000000) + " million ";
        number %= 1000000;
    }
    if (number >= 1000) {
        result += read_number(number / 1000) + " thousand ";
        number %= 1000;
    }
    if (number >= 100) {
        result += read_number(number / 100) + " hundred ";
        number %= 100;
    }
    if (number >= 20) {
        switch (number / 10) {
            case 2: result += "twenty "; break;
            case 3: result += "thirty "; break;
            case 4: result += "forty "; break;
            case 5: result += "fifty "; break;
            case 6: result += "sixty "; break;
            case 7: result += "seventy "; break;
            case 8: result += "eighty "; break;
            case 9: result += "ninety "; break;
        }
        number %= 10;
    }
    if (number >= 10) {
        switch (number) {
            case 10: result += "ten"; break;
            case 11: result += "eleven"; break;
            case 12: result += "twelve"; break;
            case 13: result += "thirteen"; break;
            case 14: result += "fourteen"; break;
            case 15: result += "fifteen"; break;
            case 16: result += "sixteen"; break;
            case 17: result += "seventeen"; break;
            case 18: result += "eighteen"; break;
            case 19: result += "nineteen"; break;
        }
        number = 0;
    }
    if (number > 0) {
        switch (number) {
            case 1: result += "one"; break;
            case 2: result += "two"; break;
            case 3: result += "three"; break;
            case 4: result += "four"; break;
            case 5: result += "five"; break;
            case 6: result += "six"; break;
            case 7: result += "seven"; break;
            case 8: result += "eight"; break;
            case 9: result += "nine"; break;
        }
    }
    return result;
}

void test6() {
    // DLinkedList<K> keys();
    // DLinkedList<V> values();
    // DLinkedList<int> clashes();
    // test #collisions
    xMap<int, string> hash(&fake_hash, 0.5);
    for (int i = -10000; i < 10000; i++) {
        hash.put(i, read_number(i));
    }

    int size = hash.size();
    cout << "Size: " << size << endl;
    int capacity = hash.getCapacity();
    cout << "Capacity: " << hash.getCapacity() << endl;
    int load_factor = (float)size / (float)hash.getCapacity();
    cout << "Load Factor: " << load_factor << endl;
    if (load_factor > 0.5) {
        cout << "Load factor is greater than 0.5" << endl;
        cout << "Expected: less than 0.5" << endl;
        std::cout << "Test 6: Fail" << std::endl;
        return;
    }

    DLinkedList<int> keys = hash.keys();
    DLinkedList<string> values = hash.values();
    DLinkedList<int> clashes = hash.clashes();
    int max_clashes = 0;
    for (auto iter = clashes.begin(); iter != clashes.end(); iter++) {
        int item = *iter;
        if (item > max_clashes) {
            max_clashes = item;
        }
    }

    cout << "Max collisions: " << max_clashes << endl;
}

int hash_consumer_id(int& consumer_id, int size) {
    return consumer_id % size;
}

void test7() {
    // simulate a real case (database to store consumer information)
    // schema: consumer_id INT,
    //         consumer_name VARCHAR(255),
    //         consumer_dob DATE,
    //         consumer_address VARCHAR(255),
    //         consumer_phone VARCHAR(15),
    //         consumer_email VARCHAR(255)

    // hash function: hash_consumer_id(consumer_id, private_key) = consumer_id % 2809
    
    xMap<int, string> hash(&hash_consumer_id, 0.8);
    int consumer_id[] = {1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008};
    string consumer_name[] = {"Alice", "Bob", "Charlie", "David", "Eva", "Frank", "Grace", "Helen"};
    string consumer_dob[] = {"19900101", "19900202", "19900303", "19900404", "19900505", "19900606", "19900707", "19900808"};
    string consumer_address[] = {"1st Street", "2nd Street", "3rd Street", "4th Street", "5th Street", "6th Street", "7th Street", "8th Street"};
    string consumer_phone[] = {"1234567890", "2345678901", "3456789012", "4567890123", "5678901234", "6789012345", "7890123456", "8901234567"};
    string consumer_email[] = {"abc@gmail.com", "fgh@gmail.com", "None", "ghx@gmail.com", "None", "None", "None", "None"};
    for (int i = 0; i < 8; i++) {
        hash.put(consumer_id[i], consumer_name[i] + " " + consumer_dob[i] + " " + consumer_address[i] + " " + consumer_phone[i] + " " + consumer_email[i]);
    }
    hash.println();

    // get consumer information by consumer_id
    for (int i = 0; i < 8; i++) {
        try {
            string consumer_info = hash.get(consumer_id[i]);
            cout << "Consumer information with consumer_id " << consumer_id[i] << ": " << consumer_info << endl;
        } catch (std::exception& e) {
            cout << e.what() << endl;
        }
    }

    // remove consumer information by consumer_id
    string removed_consumer_info = hash.remove(1001);
    cout << "Remove consumer information with consumer_id 1001: " << removed_consumer_info << endl;
    removed_consumer_info = hash.remove(1002);
    cout << "Remove consumer information with consumer_id 1002: " << removed_consumer_info << endl;

    // update consumer information by consumer_id
    string updated_consumer_info = "Alice 19900101 1st Street 1234567890 wsx@gmail.com";
    string old_consumer_info = hash.put(1001, updated_consumer_info);
    cout << "Update consumer information with consumer_id 1001: " << old_consumer_info << endl;

    // add more consumer information
    hash.put(1009, "Ivy 19900909 9th Street 9999999999 None");
    hash.put(1010, "Jack 19901010 10th Street 1010101010 j97@gmailc.om");

    cout << "After adding more consumer information" << endl;
    hash.println();

    DLinkedList<int> keys = hash.keys();
    DLinkedList<string> values = hash.values();
    DLinkedList<int> clashes = hash.clashes();
    int max_clashes = 0;
    for (auto iter = clashes.begin(); iter != clashes.end(); iter++) {
        int item = *iter;
        if (item > max_clashes) {
            max_clashes = item;
        }
    }

    cout << "Keys: ";
    keys.println();

    cout << "Values: ";
    values.println();

    cout << "Clashes: ";
    clashes.println();

    cout << "Size: " << hash.size() << endl;
    cout << "Capacity: " << hash.getCapacity() << endl;
    cout << "Load Factor: " << (float)hash.size() / (float)hash.getCapacity() << endl;
    cout << "Max collisions: " << max_clashes << endl;

    // remove all consumer information
    hash.clear();
    cout << "After clear" << endl;
    hash.println();
}

void test8() {
    int count = 10000;
    //int count = 100;
    int *keys = genIntArray(count, 0, 1999999999);
    xmap<int, int*> hash(&xmap<int, int*>::simpleHash);
    for (int idx = 0; idx < count; idx++) {
        hash.put(keys[idx], 0);
    }
    //hash.println();

    // remove key
    for (int idx = 100; idx < 1000; idx++) {
        if (hash.containsKey(keys[idx])) {
            hash.remove(keys[idx]);
        }
    }

    DLinkedList<int> clashes = hash.clashes();
    int max = -1;
    for (DLinkedList<int>::Iterator it = clashes.begin(); it != clashes.end(); it++) {
        int item = *it;
        if (item > max) max = item;
    }

    cout << "table size: " << hash.getCapacity() << endl;
    cout << "current count: " << hash.size() << endl;
    cout << "real load factor: " << (float) hash.size() / hash.getCapacity() << endl;
    cout << "max #collisions: " << max << endl;
    cout << "Data was generated randomly => #collisions may be different" << endl;
    delete []keys;
}


void runDemo() {

    cout << "Demo 0: simpleMap" << endl;
    try {
        simpleMap();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 1: hashDemo1" << endl;
    try {
        hashDemo1();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 2: hashDemo2" << endl;
    try {
        hashDemo2();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 3: hashDemo3" << endl;
    try {
        hashDemo3();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 4: hashDemo4" << endl;
    try
    {
        hashDemo4();
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }
    

    cout << "Demo 5: hashDemo5" << endl;
    try {
        hashDemo5();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 6: hashDemo6" << endl;
    try {
        hashDemo6();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo 7: hashDemo7" << endl;
    try {
        hashDemo7();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }

    cout << "Demo: countryDemo" << endl;
    try {
        countryDemo();
    } catch (std::exception& e) {
        cout << e.what() << endl;
    }
}
//test with operator=, copy constructure
void test9() {
    // test with static type
    xMap<int, int> hash(&fake_hash, 0.5);
    for (int i = 0; i < 10; i++) {
        hash.put(i, i);
    }
    cout << "Created hash 1" << endl;
    hash.println();

    cout << "Created hash 2 by copy constructure" << endl;
    xMap<int, int> hash2 = hash;
    hash2.println();

    cout << "Created hash 3 by operator=" << endl;
    xMap<int, int> hash3(&fake_hash, 0.75);
    hash3 = hash;
    hash3.println();

    // change content of hash 1
    cout << "Change content of hash 1" << endl;
    for (int i = 0; i < 10; i++) {
        hash.put(i, i * 2);
    }
    cout << "hash 1" << endl;
    hash.println();
    cout << "hash 2" << endl;
    hash2.println();
    cout << "hash 3" << endl;
    hash3.println();

    // clear hash 2
    cout << "Clear hash 2" << endl;
    hash2.clear();
    hash2.println();
    cout << "hash 1" << endl;
    hash.println();
    cout << "hash 3" << endl;
    hash3.println();
}

void test10() {
    // test with dynamic type
    xmap<int*, int*> hashMap(
            &hashFunc,
            0.75,
            &valueEQ,
            &xmap<int*, int*>::freeValue,
            &keyEQ,
            &xmap<int*, int*>::freeKey
            );
    for (int i = 0; i < 10; i++) {
        int* key = new int(i);
        int* value = new int(i);
        hashMap.put(key, value);
    }
    cout << "Created hash 1" << endl;
    hashMap.println(&key2str, &value2str);

    cout << "Created hash 2 by copy constructure" << endl;
    xmap<int*, int*> hashMap2 = hashMap;
    hashMap2.println(&key2str, &value2str);

    cout << "Created hash 3 by operator=" << endl;
    xmap<int*, int*> hashMap3(&hashFunc);
    hashMap3 = hashMap;
    hashMap3.println(&key2str, &value2str);

    // change content of hash 1
    cout << "Change content of hash 1" << endl;
    for (int i = 0; i < 10; i++) {
        int* key = new int(i);
        int* value = new int(i * 2);
        int* oldValue = hashMap.put(key, value);
        if (oldValue != nullptr) {
            delete oldValue;
        }
        delete key;
    }

    cout << "hash 1" << endl;
    hashMap.println(&key2str, &value2str);
}
// pointer function to store 15 test
void (*testFuncs[])() = {
    test1, test2, test3, test4, test5, test6, test7, test8, test9, test10
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
            string file_name = "DemoLog.txt";
            fstream file_demo(file_name, ios::out);
            if (!file_demo.is_open()) {
                std::cout << "Cannot open file" << std::endl;
                return 0;
            } 
            streambuf* stream_buffer_cout = cout.rdbuf();
            cout.rdbuf(file_demo.rdbuf());

            runDemo();

            cout.rdbuf(stream_buffer_cout);
            return 0;
        }
        else if (string(argv[1]) == "test")
        {
            string folder = "TestLog/HashMap";
            string path = "HashMapTestLog_NhanOutput.txt";
            string output = "HashMapTestLog_YourOutput.txt";
            fstream file(folder + "/" + output, ios::out);
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