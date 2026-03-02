#include "EquationManager.h"

//find index of a variable; if not found, add it.
size_t EquationManager::getOrAddVariableIndex(const string& name) {
    for (size_t i = 0; i < allVariableNames.size(); ++i) {
        if (allVariableNames[i] == name)
            return i;
    }
    // new variable
    allVariableNames.push_back(name);
    return allVariableNames.size() - 1;
}


// parse a single term like "2ahmed", "-3.5x", "+apple"
// updates coeffs and constShift.
void EquationManager::parseTerm(const string& term, char sign, float sideMultiplier,
    vector<float>& coeffs, float& constShift) {
    // Find where variable name starts (first letter or underscore)
    size_t varStart = term.length();
    for (size_t i = 0; i < term.length(); ++i) {
        if (isalpha(term[i]) || term[i] == '_') {
            varStart = i;
            break;
        }
    }

    float value;
    string varName;

    if (varStart < term.length()) {
        // There is a variable
        string coefPart = term.substr(0, varStart);
        varName = term.substr(varStart);
        // Clean variable name (take only alnum and _)
        string cleanVar;
        for (char c : varName) {
            if (isalnum(c) || c == '_')
                cleanVar += c;
            else
                break;
        }
        varName = cleanVar;

        // Convert coefficient part
        if (coefPart.empty() || coefPart == "+")
            value = 1.0f;
        else if (coefPart == "-")
            value = -1.0f;
        else
            value = atof(coefPart.c_str());

        // Apply sign from operator
        if (sign == '-')
            value = -value;

        // apply side multiplier (left +1, right -1)
        value *= sideMultiplier;
        size_t idx = getOrAddVariableIndex(varName);
        if (idx >= coeffs.size())
            coeffs.resize(idx + 1, 0.0f);
        coeffs[idx] += value;
    }
    else {
        // Constant term
        value = atof(term.c_str());
        if (sign == '-')
            value = -value;
        value *= sideMultiplier;
        // constants are moved to the right: we accumulate in constShift
        // constShift will later be subtracted from constant (since right side moves left)
        constShift += value;
    }
}
// Parse one side of equation

void EquationManager::parseSide(const string& side, float sideMultiplier,
    vector<float>& coeffs, float& constShift) {
    string current;
    char lastOp = '+';

    for (size_t i = 0; i <= side.length(); ++i) {
        char c = (i < side.length()) ? side[i] : '+'; // dummy op at end
        if (c == '+' || c == '-' || i == side.length()) {
            if (!current.empty()) {
                parseTerm(current, lastOp, sideMultiplier, coeffs, constShift);
                current.clear();
            }
            lastOp = c;
        }
        else {
            current += c;
        }
    }
}
// Read equations from stdin

void EquationManager::readEquations() {
    int n;
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; ++i) {
        string line;
        getline(cin, line);

        // Remove spaces
        string clean;
        for (char c : line) {
            if (!isspace(c))
                clean += c;
        }

        // Find first '='
        size_t eqPos = clean.find('=');
        if (eqPos == string::npos) {
            cerr << "Error: no '=' in equation, skipping.\n";
            continue;
        }

        string left = clean.substr(0, eqPos);
        string right = clean.substr(eqPos + 1);

        // Temporary coefficient vector and constant shift
        vector<float> coeffs;
        float constShift = 0.0f;

        // Parse left side (multiplier +1)
        parseSide(left, 1.0f, coeffs, constShift);
        // Parse right side (multiplier -1)
        parseSide(right, -1.0f, coeffs, constShift);

        // Create equation
        Equation eq;
        eq.resizeCoefficients(allVariableNames.size());
        for (size_t j = 0; j < coeffs.size(); ++j) {
            eq.setCoefficient(j, coeffs[j]);
        }
        // Constant = -constShift (because constShift accumulated from right side moves)
        eq.setConstant(-constShift);

        equations.push_back(eq);
    }
}


void EquationManager::printNumVars() const {
    cout << allVariableNames.size() << endl;
}


