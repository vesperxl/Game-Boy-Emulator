#include "Test.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <filesystem>


Test::Test() 
    : _memory(std::make_unique<Memory>()),
     _cpu(_memory.get()) 
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
    
    for(auto a: data["initial"]["ram"]){
        _memory->write(a[0], a[1]);
    }

    _cpu.step();

    if(_cpu.PC != data["final"]["pc"]){ return 1;}
    if(_cpu.SP != data["final"]["sp"]){ return 1;}
    if(_cpu.A != data["final"]["a"]){ return 1;}
    if(_cpu.B != data["final"]["b"]){ return 1;}
    if(_cpu.C != data["final"]["c"]){ return 1;}
    if(_cpu.D != data["final"]["d"]){ return 1;}
    if(_cpu.E != data["final"]["e"]){ return 1;}
    if(_cpu.F != data["final"]["f"]){ return 1;}
    if(_cpu.H != data["final"]["h"]){ return 1;}
    if(_cpu.L != data["final"]["l"]){ return 1;}

     for(auto a : data["final"]["ram"]){
        if(_memory->read(a[0]) != a[1]){
            return 1;
        }
    }
        

    return 0;
}


void Test::runJsonFile(const json& data){
    int count = 0;
    for(int i = 0; i < 1000;i++){
        _cpu.reset();
        _memory->clearMemory();
        if(runSingleTest(i,data) == 0){
            count++;
        };
    }
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

void Test::runDetailedSingleTest(uint8_t i, const json& jsonFile){
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
    
    for(auto a: data["initial"]["ram"]){
        _memory->write(a[0], a[1]);
    }

    _cpu.step();

    if(_cpu.PC != data["final"]["pc"]){ std::cout << "PC WRONG, ACTUAL : " << _cpu.PC << ", EXPECTED : " << data["final"]["pc"] << "\n"; }
    else{ std::cout << "PC RIGHT : " << _cpu.PC << "\n";}

    if(_cpu.SP != data["final"]["sp"]){ std::cout << "SP WRONG, ACTUAL : " << _cpu.SP << ", EXPECTED : "<< data["final"]["sp"] << "\n";}
    else{ std::cout << "SP RIGHT : " << _cpu.SP << "\n";}

    if(_cpu.A != data["final"]["a"]){std::cout << "REG A WRONG, ACTUAL : " << +_cpu.A << ", EXPECTED : "<< data["final"]["a"] << "\n";}
    else{ std::cout << "REG A RIGHT : " << +_cpu.A << "\n";}

    if(_cpu.B != data["final"]["b"]){ std::cout << "REG B WRONG, ACTUAL : " << +_cpu.B << ", EXPECTED : "<< data["final"]["b"] << "\n";}
    else{ std::cout << "REG B RIGHT : " << +_cpu.B << "\n";}

    if(_cpu.C != data["final"]["c"]){std::cout << "REG C WRONG, ACTUAL : " << +_cpu.C << ", EXPECTED : "<< data["final"]["c"] << "\n";}
    else{ std::cout << "REG C RIGHT : " << +_cpu.C << "\n";}

    if(_cpu.D != data["final"]["d"]){std::cout << "REG D WRONG, ACTUAL : " << +_cpu.D << ", EXPECTED : "<< data["final"]["d"] << "\n";}
    else{ std::cout << "REG D RIGHT : " << +_cpu.D << "\n";}

    if(_cpu.E != data["final"]["e"]){ std::cout << "REG E WRONG, ACTUAL : " << +_cpu.E << ", EXPECTED : "<< data["final"]["e"] << "\n";}
    else{ std::cout << "REG E RIGHT : " << +_cpu.E << "\n";}

    if(_cpu.F != data["final"]["f"]){ std::cout << "REG F WRONG, ACTUAL : " << +_cpu.F << ", EXPECTED : "<< data["final"]["f"] << "\n";}
    else{ std::cout << "REG F RIGHT : " << +_cpu.F << "\n";}

    if(_cpu.H != data["final"]["h"]){ std::cout << "REG H WRONG, ACTUAL : " << +_cpu.H << ", EXPECTED : "<< data["final"]["h"] << "\n";}
    else{ std::cout << "REG H RIGHT : " << +_cpu.H << "\n";}

    if(_cpu.L != data["final"]["l"]){std::cout << "REG L WRONG, ACTUAL : " << +_cpu.L << ", EXPECTED : "<< data["final"]["l"] << "\n";}
    else{ std::cout << "REG L RIGHT : " << +_cpu.L << "\n";}
 
     for(auto a : data["final"]["ram"]){
        if(_memory->read(a[0]) != a[1]){
            std::cout << "RAM WRONG, AT  " << a[0] << " : " << +_memory->read(a[0]) << ", EXPECTED : " << a[1] << "\n"; 
        }else{
            std::cout << "RAM RIGHT, AT  " << a[0] << " : " << +_memory->read(a[0]) << "\n";
        }
    }
    
}


void Test::runDetailedJsonFile(const json &data){   
    for(int i = 0; i < 1000;i++){
        std::string name = data[i]["name"];
        _cpu.reset();
        _memory->clearMemory();
        if(runSingleTest(i,data) == 0){
            std::cout << "Test " << name << " Successed. \n";
        }else{
            std::cout << "Test " << name << " Failed. Details : \n";
            runDetailedSingleTest(i,data);
            exit(EXIT_FAILURE);
        }
    }
}




