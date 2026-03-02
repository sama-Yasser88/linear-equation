#ifndef EQUATION_H
#define EQUATION_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <cctype>
using namespace std;

class Equation {
private:
    vector<float> coefficients;   // coefficient for each global variable
    float constant;                // right-hand side constant

public:
    Equation();

    // Resize the coefficient vector to match the global variable count
    void resizeCoefficients(size_t size);

    
    float getCoefficient(size_t index) const;
    void setCoefficient(size_t index, float value);

    float getConstant() const;
    void setConstant(float value);

    // Add/subtract another equation (assumes same coefficient vector size)
    void addEquation(const Equation& other);
    void subtractEquation(const Equation& other);
    void multiplyByScalar(float scalar);

    // Convert to string using a provided list of variable names
    string toString(const vector<string>& varNames) const;

    // Debug print
    void printDebug(const vector<string>& varNames) const;
};

#endif