#include "Test.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <filesystem>


Test::Test() 
    : _memory(std::make_unique<Memory>()),
     _cpu(*_memory) 
    {   
        
    }

json Test::parseFile(std::string path){

    std::ifstream f(path);
    json data = json::parse(f);
    return data;
}

uint8_t Test::runSingleTest(uint8_t i, const json& jsonFile){

    auto data = jsonFile[i];
    
    _cpu.PC = data["initial"]["pc"];
    _cpu.SP = data["initial"]["sp"];
    _cpu.A = data["initial"]["a"];
    _cpu.B = data["initial"]["b"];
    _cpu.C = data["initial"]["c"];
    _cpu.D = data["initial"]["d"];
    _cpu.E = data["initial"]["e"];
    _cpu.F = data["initial"]["f"];
    _cpu.H = data["initial"]["h"];
    _cpu.L = data["initial"]["l"];
    
    for(auto a: data["ram"]){
        _memory->write(a[0], a[1]);
    }

    _cpu.step();

    if(_cpu.PC != data["final"]["pc"]){ return 1;};
    if(_cpu.SP != data["final"]["sp"]){ return 1;};
    if(_cpu.A != data["final"]["a"]){ return 1;};
    if(_cpu.B != data["final"]["b"]){ return 1;};
    if(_cpu.C != data["final"]["c"]){ return 1;};
    if(_cpu.D != data["final"]["d"]){ return 1;};
    if(_cpu.E != data["final"]["e"]){ return 1;};
    if(_cpu.F != data["final"]["f"]){ return 1;};
    if(_cpu.H != data["final"]["h"]){ return 1;};
    if(_cpu.L != data["final"]["l"]){ return 1;};

     for(auto a : data["ram"]){
        if(_memory->read(a[0]) != a[1]){
            return 1;
        }
    }
        

    return 0;
}


void Test::runJsonFile(const json& data){
    int count = 0;
    for(int i = 0; i < 1000;i++){
        if(runSingleTest(i,data) == 0){
            count++;
        };
    }
    std::string name = data[0]["name"];
    std::cout << count << "/1000 tests successed \n"; 
    
}


void Test::runAllTests(){
    
    std::string path = "/Users/vesperal/Documents/Private/Game-Boy-Emulator/SingleStepTests/";

    std::vector<std::filesystem::path> files;
    std::copy(std::filesystem::directory_iterator(path),std::filesystem::directory_iterator(),std::back_inserter(files));
    std::sort(files.begin(),files.end());

    for(auto& filename : files){

        if(filename.string().substr(filename.string().length() - 9) == ".DS_Store"){
            continue;
        }
        std::cout << "File : " << filename.string().substr(filename.string().find_last_of("/") + 1) << " ->  ";
        json jsonFile = parseFile(filename.string());
        runJsonFile(jsonFile);
    }

    

}




