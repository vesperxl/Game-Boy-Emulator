#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Test.hpp"

using json = nlohmann::json;

int main(int argvc, char* argv[]){

    Test test;
    /* json jsonFile = test.parseFile("/Users/vesperal/Documents/Private/Game-Boy-Emulator/SingleStepTests/0C.json");
    test.runDetailedJsonFile(jsonFile); */

       test.runAllTests();
      return 0;
}