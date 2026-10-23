#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Test.hpp"

using json = nlohmann::json;

int main(int argvc, char* argv[]){

    Test test;
    test.runAllTests();
    
    return 0;
}