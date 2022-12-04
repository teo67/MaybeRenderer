#ifndef DEBUGGER_H
#define DEBUGGER_H
#include <string>
class Debugger {
    private:
        const static bool on = true;
    public:
        static void print(std::string output);
};
#endif