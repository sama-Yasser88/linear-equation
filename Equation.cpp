#include "Equation.h"

Equation::Equation() : constant(0.0f) {}

void Equation::resizeCoefficients(size_t size) {
    coefficients.resize(size, 0.0f);
}

float Equation::getCoefficient(size_t index) const {
    if (index < coefficients.size())
        return coefficients[index];
    return 0.0f;
}

void Equation::setCoefficient(size_t index, float value) {
    if (index < coefficients.size())
        coefficients[index] = value;
    // if index out of range, we ignore – should not happen if properly resized.
}

float Equation::getConstant() const {
    return constant;
}

void Equation::setConstant(float value) {
    constant = value;
}

void Equation::addEquation(const Equation& other) {
    // assume both have same size
    for (size_t i = 0; i < coefficients.size() && i < other.coefficients.size(); ++i) {
        coefficients[i] += other.coefficients[i];
    }
    constant += other.constant;
}

void Equation::subtractEquation(const Equation& other) {
    for (size_t i = 0; i < coefficients.size() && i < other.coefficients.size(); ++i) {
        coefficients[i] -= other.coefficients[i];
    }
    constant -= other.constant;
}

void Equation::multiplyByScalar(float scalar) {
    for (float& c : coefficients) {
        c *= scalar;
    }
    constant *= scalar;
}

string Equation::toString(const vector<string>& varNames) const {
    ostringstream oss;
    bool first = true;

    for (size_t i = 0; i < coefficients.size(); ++i) {
        float coef = coefficients[i];
        if (fabs(coef) < 1e-6) continue;  // skip zero

        // sign
        if (coef > 0 && !first) oss << "+";
        else if (coef < 0) oss << "-";

        // coefficient value (handle 1 and -1)
        float absCoef = fabs(coef);
        if (fabs(absCoef - 1.0) < 1e-6) {
            oss << varNames[i];
        }
        else {
            // print with 2 decimal places if needed
            if (fabs(absCoef - (int)absCoef) < 1e-6)
                oss << (int)absCoef << varNames[i];
            else
                oss << fixed << setprecision(2) << absCoef << varNames[i];
        }
        first = false;
    }

    if (first) oss << "0";  // no variables

    oss << "=";
    if (fabs(constant - (int)constant) < 1e-6)
        oss << (int)constant;
    else
        oss << fixed << setprecision(2) << constant;

    return oss.str();
}

void Equation::printDebug(const vector<string>& varNames) const {
    cout << "Equation coefficients:\n";
    for (size_t i = 0; i < coefficients.size(); ++i) {
        if (fabs(coefficients[i]) > 1e-6)
            cout << "  " << varNames[i] << " = " << coefficients[i] << endl;
    }
    cout << "  constant = " << constant << endl;
}