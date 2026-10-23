#pragma once
#include "nlohmann/json.hpp"
#include <fstream>
#include "Memory.hpp"
#include "Cpu.hpp"
#include <iostream>

using json = nlohmann::json;


class Test{
    
    private:
        std::unique_ptr<Memory> _memory;
        Cpu _cpu;

    public:
        Test();
        json parseFile(std::string path);
        uint8_t runSingleTest(uint8_t i, const json& data);
        void runJsonFile(const json& data);
        void runAllTests();
        void runDetailedSingleTest(const uint8_t i, const json& data);
        void runDetailedJsonFile(const json& data);
};