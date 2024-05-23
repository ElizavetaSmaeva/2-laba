#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "Student.h"
#include "QuadraticEquation.h"

// Эта функция решает квадратное уравнение с помощью дискриминанта и сохраняет корни в переданные переменные x1 и x2.
void solveQuadraticEquation(QuadraticEquation eq, double& x1, double& x2) {
    double D = eq.b * eq.b - 4 * eq.a * eq.c;
    if (D < 0) {
        x1 = x2 = 0;
    } else {
        x1 = (-eq.b + sqrt(D)) / (2 * eq.a);
        x2 = (-eq.b - sqrt(D)) / (2 * eq.a);
    }
}
bool isCorrectSolution(QuadraticEquation eq, double x1, double x2) {
    return eq.a * x1 * x1 + eq.b * x1 + eq.c == 0 && eq.a * x2 * x2 + eq.b * x2 + eq.c == 0;
}
std::vector<Student> generateStudents() {
    std::vector<Student> students;
    srand(time(0));
    for (int i = 0; i < 10; ++i) {
        Student s;
        s.name = "Student" + std::to_string(i);
        int r = rand() % 3;
        if (r == 0) {
            s.status = "good";
        } else if (r == 1) {
            s.status = "average";
        } else {
            s.status = "poor";
        }
        students.push_back(s);
    }
    return students;
}
std::vector<QuadraticEquation> readEquationsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<QuadraticEquation> equations;
    if (file.is_open()) {
        QuadraticEquation eq;
        while (file >> eq.a >> eq.b >> eq.c) {
            equations.push_back(eq);
        }
        file.close();
    }
    return equations;
}
void solveEquationsAndStoreResults(std::vector<Student>& students, const std::vector<QuadraticEquation>& equations, std::vector<std::string>& solutions) {
    for (const auto& eq : equations) {
        double x1, x2;
        solveQuadraticEquation(eq, x1, x2);
        for (const auto& student : students) {
            if (student.status == "good" || (student.status == "average" && (rand() % 2 == 0))) {
                solutions.push_back(std::to_string(eq.a) + "x^2 + " + std::to_string(eq.b) + "x + " + std::to_string(eq.c) + " solution: " + std::to_string(x1) + " and " + std::to_string(x2) + " " + student.name);
            } else {
                solutions.push_back(std::to_string(eq.a) + "x^2 + " + std::to_string(eq.b) + "x + " + std::to_string(eq.c) + " solution: 0 " + student.name);
            }
        }
    }
}

std::vector<std::pair<std::string, int>> calculateSuccessfulSolutions(const std::vector<std::string>& solutions, const std::vector<Student>& students) {
    std::vector<std::pair<std::string, int>> results;

    for (const auto& solution : solutions) {
        std::string name = solution.substr(solution.find_last_of(" ") + 1);
        bool found = false;

        for (auto& result : results) {
            if (result.first == name) {
                found = true;
                result.second++;
                break;
            }
        }

        if (!found) {
            results.push_back(std::make_pair(name, 1));
        }
    }
    for (auto& result : results) {
        for (const auto& student : students) {
            if (result.first == student.name) {
                if (student.status == "average") {
                    result.second /= 2; // Reduce count by half for average students
                } else if (student.status == "poor") {
                    result.second = 0; // Set count to 0 for poor students
                }
            }
        }
    }

    return results;
}
// Эта функция выводит результаты успешных решений для каждого студента на экран.
void printResults(const std::vector<std::pair<std::string, int>>& results, const std::vector<Student>& students) {
    std::cout << "Student Successful solutions\n";
    for (const auto& res : results) {
        std::string status;
        for (const auto& student : students) {
            if (student.name == res.first) {
                status = student.status;
                break;
            }
        }
        if (status == "good") {
            std::cout << res.first << " " << res.second << " Excellent solutions\n";
        } else if (status == "average") {
            std::cout << res.first << " " << res.second << " Average solutions\n";
        } else {
            std::cout << res.first << " " << res.second << " Poor solutions\n";
        }
    }
}
int main() {
   std::vector<Student> students = generateStudents();
    std::vector<std::string> solutions;
    std::vector<QuadraticEquation> equations = readEquationsFromFile("quadratic_equations.txt");

    solveEquationsAndStoreResults(students, equations, solutions);

    std::vector<std::pair<std::string, int>> results = calculateSuccessfulSolutions(solutions, students);

   printResults(results, students);

    return 0;
}