void EquationManager::printEquation(int index) const {
    if (index >= 1 && index <= equations.size()) {
        cout << equations[index - 1].toString(allVariableNames) << endl;
    }
}


void EquationManager::printColumn(const string& varName) const {
    int idx = findVarIndex(varName);
    if (idx == -1) {
        // variable not present: print zeros
        for (size_t i = 0; i < equations.size(); ++i) {
            cout << "0";
            if (i < equations.size() - 1) cout << " ";
        }
        cout << endl;
        return;
    }
    for (size_t i = 0; i < equations.size(); ++i) {
        float val = equations[i].getCoefficient(idx);
        if (fabs(val - (int)val) < 1e-6)
            cout << (int)val;
        else
            cout << fixed << setprecision(2) << val;
        if (i < equations.size() - 1) cout << " ";
    }
    cout << endl;
}

 //add i j
void EquationManager::addEquations(int i, int j) {
    if (i < 1 || i > equations.size() || j < 1 || j > equations.size()) return;
    Equation result = equations[i - 1];
    result.addEquation(equations[j - 1]);
    cout << result.toString(allVariableNames) << endl;
}


//  subtract i j

void EquationManager::subtractEquations(int i, int j) {
    if (i < 1 || i > equations.size() || j < 1 || j > equations.size()) return;
    Equation result = equations[i - 1];
    result.subtractEquation(equations[j - 1]);
    cout << result.toString(allVariableNames) << endl;
}


// build coefficient matrix (rows = equations, cols = variables)

vector<vector<float>> EquationManager::buildMatrix() const {
    size_t rows = equations.size();
    size_t cols = allVariableNames.size();
    vector<vector<float>> mat(rows, vector<float>(cols, 0.0f));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat[i][j] = equations[i].getCoefficient(j);
        }
    }
    return mat;
}


//  get constants column

vector<float> EquationManager::getConstantsColumn() const {
    vector<float> col;
    for (const Equation& eq : equations) {
        col.push_back(eq.getConstant());
    }
    return col;
}


//  find variable index (returns -1 if not found)

int EquationManager::findVarIndex(const string& varName) const {
    for (size_t i = 0; i < allVariableNames.size(); ++i) {
        if (allVariableNames[i] == varName)
            return i;
    }
    return -1;
}


// Determinant calculation (supports up to 3x3, but recursive for larger)
float EquationManager::determinant(const vector<vector<float>>& mat) const {
    size_t n = mat.size();
    if (n == 1) return mat[0][0];
    if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    if (n == 3) {
        return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
            - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
            + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    }
    // For larger matrices, use recursive expansion (not efficient but works)
    float det = 0;
    for (size_t i = 0; i < n; ++i) {
        vector<vector<float>> sub(n - 1, vector<float>(n - 1));
        for (size_t j = 1; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                if (k < i) sub[j - 1][k] = mat[j][k];
                else if (k > i) sub[j - 1][k - 1] = mat[j][k];
            }
        }
        float sign = (i % 2 == 0) ? 1.0f : -1.0f;
        det += sign * mat[0][i] * determinant(sub);
    }
    return det;
}


// d (print coefficient matrix)

void EquationManager::printCoefficientMatrix() const {
    auto mat = buildMatrix();
    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); ++j) {
            float val = mat[i][j];
            if (fabs(val - (int)val) < 1e-6)
                cout << (int)val;
            else
                cout << fixed << setprecision(2) << val;
            if (j < mat[i].size() - 1) cout << " ";
        }
        cout << endl;
    }
}


//  d x1  (print Cramer matrix for variable)

void EquationManager::printCramerMatrix(const string& varName) const {
    int varIdx = findVarIndex(varName);
    if (varIdx == -1) {
        cout << "Variable not found.\n";
        return;
    }
    auto mat = buildMatrix();
    auto constants = getConstantsColumn();
    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); ++j) {
            float val;
            if (j == (size_t)varIdx)
                val = constants[i];
            else
                val = mat[i][j];
            if (fabs(val - (int)val) < 1e-6)
                cout << (int)val;
            else
                cout << fixed << setprecision(2) << val;
            if (j < mat[i].size() - 1) cout << " ";
        }
        cout << endl;
    }
}


