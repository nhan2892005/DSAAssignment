#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
using namespace std;

#include "graph/AbstractGraph.h"
#include "graph/DGraphModel.h"
#include "graph/UGraphModel.h"
#include "graph/DGraphDemo.h"
#include "graph/UGraphDemo.h"
#include "graph/TopoSorter.h"

using namespace std;
namespace fs = std::filesystem;
int num_task = 77;


vector<vector<string>> expected_task (num_task, vector<string>(1000, ""));
vector<vector<string>> output_task (num_task, vector<string>(1000, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/Graph/GraphTestLog_Compare.txt";
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
                cout << "Task " << task + 1 << " - Line " << line << ":\n";
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
            std::cout << "Task " << diffTasks[i] + 1 << std::endl;
        }
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
void test1() {
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('C', 'B');
    model.connect('C', 'D');

    cout << model.toString();
}

void test2() {
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('C', 'B');
    model.connect('C', 'D');

    cout << model.toString();
}

void test03()
{
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D', 'E'};
    for (int idx = 0; idx < 5; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B', 8);
    model.connect('B', 'D', 6);
    model.connect('C', 'B', 1);
    model.connect('C', 'D', 2);
    model.connect('E', 'A', 3);
    model.connect('E', 'B', 4);
    model.connect('E', 'C', 5);
    DLinkedList<char> out = model.getOutwardEdges('E');
    DLinkedList<char> in = model.getInwardEdges('B');
    cout << "getOutwardEdges : E: ";
    for (auto it = out.begin(); it != out.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model.toString();
}

void test04()
{
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D', 'E'};
    for (int idx = 0; idx < 5; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B', 8);
    model.connect('B', 'D', 6);
    model.connect('C', 'B', 1);
    model.connect('C', 'D', 2);
    model.connect('E', 'A', 3);
    model.connect('E', 'B', 4);
    model.connect('E', 'C', 5);
    model.connect('E', 'E', 5);
    DLinkedList<char> out = model.getOutwardEdges('E');
    DLinkedList<char> in = model.getInwardEdges('B');
    cout << "getOutwardEdges : E: ";
    for (auto it = out.begin(); it != out.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model.toString();
}

void test05()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[3] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(vertices, 4, edges, 3, &charComparator, &vertex2str);

    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        DLinkedList<char> out = model->getOutwardEdges('E');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "getOutwardEdges :E khong ton tai" << endl; // In ra thông báo lỗi
    }
    DLinkedList<char> in = model->getInwardEdges('B');

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model->toString();
    delete model;
}

void test06()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[3] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(vertices, 4, edges, 3, &charComparator, &vertex2str);

    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        DLinkedList<char> out = model->getOutwardEdges('E');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "getOutwardEdges :E khong ton tai" << endl; // In ra thông báo lỗi
    }
    DLinkedList<char> in = model->getInwardEdges('B');

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model->toString();
    delete model;
}

void test07()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[4] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(vertices, 4, edges, 4, &charComparator, &vertex2str);

    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->weight('A', 'D');
    }
    catch (const EdgeNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "AB khong ton tai" << endl; // In ra thông báo lỗi
    }
    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->weight('E', 'D');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "E khong ton tai" << endl; // In ra thông báo lỗi
    }

    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->weight('D', 'F');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "F khong ton tai" << endl; // In ra thông báo lỗi
    }
    cout << "AB : " << model->weight('A', 'B') << endl;
    cout << "CD : " << model->weight('C', 'D') << endl;

    DLinkedList<char> in = model->getInwardEdges('D');

    cout << "getInwardEdges : D: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model->toString();
    delete model;
}

void test08()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(vertices, 4, edges, 5, &charComparator, &vertex2str);

    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->weight('A', 'D');
    }
    catch (const EdgeNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "AB khong ton tai" << endl; // In ra thông báo lỗi
    }
    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->weight('E', 'D');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "E khong ton tai" << endl; // In ra thông báo lỗi
    }

    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->weight('D', 'F');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "F khong ton tai" << endl; // In ra thông báo lỗi
    }
    cout << "AA : " << model->weight('A', 'A') << endl;
    cout << "CD : " << model->weight('C', 'D') << endl;

    DLinkedList<char> in = model->getInwardEdges('D');

    cout << "getInwardEdges : D: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model->toString();
    delete model;
}

void test09()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(vertices, 4, edges, 5, &charComparator, &vertex2str);
    model->remove('A');
    model->remove('B');
    DLinkedList<char> in = model->getInwardEdges('D');

    cout << "getInwardEdges : D: ";
    for (auto it = in.begin(); it != in.end(); it++)
    {
        cout << *it << "-> ";
    }
    cout << "NULL\n";
    cout << model->toString();
    delete model;
}

void test10()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(vertices, 4, edges, 5, &charComparator, &vertex2str);
    model->remove('B');
    model->remove('D');
    cout << model->toString();
    delete model;
}

void test11()
{
    string name = "graph11";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(vertices, 4, edges, 5, &charComparator, &vertex2str);
    cout<<"Size: "<<model->size()<<endl;
    cout<<"Indegree: "<<model->inDegree('A')<<endl;
    cout<<"Outdegree: "<<model->outDegree('A')<<endl;
    model->remove('A');
    model->remove('B');
    model->remove('C');
    model->remove('D');
    cout<<"Size: "<<model->size()<<endl;
    try{
        model->inDegree('F');
    }
    catch(const VertexNotFoundException &e){
        cout<<"Error: "<<"F khong ton tai"<<endl;
    }
    try{
        model->outDegree('F');
    }
    catch(const VertexNotFoundException &e){
        cout<<"Error: "<<"F khong ton tai"<<endl;
    }
    cout << model->toString();
    delete model;
}


