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

using namespace std;
namespace fs = std::filesystem;
int num_task = 16;


vector<vector<string>> expected_task (num_task, vector<string>(1000, ""));
vector<vector<string>> output_task (num_task, vector<string>(1000, ""));
vector<int> diffTasks(0);
vector<int> doTasks(0);

void compareFile(const string& filename1, const string& filename2) {
    string log_file = "TestLog/Layer/LayerTestLog_Compare.txt";
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
    //! data ------------------------------------
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
}
void runDemo() {
    std::cout << "Direct Graph Demo 1" << std::endl;
    DGraphDemo1();
    std::cout << "Direct Graph Demo 2" << std::endl;
    DGraphDemo2();
    std::cout << "Direct Graph Demo 3" << std::endl;
    DGraphDemo3();
    std::cout << "Dijkstra Demo" << std::endl;
    dijkstraDemo();

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
    test16
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