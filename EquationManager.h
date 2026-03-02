#ifndef EQUATION_MANAGER_H
#define EQUATION_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cctype>
#include "Equation.h"
using namespace std;

class EquationManager {
private:
    vector<Equation> equations;           // all equations
    vector<string> allVariableNames;      // global variable 
    size_t getOrAddVariableIndex(const string& name);
    void parseTerm(const string& term, char sign, float sideMultiplier,
        vector<float>& coeffs, float& constShift);
   
    void parseSide(const string& side, float sideMultiplier,
        vector<float>& coeffs, float& constShift);
public:
    void readEquations();
    // Commands
    void printNumVars() const;
    void printEquation(int index) const;
    void printColumn(const string& varName) const;
    void addEquations(int i, int j);
    void subtractEquations(int i, int j);
    void printCoefficientMatrix() const;
    void printCramerMatrix(const string& varName) const;
    void printDeterminantValue() const;
    void substitute(const string& varName, int eq1, int eq2);
    void solve();
    void executeCommands();
private:
    // Matrix 
    vector<vector<float>> buildMatrix() const;
    vector<float> getConstantsColumn() const;
    float determinant(const vector<vector<float>>& mat) const;
    int findVarIndex(const string& varName) const;
};

#endif