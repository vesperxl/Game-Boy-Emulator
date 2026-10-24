#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Test.hpp"
#include<string>

using json = nlohmann::json;

int main(int argc, char* argv[]){

    Test test;

    if(argc == 2){
        std::string path = "/Users/vesperal/Documents/Private/Game-Boy-Emulator/SingleStepTests/" + (std::string)argv[1] + ".json";
        json jsonFile = test.parseFile(path);
        test.runDetailedJsonFile(jsonFile);
        return 0;
    }else if (argc == 3){
        std::string path = "/Users/vesperal/Documents/Private/Game-Boy-Emulator/SingleStepTests/" + (std::string)argv[1] + " " + (std::string)argv[2] + ".json";
        json jsonFile = test.parseFile(path);
        test.runDetailedJsonFile(jsonFile);
        return 0;
    }
    
 
    test.runAllTests();
      return 0;
}