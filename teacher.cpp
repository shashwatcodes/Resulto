#include "teacher.h"
#include "grade.h"
#include "student.h"
#include "course.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>

void LoadTeacherData(const string& filePath, vector<Teacher*>& teachers) {
    ifstream file(filePath);
    string line;

    // header skip as column info 
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string data[2];

        getline(ss, data[0], ',');  
        getline(ss, data[1], ',');  

        Teacher* teacher = new Teacher(data[0], data[1]);
        teachers.push_back(teacher);
    }
}
bool TeacherLogin(vector<Teacher*>& teachers) {
    string inputUsername, inputPassword;

    cout << "Enter username: ";
    cin >> inputUsername;
    cout << "Enter password: ";
    cin >> inputPassword;

    // credintial check for teachers
    for (auto& teacher : teachers) {
        if (teacher->username == inputUsername && teacher->password == inputPassword) {
            cout << "Hello "<<teacher->username << endl;
            return true;
        }
    }

    cout << "Login failed. Incorrect username or password." << endl;
    return false;
}

void TeacherLoginn(vector<Student*>& students, map<string, Course*>& courses) {
    int choice;
    string gradesFilePath = "grades.csv";
    string studentsFilePath = "students.csv";
    string coursesFilePath = "subjects.csv";

    do {
        cout << "\nselect option\n";
        cout << "1. Enter/Update Grades\n";
        cout << "2. Enter/Update Students\n";
        cout << "3. Enter/Update Courses\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            EnterGrades(students, courses, gradesFilePath);

        } else if (choice == 2) {
            EnterStudents(students, studentsFilePath);

        } else if (choice == 3) {
            EnterCourses(courses, coursesFilePath);

        } else if (choice == 4) {
            cout << "Logging out..." << endl;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 4);
}