void test12()
{
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(vertices, 4, edges, 5, &charComparator, &vertex2str);
    cout<<"Size: "<<model->size()<<endl;
    cout<<"vertices: ";
    DLinkedList<char> a = model->vertices();
    for(auto it = a.begin(); it != a.end(); ++it){
        cout<<*it<<" ";
    }
    cout<<"\n";
    cout<<"Indegree: "<<model->inDegree('A')<<endl;
    cout<<"Outdegree: "<<model->outDegree('A')<<endl;
    model->remove('A');
    model->remove('B');
    model->remove('C');
    model->remove('D');
    cout<<"Size: "<<model->size()<<endl;
    try{
        model->inDegree('F');
    }
    catch(const VertexNotFoundException &e){
        cout<<"Error: "<<"F khong ton tai"<<endl;
    }
    try{
        model->outDegree('F');
    }
    catch(const VertexNotFoundException &e){
        cout<<"Error: "<<"F khong ton tai"<<endl;
    }
    cout << model->toString();
    delete model;
}
void test13()
    {
    char vertices[] = {'A', 'B', 'C', 'D','E','F','H','G'};

    // Định nghĩa các cạnh
    Edge<char> edges[6] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('B', 'B', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(vertices, 8, edges,6 , &charComparator, &vertex2str);
    cout<<"Size: "<<model->size()<<endl;
    cout<<"vertices: ";
    DLinkedList<char> a = model->vertices();
    for(auto it = a.begin(); it != a.end(); ++it){
        cout<<*it<<" ";
    }
    cout<<"\n";
    cout<<"Indegree H: "<<model->inDegree('H')<<endl;
    cout<<"Outdegree C: "<<model->outDegree('C')<<endl;
    model->remove('F');
    model->remove('B');
    model->remove('H');
    model->remove('G');
    cout<<"Size: "<<model->size()<<endl;
    cout << model->toString();
    delete model;
}


void test14()
{
    char vertices[] = {'A', 'B', 'C', 'D','E','F','H','G'};

    // Định nghĩa các cạnh
    Edge<char> edges[6] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('B', 'B', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(vertices, 8, edges,6 , &charComparator, &vertex2str);
    cout<<"Size: "<<model->size()<<endl;
    cout<<"vertices: ";
    DLinkedList<char> a = model->vertices();
    for(auto it = a.begin(); it != a.end(); ++it){
        cout<<*it<<" ";
    }
    cout<<"\n";
    cout<<"Indegree H: "<<model->inDegree('H')<<endl;
    cout<<"Outdegree C: "<<model->outDegree('C')<<endl;
    cout<<"connected AA : "<<model->connected('A','A')<<endl;
    cout<<"connected AH : "<<model->connected('A','H')<<endl;
    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->connected('A','X');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "X khong ton tai" << endl; // In ra thông báo lỗi
    }
    try
    {
        // Gọi một phương thức có thể ném ngoại lệ
        model->connected('V','Q');
    }
    catch (const VertexNotFoundException &e)
    {
        // Xử lý ngoại lệ nếu đỉnh không tìm thấy
        cout << "Error: " << "V khong ton tai" << endl; // In ra thông báo lỗi
    }
    cout<<"Size: "<<model->size()<<endl;
    cout << model->toString();
    delete model;
}


void test15()
{
    char vertices[] = {'A', 'B', 'C', 'D','E','F','H','G'};

    // Định nghĩa các cạnh
    Edge<char> edges[6] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('A', 'A', 10),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('B', 'B', 2.0),
        Edge<char>('C', 'D', 3.2),
        Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(vertices, 8, edges, 6 , &charComparator, &vertex2str);
    cout<<"Size: "<<model->size()<<endl;
    cout<<"Indegree H: "<<model->inDegree('H')<<endl;
    cout<<"Outdegree C: "<<model->outDegree('C')<<endl;
    cout<<"contains C : "<<model->contains('C')<<endl;
    cout<<"contains J : "<<model->contains('J')<<endl;
    cout<<"Size: "<<model->size()<<endl;

    cout << model->toString();
    model->clear();
    cout << model->toString();
    delete model;
}

void test16() {
    // test update weight
    char vertices[] = {'A', 'B', 'C', 'D'};
    Edge<char> edges[3] = {
        Edge<char>('A', 'B', 1.5),
        Edge<char>('B', 'C', 2.0),
        Edge<char>('C', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(vertices, 4, edges, 3, &charComparator, &vertex2str);
    cout << "Before update weight: " << model->toString() << endl;
    model->connect('A', 'B', 2.5);
    model->connect('B', 'C', 3.0);
    model->connect('C', 'D', 4.2);
    cout << "After update weight: " << model->toString() << endl;
    delete model;
}

class Animal {
public:
    string name;
    int age;
    Animal(string name, int age) {
        this->name = name;
        this->age = age;
    }
    virtual ~Animal() {}
    virtual string toString() = 0;
};

class Dog : public Animal {
public:
    Dog(string name, int age) : Animal(name, age) {}
    string bark() {
        return "Gâu gâu";
    }
    string toString() {
        return "Dog: " + name + " - " + to_string(age);
    }
    bool operator==(const Dog& other) {
        return this->name == other.name && this->age == other.age;
    }
};

class Cat : public Animal {
public:
    Cat(string name, int age) : Animal(name, age) {}
    string meow() {
        return "Meo meo";
    }
    string toString() {
        return "Cat: " + name + " - " + to_string(age);
    }
    bool operator==(const Cat& other) {
        return this->name == other.name && this->age == other.age;
    }
};

class Mouse : public Animal {
public:
    Mouse(string name, int age) : Animal(name, age) {}
    string squeak() {
        return "Chit chit";
    }
    string toString() {
        return "Mouse: " + name + " - " + to_string(age);
    }
    bool operator==(const Mouse& other) {
        return this->name == other.name && this->age == other.age;
    }
};

string animal2str(Animal*& animal) {
    return animal->toString();
}

bool animalComparator(Animal*& animal1, Animal*& animal2) {
    return animal1->name == animal2->name && animal1->age == animal2->age;
}

void test17() {
    // test with custom class
    Dog* dog1 = new Dog("Dog1", 1);
    Dog* dog2 = new Dog("Dog2", 2);
    Dog* dog3 = new Dog("Dog3", 3);
    Cat* cat1 = new Cat("Cat1", 1);
    Cat* cat2 = new Cat("Cat2", 2);
    Cat* cat3 = new Cat("Cat3", 3);
    Mouse* mouse1 = new Mouse("Mouse1", 1);
    Mouse* mouse2 = new Mouse("Mouse2", 2);
    Mouse* mouse3 = new Mouse("Mouse3", 3);

    Animal* vertices[] = {dog1, dog2, dog3, cat1, cat2, cat3, mouse1, mouse2, mouse3};

    Edge<Animal*> edges[9] = {
        Edge<Animal*>(dog1, dog2, 1.5),
        Edge<Animal*>(dog2, dog3, 2.0),
        Edge<Animal*>(dog3, cat1, 3.2),
        Edge<Animal*>(cat1, cat2, 1.5),
        Edge<Animal*>(cat2, cat3, 2.0),
        Edge<Animal*>(cat3, mouse1, 3.2),
        Edge<Animal*>(mouse1, mouse2, 1.5),
        Edge<Animal*>(mouse2, mouse3, 2.0),
        Edge<Animal*>(mouse3, dog1, 3.2)
    };

    UGraphModel<Animal*> *model = UGraphModel<Animal*>::create(vertices, 9, edges, 9, &animalComparator, &animal2str);
    cout << model->toString();

    delete model;
    delete dog1;
    delete dog2;
    delete dog3;
    delete cat1;
    delete cat2;
    delete cat3;
    delete mouse1;
    delete mouse2;
    delete mouse3;
}

bool intComparator(int& a, int& b) {
    return a == b;
}

string vertexInt2str(int& vertex) {
    return to_string(vertex);
}

void testTopo1() {
    DGraphModel<char> model(&charComparator, &vertex2str);
    for(int idx=0; idx<8; idx++){
        model.add((char)('0' + idx));
    }
    model.connect('0', '1');
    model.connect('0', '2');
    model.connect('0', '4');
    model.connect('1', '3');
    model.connect('1', '6');
    model.connect('2', '6');
    model.connect('4', '7');
    model.connect('6', '7');
    model.connect('5', '6');
    model.println();
    
    TopoSorter<char> sorter(&model, &charNumericHash);
    DLinkedList<char> bfs = sorter.sort(TopoSorter<char>::BFS);
    cout << left << setw(15) << "Topo-order (BFS): " << bfs.toString() << endl;
    
    DLinkedList<char> dfs = sorter.sort(TopoSorter<char>::DFS);
    cout << left << setw(15) << "Topo-order (DFS): " << dfs.toString() << endl;
}

void testTopo2() {
    char vertices[] = {'4', '5', '0', '1', '2', '7', '8', '9', '3', '6'};

    // Định nghĩa các cạnh
    Edge<char> edges[14] = {
        Edge<char>('3', '4', 0),
        Edge<char>('3', '7', 0),
        Edge<char>('3', '8', 0),
        Edge<char>('4', '8', 0),
        Edge<char>('6', '0', 0),
        Edge<char>('0', '1', 0),
        Edge<char>('0', '5', 0),
        Edge<char>('1', '7', 0),
        Edge<char>('3', '2', 0),
        Edge<char>('6', '1', 0),
        Edge<char>('6', '2', 0),
        Edge<char>('8', '2', 0),
        Edge<char>('8', '7', 0),
        Edge<char>('9', '4', 0)
    };
    DGraphModel<char> model(&charComparator, &vertex2str);
    for(int idx=0; idx<10; idx++){
        model.add(vertices[idx]);
    }
    for(int idx=0; idx<14; idx++){
        model.connect(edges[idx].from, edges[idx].to, edges[idx].weight);
    }
    model.println();
    
    TopoSorter<char> sorter(&model, &charNumericHash);
    DLinkedList<char> bfs = sorter.sort(TopoSorter<char>::BFS);
    cout << left << setw(15) << "Topo-order (BFS): " << bfs.toString() << endl;
    
    DLinkedList<char> dfs = sorter.sort(TopoSorter<char>::DFS);
    cout << left << setw(15) << "Topo-order (DFS): " << dfs.toString() << endl;
}

void new_testgraph01() {
    string name = "graph01";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('A', 'B', 5);
    model.connect('B', 'D');
    model.connect('C', 'B');
    model.connect('C', 'D');

    cout << model.toString();

    model.clear();
}



void new_testgraph02() {
    string name = "graph02";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('A', 'B', 5);
    model.connect('B', 'D');
    model.connect('C', 'B');
    model.connect('C', 'D');

    cout << model.toString();

    model.clear();
}



void new_testgraph03() {
    string name = "graph03";
    stringstream output;
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D', 'E'};
    for (int idx = 0; idx < 5; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 8);
    model.connect('B', 'D', 6);
    model.connect('C', 'B', 1);
    model.connect('C', 'D', 2);
    model.connect('E', 'A', 3);
    model.connect('E', 'B', 4);
    model.connect('E', 'C', 5);
    DLinkedList<char> out = model.getOutwardEdges('E');
    DLinkedList<char> in = model.getInwardEdges('B');
    cout << "getOutwardEdges : E: ";
    for (auto it = out.begin(); it != out.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << model.toString();

    model.clear();
}


void new_testgraph04() {
    string name = "graph04";
    stringstream output;
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D', 'E'};
    for (int idx = 0; idx < 5; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 8);
    model.connect('B', 'D', 6);
    model.connect('C', 'B', 1);
    model.connect('C', 'D', 2);
    model.connect('E', 'A', 3);
    model.connect('E', 'B', 4);
    model.connect('E', 'C', 5);
    model.connect('E', 'E', 5);
    DLinkedList<char> out = model.getOutwardEdges('E');
    DLinkedList<char> in = model.getInwardEdges('B');
    cout << "getOutwardEdges : E: ";
    for (auto it = out.begin(); it != out.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << model.toString();

    model.clear();
}


void new_testgraph05() {
    string name = "graph05";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[3] = {Edge<char>('A', 'B', 1.5), Edge<char>('B', 'C', 2.0),
                          Edge<char>('C', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(
        vertices, 4, edges, 3, &charComparator, &vertex2str);

    try {
      // Gọi một phương thức có thể ném ngoại lệ
      DLinkedList<char> out = model->getOutwardEdges('E');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "getOutwardEdges :E khong ton tai"
            << endl;  // In ra thông báo lỗi
    }
    DLinkedList<char> in = model->getInwardEdges('B');

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph06() {
    string name = "graph06";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[3] = {Edge<char>('A', 'B', 1.5), Edge<char>('B', 'C', 2.0),
                          Edge<char>('C', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(
        vertices, 4, edges, 3, &charComparator, &vertex2str);

    try {
      // Gọi một phương thức có thể ném ngoại lệ
      DLinkedList<char> out = model->getOutwardEdges('E');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "getOutwardEdges :E khong ton tai"
            << endl;  // In ra thông báo lỗi
    }
    DLinkedList<char> in = model->getInwardEdges('B');

    cout << "getInwardEdges : B: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph07() {
    string name = "graph07";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[4] = {Edge<char>('A', 'B', 1.5), Edge<char>('B', 'C', 2.0),
                          Edge<char>('C', 'D', 3.2), Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(
        vertices, 4, edges, 4, &charComparator, &vertex2str);

    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->weight('A', 'D');
    } catch (const EdgeNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "AB khong ton tai" << endl;  // In ra thông báo lỗi
    }
    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->weight('E', 'D');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "E khong ton tai" << endl;  // In ra thông báo lỗi
    }

    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->weight('D', 'F');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "F khong ton tai" << endl;  // In ra thông báo lỗi
    }
    cout << "AB : " << model->weight('A', 'B') << endl;
    cout << "CD : " << model->weight('C', 'D') << endl;

    DLinkedList<char> in = model->getInwardEdges('D');

    cout << "getInwardEdges : D: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph08() {
    string name = "graph08";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('C', 'D', 3.2),
                          Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(
        vertices, 4, edges, 5, &charComparator, &vertex2str);

    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->weight('A', 'D');
    } catch (const EdgeNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "AB khong ton tai" << endl;  // In ra thông báo lỗi
    }
    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->weight('E', 'D');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "E khong ton tai" << endl;  // In ra thông báo lỗi
    }

    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->weight('D', 'F');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "F khong ton tai" << endl;  // In ra thông báo lỗi
    }
    cout << "AA : " << model->weight('A', 'A') << endl;
    cout << "CD : " << model->weight('C', 'D') << endl;

    DLinkedList<char> in = model->getInwardEdges('D');

    cout << "getInwardEdges : D: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";

    cout << model->toString();

    model->clear();
    delete model;
}

void new_testgraph09() {
    string name = "graph09";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('C', 'D', 3.2),
                          Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(
        vertices, 4, edges, 5, &charComparator, &vertex2str);
    model->remove('A');
    model->remove('B');
    DLinkedList<char> in = model->getInwardEdges('D');

    cout << "getInwardEdges : D: ";
    for (auto it = in.begin(); it != in.end(); it++) {
      cout << *it << "-> ";
    }
    cout << "NULL\n";
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph10() {
    string name = "graph10";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('C', 'D', 3.2),
                          Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(
        vertices, 4, edges, 5, &charComparator, &vertex2str);
    model->remove('B');
    model->remove('D');
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph11() {
    string name = "graph11";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('C', 'D', 3.2),
                          Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(
        vertices, 4, edges, 5, &charComparator, &vertex2str);
    cout << "Size: " << model->size() << endl;
    cout << "Indegree: " << model->inDegree('A') << endl;
    cout << "Outdegree: " << model->outDegree('A') << endl;
    model->remove('A');
    model->remove('B');
    model->remove('C');
    model->remove('D');
    cout << "Size: " << model->size() << endl;
    try {
      model->inDegree('F');
    } catch (const VertexNotFoundException &e) {
      cout << "Error: " << "F khong ton tai" << endl;
    }
    try {
      model->outDegree('F');
    } catch (const VertexNotFoundException &e) {
      cout << "Error: " << "F khong ton tai" << endl;
    }
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph12() {
    string name = "graph12";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D'};

    // Định nghĩa các cạnh
    Edge<char> edges[5] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('C', 'D', 3.2),
                          Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(
        vertices, 4, edges, 5, &charComparator, &vertex2str);
    cout << "Size: " << model->size() << endl;
    cout << "vertices: ";
    DLinkedList<char> a = model->vertices();
    for (auto it = a.begin(); it != a.end(); ++it) {
      cout << *it << " ";
    }
    cout << "\n";
    cout << "Indegree: " << model->inDegree('A') << endl;
    cout << "Outdegree: " << model->outDegree('A') << endl;
    model->remove('A');
    model->remove('B');
    model->remove('C');
    model->remove('D');
    cout << "Size: " << model->size() << endl;
    try {
      model->inDegree('F');
    } catch (const VertexNotFoundException &e) {
      cout << "Error: " << "F khong ton tai" << endl;
    }
    try {
      model->outDegree('F');
    } catch (const VertexNotFoundException &e) {
      cout << "Error: " << "F khong ton tai" << endl;
    }

    cout << model->toString();

    model->clear();
    delete model;
}

void new_testgraph13() {
    string name = "graph13";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'G'};

    // Định nghĩa các cạnh
    Edge<char> edges[6] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('B', 'B', 2.0),
                          Edge<char>('C', 'D', 3.2), Edge<char>('D', 'D', 3.2)};
    DGraphModel<char> *model = DGraphModel<char>::create(
        vertices, 8, edges, 6, &charComparator, &vertex2str);
    cout << "Size: " << model->size() << endl;
    cout << "vertices: ";
    DLinkedList<char> a = model->vertices();
    for (auto it = a.begin(); it != a.end(); ++it) {
      cout << *it << " ";
    }
    cout << "\n";
    cout << "Indegree H: " << model->inDegree('H') << endl;
    cout << "Outdegree C: " << model->outDegree('C') << endl;
    model->remove('F');
    model->remove('B');
    model->remove('H');
    model->remove('G');
    cout << "Size: " << model->size() << endl;
  
    cout << model->toString();

    model->clear();
    delete model;
}


void new_testgraph14() {
    string name = "graph14";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'G'};

    // Định nghĩa các cạnh
    Edge<char> edges[6] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('B', 'B', 2.0),
                          Edge<char>('C', 'D', 3.2), Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(
        vertices, 8, edges, 6, &charComparator, &vertex2str);
    cout << "Size: " << model->size() << endl;
    cout << "vertices: ";
    DLinkedList<char> a = model->vertices();
    for (auto it = a.begin(); it != a.end(); ++it) {
      cout << *it << " ";
    }
    cout << "\n";
    cout << "Indegree H: " << model->inDegree('H') << endl;
    cout << "Outdegree C: " << model->outDegree('C') << endl;
    cout << "connected AA : " << model->connected('A', 'A') << endl;
    cout << "connected AH : " << model->connected('A', 'H') << endl;
    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->connected('A', 'X');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "X khong ton tai" << endl;  // In ra thông báo lỗi
    }
    try {
      // Gọi một phương thức có thể ném ngoại lệ
      model->connected('V', 'Q');
    } catch (const VertexNotFoundException &e) {
      // Xử lý ngoại lệ nếu đỉnh không tìm thấy
      cout << "Error: " << "V khong ton tai" << endl;  // In ra thông báo lỗi
    }
    cout << "Size: " << model->size() << endl;

    cout << model->toString();

    model->clear();
    delete model;
}

void new_testgraph15() {
    string name = "graph15";
    stringstream output;
    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F', 'H', 'G'};

    // Định nghĩa các cạnh
    Edge<char> edges[6] = {Edge<char>('A', 'B', 1.5), Edge<char>('A', 'A', 10),
                          Edge<char>('B', 'C', 2.0), Edge<char>('B', 'B', 2.0),
                          Edge<char>('C', 'D', 3.2), Edge<char>('D', 'D', 3.2)};
    UGraphModel<char> *model = UGraphModel<char>::create(
        vertices, 8, edges, 6, &charComparator, &vertex2str);
    cout << "Size: " << model->size() << endl;
    cout << "Indegree H: " << model->inDegree('H') << endl;
    cout << "Outdegree C: " << model->outDegree('C') << endl;
    cout << "contains C : " << model->contains('C') << endl;
    cout << "contains J : " << model->contains('J') << endl;
    cout << "Size: " << model->size() << endl;

    
    cout << model->toString();
    model->clear();
    cout << model->toString();

    model->clear();
    delete model;
}

void new_testgraph16() {
    string name = "graph16";
    DGraphModel<char> model(&charComparator, &vertex2str);

    cout << model.toString();

    model.clear();
}


void new_testgraph18() {
    string name = "graph18";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'A');

    cout << model.inDegree('A') << " " << model.outDegree('A') << endl;
    cout << model.toString();

    model.clear();
}


void new_testgraph19() {
    string name = "graph19";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.disconnect('A', 'B');

    cout << model.inDegree('A') << " " << model.outDegree('A') << endl;
    cout << model.toString();

    model.clear();
}


void new_testgraph20() {
    string name = "graph20";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.disconnect('A', 'B');
    model.connect('A', 'B');
    model.disconnect('A', 'B');
    model.connect('A', 'B');
    model.disconnect('A', 'B');

    cout << model.toString();

    model.clear();
}


void new_testgraph21() {
    string name = "graph21";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.add('A');
    model.add('B');

    cout << model.toString();

    model.clear();
}


void new_testgraph22() {
    string name = "graph22";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    cout << model.contains('A') << " ";
    cout << model.contains('E') << endl;
    cout << model.toString();

    model.clear();
}


void new_testgraph23() {
    string name = "graph23";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);

    cout << model.weight('A', 'B') << endl;

    try {
      model.weight('A', 'X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.weight('Y', 'A');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.weight('A', 'C');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph24() {
    string name = "graph24";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    model.connect('A', 'C', 1);
    DLinkedList<char> outwardEdgesA = model.getOutwardEdges('A');
    DLinkedList<char> outwardEdgesB = model.getOutwardEdges('B');
    cout << "outwardEdgesA: ";
    for (auto v : outwardEdgesA) {
      cout << v << " ";
    }
    cout << endl;

    cout << "outwardEdgesB: ";
    for (auto v : outwardEdgesB) {
      cout << v << " ";
    }
    cout << endl;

    try {
      DLinkedList<char> outwardEdgesX = model.getOutwardEdges('X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph25() {
    string name = "graph25";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    model.connect('A', 'C', 1);
    DLinkedList<char> outwardEdgesA = model.getInwardEdges('A');
    DLinkedList<char> outwardEdgesB = model.getInwardEdges('B');

    cout << "getInwardEdgesA: ";
    for (auto v : outwardEdgesA) {
      cout << v << " ";
    }
    cout << endl;

    cout << "getInwardEdgesB: ";
    for (auto v : outwardEdgesB) {
      cout << v << " ";
    }
    cout << endl;

    try {
      DLinkedList<char> outwardEdgesX = model.getInwardEdges('X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph26() {
    string name = "graph26";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    model.connect('A', 'C', 1);
    cout << model.inDegree('A') << " " << model.inDegree('B') << endl;
    try {
      model.inDegree('X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph27() {
    string name = "graph27";
    
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    model.connect('A', 'C', 1);

    cout << model.outDegree('A') << " " << model.outDegree('B') << endl;
    try {
      model.outDegree('X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph28() {
    string name = "graph28";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    cout << model.connected('A', 'B') << " " << model.connected('A', 'C')
          << endl;

    try {
      model.connected('A', 'X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.connected('Y', 'A');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph29() {
    string name = "graph29";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('A', 'B', 5);

    try {
      model.connect('A', 'X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.connect('Y', 'A');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph30() {
    string name = "graph30";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('A', 'B', 5);
    try {
      model.connect('A', 'X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.connect('Y', 'A');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph31() {
    string name = "graph31";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    model.disconnect('A', 'B');
    try {
      model.disconnect('A', 'X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.disconnect('Y', 'A');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.disconnect('A', 'B');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph32() {
    string name = "graph32";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B', 5);
    model.disconnect('A', 'B');
    try {
      model.disconnect('A', 'X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.disconnect('Y', 'A');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    try {
      model.disconnect('A', 'B');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph33() {
    string name = "graph33";
    UGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('A', 'C');
    model.connect('A', 'D');
    model.remove('B');
    model.remove('A');
    try {
      model.remove('X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}


void new_testgraph34() {
    string name = "graph34";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++) {
      model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('A', 'C');
    model.connect('A', 'D');
    model.remove('B');
    model.remove('A');

    try {
      model.remove('X');
    } catch (const exception& e) {
      cout << "Error: " << string(e.what()) << endl;
    }

    cout << model.toString();

    model.clear();
}

int intKeyHash(char& key, int capacity) {
  return (key - '0') % capacity;
}

void sort_topo01() {
  string name = "sort_topo01";
  
  Queue<char> queue;
  queue.push('A');
  queue.push('B');
  queue.push('C');

  cout << queue.empty() << " " << queue.pop() << " ";
  queue.push('D');
  cout << queue.pop() << " ";
  cout << queue.pop() << " ";
  cout << queue.pop() << " " << queue.empty();
  cout << endl;
}


void sort_topo02() {
  string name = "sort_topo02";
  
  Stack<char> stack;
  stack.push('A');
  stack.push('B');
  stack.push('C');

  cout << stack.peek() << " " << stack.empty() << " " << stack.pop() << " ";
  stack.push('D');
  cout << stack.pop() << " ";
  cout << stack.pop() << " " << stack.peek() << " ";
  cout << stack.pop() << " " << stack.empty();
  cout << endl;
}


void sort_topo03() {
  string name = "sort_topo03";
  
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('1', '6');
  model.connect('1', '5');
  model.connect('1', '3');
  model.connect('4', '3');
  model.connect('3', '5');
  model.connect('5', '6');
  model.connect('5', '2');
  model.connect('8', '7');
  model.connect('6', '2');
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  model.clear();cout << endl;
}


void sort_topo04() {
  string name = "sort_topo04";
  
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('1', '6');
  model.connect('1', '5');
  model.connect('1', '3');
  model.connect('4', '3');
  model.connect('3', '5');
  model.connect('5', '6');
  model.connect('5', '2');
  model.connect('8', '7');
  model.connect('6', '2');
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  model.clear();cout << endl;
}

void sort_topo05() {
    string name = "sort_topo05";
  
    DLinkedListSE<int> data;
    data.add(1);
    data.add(2);
    data.add(0);
    data.add(3);
    data.add(3);
    data.add(8);
    data.add(9);
    data.sort();


    cout << data.toString();

    cout << "\nduyet nguoc : [";
    for (auto it = data.bbegin(); it != data.bend(); it--) {
       
        if (it != data.bbegin()) cout << ", "; // Thêm dấu phẩy
         cout << *it;
    }
    cout << "]";cout << endl;
}

int compare(int& lhs, int& rhs) {
    if (lhs > rhs) return -1; // lhs lớn hơn rhs -> giữ nguyên thứ tự
    else if (lhs < rhs) return 1; // lhs nhỏ hơn rhs -> đảo thứ tự
    else return 0; // Bằng nhau
}
void sort_topo06() {
    string name = "sort_topo06";
  
    DLinkedListSE<int> data;
    data.add(1);
    data.add(2);
    data.add(0);
    data.add(3);
    data.add(3);
    data.add(8);
    data.add(9);
    data.sort(&compare);

    cout << data.toString();
    cout << "\nduyet nguoc : [";
    for (auto it = data.bbegin(); it != data.bend(); it--) {
    if (it != data.bbegin()) cout << ", "; // Thêm dấu phẩy
        cout << *it;
    }
    cout << "]";cout << endl;
}

void sort_topo07() {
  string name = "sort_topo07";
  
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A','B','C','D','E'};
  for (int idx = 0; idx < 5; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.connect('B', 'D');
  model.connect('B', 'C');
  model.connect('B', 'E');

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}
void sort_topo08() {
  string name = "sort_topo08";
  
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A','B','C','D','E'};
  for (int idx = 0; idx < 5; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B');
  model.connect('B', 'D');
  model.connect('B', 'C');
  model.connect('B', 'E');

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}

void sort_topo09()
{
    string name = "sort_topo09";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'F','G','B', 'C', 'D', 'E', 'A'};
    for (int idx = 0; idx < 7; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('B', 'C');
    model.connect('B', 'E');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');
    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);
  
    cout << "BFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

  
    model.clear();cout << endl;
}

void sort_topo10()
{
    string name = "sort_topo10";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'F','G','B', 'C', 'D', 'E', 'A'};
    for (int idx = 0; idx < 7; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('B', 'C');
    model.connect('B', 'E');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);
  
    cout << "DFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

    model.clear();cout << endl;
}

void sort_topo11()
{
    string name = "sort_topo11";
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'F', 'G', 'B', 'C', 'D', 'E', 'A', 'X', 'Y', 'V'};
    for (int idx = 0; idx < 10; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('B', 'C');
    model.connect('B', 'E');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');
    model.connect('F', 'Y');
    model.connect('F', 'V');
    model.connect('D', 'X');
    model.connect('X', 'Y');
    model.connect('Y', 'V');
    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);
  
    cout << "BFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

  
    model.clear();cout << endl;
}

void sort_topo12()
{
    string name = "sort_topo12";
    DGraphModel<char> model(&charComparator, &vertex2str);
     char vertices[] = {'F', 'G', 'B', 'C', 'D', 'E', 'A', 'X', 'Y', 'V'};
    for (int idx = 0; idx < 10; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('B', 'C');
    model.connect('B', 'E');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');
    model.connect('F', 'Y');
    model.connect('D', 'X');
    model.connect('X', 'Y');
    model.connect('Y', 'V');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);
  
    cout << "DFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

    model.clear();cout << endl;
}

void sort_topo13()
{
    string name = "sort_topo13";
    DGraphModel<char> model(&charComparator, &vertex2str);
     char vertices[] = {'F', 'G', 'B', 'C', 'D', 'E', 'A', 'X', 'Y', 'V'};
    for (int idx = 0; idx < 10; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');
    model.connect('F', 'Y');
    model.connect('D', 'X');
    model.connect('X', 'Y');
    model.connect('Y', 'V');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);
  
    cout << "DFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

  
    model.clear();cout << endl;
}

void sort_topo14()
{
    string name = "sort_topo14";
    DGraphModel<char> model(&charComparator, &vertex2str);
     char vertices[] = {'F', 'G', 'B', 'C', 'D', 'E', 'A', 'X', 'Y', 'V'};
    for (int idx = 0; idx < 10; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');
    model.connect('F', 'Y');
    model.connect('D', 'X');
    model.connect('X', 'Y');
    model.connect('Y', 'V');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.bfsSort(false);
  
    cout << "BFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

  
    model.clear();cout << endl;
}

void sort_topo15()
{
    string name = "sort_topo15";
    DGraphModel<char> model(&charComparator, &vertex2str);
     char vertices[] = {'F', 'G', 'B', 'C', 'D', 'E', 'A', 'X', 'Y', 'V'};
    for (int idx = 0; idx < 10; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('F', 'Y');
    model.connect('D', 'X');
    model.connect('X', 'Y');
    model.connect('Y', 'V');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.dfsSort(false);
  
    cout << "DFS Topological Sort: ";
    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
    cout << "NULL";

  
    model.clear();cout << endl;
}

void sort_topo16() {
  string name = "sort_topo16";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}


void sort_topo17() {
  string name = "sort_topo17";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}


void sort_topo18() {
  string name = "sort_topo18";
  
  DGraphModel<char> model(&charComparator, &vertex2str);

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}


void sort_topo19() {
  string name = "sort_topo19";
  DGraphModel<char> model(&charComparator, &vertex2str);

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  string expect = "DFS Topological Sort: NULL";


  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";
  model.clear();cout << endl;
}


void sort_topo20() {
  string name = "sort_topo20";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('1', '6');
  model.connect('1', '5');
  model.connect('1', '3');
  model.connect('1', '2');
  model.connect('1', '4');
  model.connect('1', '7');
  model.connect('1', '8');
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";
  model.clear();cout << endl;
}


void sort_topo21() {
  string name = "sort_topo21";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  for (int idx = 0; idx < 10; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('0', '1');
  model.connect('0', '5');
  model.connect('1', '7');
  model.connect('3', '2');
  model.connect('3', '4');
  model.connect('3', '7');
  model.connect('3', '8');
  model.connect('4', '8');
  model.connect('6', '0');
  model.connect('6', '1');
  model.connect('6', '2');
  model.connect('8', '2');
  model.connect('8', '7');
  model.connect('9', '4');
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  model.clear();cout << endl;
}


void sort_topo22() {
  string name = "sort_topo22";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '7', '8', '6', '4', '5'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}


void sort_topo23() {
  string name = "sort_topo23";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '7', '8', '6', '4', '5'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  model.clear();cout << endl;
}


void sort_topo24() {
  string name = "sort_topo24";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '7', '8', '6', '4', '5'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('1', '2');
  model.connect('1', '3');
  model.connect('1', '2');
  model.connect('1', '7');
  model.connect('1', '8');
  model.connect('1', '6');
  model.connect('1', '4');
  model.connect('1', '5');

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

  cout << "DFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";


  model.clear();cout << endl;
}


void sort_topo25() {
  string name = "sort_topo25";
  DGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'1', '2', '3', '7', '8', '6', '4', '5'};
  for (int idx = 0; idx < 8; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('1', '2');
  model.connect('1', '3');
  model.connect('1', '2');
  model.connect('1', '7');
  model.connect('1', '8');
  model.connect('1', '6');
  model.connect('1', '4');
  model.connect('1', '5');

  TopoSorter<char> topoSorter(&model, &intKeyHash);
  DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::BFS);

  cout << "BFS Topological Sort: ";
  for (auto it = result.begin(); it != result.end(); it++) {
    cout << *it << "->";
  }
  cout << "NULL";

  model.clear();cout << endl;
}


void runDemo() {
    std::cout << "Direct Graph Demo 1" << std::endl;
    DGraphDemo1();
    std::cout << "Direct Graph Demo 2" << std::endl;
    DGraphDemo2();
    std::cout << "Direct Graph Demo 3" << std::endl;
    DGraphDemo3();
    // std::cout << "Dijkstra Demo" << std::endl;
    // dijkstraDemo();

    std::cout << "Undirect Graph Demo 1" << std::endl;
    ugraphDemo1();
    std::cout << "Undirect Graph Demo 2" << std::endl;
    ugraphDemo2();
    std::cout << "Undirect Graph Demo 3" << std::endl;
    ugraphDemo3();
}

// pointer function to store 15 test
void (*testFuncs[])() = {
    test1, test2, test03, test04, test05, test06, test07, test08, test09, test10, test11, test12, test13, test14, test15,
    test16, test17, testTopo1, testTopo2,
    new_testgraph01,
    new_testgraph02,
    new_testgraph03,
    new_testgraph04,
    new_testgraph05,
    new_testgraph06,
    new_testgraph07,
    new_testgraph08,
    new_testgraph09,
    new_testgraph10,
    new_testgraph11,
    new_testgraph12,
    new_testgraph13,
    new_testgraph14,
    new_testgraph15,
    new_testgraph16,
    // new_testgraph17,
    new_testgraph18,
    new_testgraph19,
    new_testgraph20,
    new_testgraph21,
    new_testgraph22,
    new_testgraph23,
    new_testgraph24,
    new_testgraph25,
    new_testgraph26,
    new_testgraph27,
    new_testgraph28,
    new_testgraph29,
    new_testgraph30,
    new_testgraph31,
    new_testgraph32,
    new_testgraph33,
    new_testgraph34,
    sort_topo01, sort_topo02, sort_topo03, sort_topo04, sort_topo05, 
    sort_topo06, sort_topo07, sort_topo08, sort_topo09, sort_topo10, 
    sort_topo11, sort_topo12, sort_topo13, sort_topo14, sort_topo15, 
    sort_topo16, sort_topo17, sort_topo18, sort_topo19, sort_topo20, 
    sort_topo21, sort_topo22, sort_topo23, sort_topo24, sort_topo25 
};

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
        else 
        {
            string folder = "TestLog/Graph";
            string path = "GraphTestLog_NhanOutput.txt";
            string output = "GraphTestLog_YourOutput.txt";
            fstream file(folder + "/" + output, ios::out);
            if (!file.is_open()) {
                fs::create_directory(folder);
                std::cout << "Create folder " << fs::absolute(folder) << std::endl;
                file.open(folder + "/" + output, ios::out);
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