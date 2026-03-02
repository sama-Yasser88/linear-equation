#include <iostream>
using namespace std;
#include "EquationManager.h"
 int main() {
        EquationManager manager;
        manager.readEquations();
        manager.executeCommands();
        return 0;
    }

