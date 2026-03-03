# 📐 Linear Equation System Manager (C++)

A console-based C++ application for managing and solving systems of
linear equations using Object-Oriented Programming principles.

------------------------------------------------------------------------

## 📌 Overview

This project implements a **Linear Equation System Manager** that allows
users to:

-   Input multiple linear equations
-   Parse equations dynamically
-   Manage variables automatically
-   Perform matrix operations
-   Compute determinants
-   Apply Cramer's Rule
-   Solve the system
-   Perform substitution
-   Add and subtract equations

The system is built using clean OOP design and modular architecture.

------------------------------------------------------------------------

## 🏗️ Project Architecture

### 1️⃣ Equation Class

Responsible for: - Storing coefficients - Storing constant term -
Performing equation operations (Addition, Subtraction, Scalar
multiplication) - Converting equation to formatted string - Debug
printing

### 2️⃣ EquationManager Class

Responsible for: - Reading equations from user - Parsing equation
strings - Managing global variable list - Building coefficient matrix -
Computing determinant - Generating Cramer matrices - Performing
substitution - Solving system - Executing user commands

------------------------------------------------------------------------

## 📂 Project Structure

Linear-Equation-System/ │ ├── ConsoleApplication1.cpp ├── Equation.h ├──
Equation.cpp ├── EquationManager.h ├── EquationManager.cpp └── README.md

------------------------------------------------------------------------

## 🧮 Features

-   Dynamic variable detection\
-   Equation parsing from text input\
-   Matrix construction\
-   Determinant calculation\
-   Cramer's Rule implementation\
-   Equation addition and subtraction\
-   Variable substitution\
-   Formatted equation printing

------------------------------------------------------------------------

## 🛠️ Technologies Used

-   C++
-   Standard Template Library (STL)
-   Object-Oriented Programming
-   Matrix mathematics

------------------------------------------------------------------------

## ▶️ How to Run

### Using Visual Studio

1.  Open the solution file.
2.  Build the solution.
3.  Run the project.

### Using g++

g++ ConsoleApplication1.cpp Equation.cpp EquationManager.cpp -o solver
./solver

------------------------------------------------------------------------

## 📊 Mathematical Techniques Used

-   Matrix Representation of Linear Systems
-   Determinant Calculation (Recursive Method)
-   Cramer's Rule
-   Row-based Equation Operations

------------------------------------------------------------------------

## ⚠️ Limitations

-   Floating-point precision limitations
-   Determinant recursion may be inefficient for very large matrices

------------------------------------------------------------------------

## 🚀 Future Improvements

-   Gaussian Elimination implementation
-   Performance optimization
-   GUI version
-   Step-by-step solving visualization

------------------------------------------------------------------------

## 👥 Project Team

-   Sama Yasser Gemeay -- 12265
-   Hager Sherif Ibrahim -- 12266
-   Rabea Shaban Ibrahim -- 11219

------------------------------------------------------------------------

## 📜 Academic Purpose

This project was developed as part of a university assignment to
demonstrate OOP principles, matrix operations, and solving linear
systems programmatically.
