#include <iostream>
#include <string>
#include "Debugger.h"
void Debugger::print(std::string output) {
    if(on) {
        std::cout << output << std::endl;
    }
}