// Command: d_value

void EquationManager::printDeterminantValue() const {
    size_t n = allVariableNames.size();
    if (equations.size() != n) {
        cout << "Matrix not square. Cannot compute determinant.\n";
        return;
    }
    auto mat = buildMatrix();
    float det = determinant(mat);
    if (fabs(det - (int)det) < 1e-6)
        cout << (int)det << endl;
    else
        cout << fixed << setprecision(2) << det << endl;
}


// substitute varName eq1 eq2

void EquationManager::substitute(const string& varName, int eq1, int eq2) {
    int varIdx = findVarIndex(varName);
    if (varIdx == -1) {
        cout << "Variable not found.\n";
        return;
    }
    if (eq1 < 1 || eq1 > equations.size() || eq2 < 1 || eq2 > equations.size()) {
        cout << "Invalid equation indices.\n";
        return;
    }

    Equation e1 = equations[eq1 - 1];
    Equation e2 = equations[eq2 - 1];

    float coef1 = e1.getCoefficient(varIdx);
    float coef2 = e2.getCoefficient(varIdx);

    if (fabs(coef2) < 1e-6) {
        cout << "Cannot substitute: variable not in second equation.\n";
        return;
    }

    float multiplier = -coef1 / coef2;
    e2.multiplyByScalar(multiplier);
    e1.addEquation(e2);
    cout << e1.toString(allVariableNames) << endl;
}


// solve (Cramer's rule)

void EquationManager::solve() {
    size_t n = allVariableNames.size();
    if (equations.size() != n) {
        cout << "Cannot solve: number of equations (" << equations.size()
            << ") != number of variables (" << n << ").\n";
        return;
    }

    auto A = buildMatrix();
    auto B = getConstantsColumn();

    float D = determinant(A);
    if (fabs(D) < 1e-6) {
        cout << "No unique solution (determinant = 0).\n";
        return;
    }

    for (size_t var = 0; var < n; ++var) {
        auto Ai = A;
        for (size_t i = 0; i < n; ++i) {
            Ai[i][var] = B[i];
        }
        float Di = determinant(Ai);
        float sol = Di / D;
        if (fabs(sol) < 1e-6) sol = 0;

        cout << allVariableNames[var] << "=";
        if (fabs(sol - (int)sol) < 1e-6)
            cout << (int)sol;
        else
            cout << fixed << setprecision(2) << sol;

        if (var < n - 1) cout << " ";
    }
    cout << endl;
}


//  dispatcher

void EquationManager::executeCommands() {
    string line;
    while (getline(cin, line)) {
        if (line == "quit") break;
        istringstream iss(line);
        string cmd;
        iss >> cmd;

        if (cmd == "num_vars") {
            printNumVars();
        }
        else if (cmd == "equation") {
            int idx;
            iss >> idx;
            printEquation(idx);
        }
        else if (cmd == "column") {
            string var;
            iss >> var;
            printColumn(var);
        }
        else if (cmd == "add") {
            int i, j;
            iss >> i >> j;
            addEquations(i, j);
        }
        else if (cmd == "subtract") {
            int i, j;
            iss >> i >> j;
            subtractEquations(i, j);
        }
        else if (cmd == "D") {
            string next;
            if (iss >> next) {
                if (next == "value")
                    printDeterminantValue();
                else
                    printCramerMatrix(next);
            }
            else {
                printCoefficientMatrix();
            }
        }
        else if (cmd == "D_value") {
            printDeterminantValue();
        }
        else if (cmd == "substitute") {
            string var;
            int i, j;
            iss >> var >> i >> j;
            substitute(var, i, j);
        }
        else if (cmd == "solve") {
            solve();
        }
        else {
            cout << "Unknown command: " << cmd << endl;
        }
    }
}