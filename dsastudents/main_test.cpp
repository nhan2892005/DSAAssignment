#include <iostream>
#include <string>
#include "include/list/XArrayListDemo.h"

using namespace std;

void testArrayList() {
    std::cout << "Formal Test" << '\n';
    std::cout << "Test 1" << '\n';
    xlistDemo1();
    std::cout << "Test 2" << '\n';
    xlistDemo2();
    std::cout << "Test 3" << '\n';
    xlistDemo3();
    std::cout << "Test 4" << '\n';
    xlistDemo4();
}

int main(int agrc, char* argv[]) {
    testArrayList();
    return 0;
}