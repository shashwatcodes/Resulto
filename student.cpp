#include "student.h"
#include "course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "grade.h"

void LoadStudentData(const string& filePath, vector<Student*>& students) {
    ifstream file(filePath);
    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string data[5];
        getline(ss, data[0], ','); // Roll number
        getline(ss, data[1], ','); // Name
        getline(ss, data[2], ','); // Branch
        getline(ss, data[3], ','); // Mother's name
        getline(ss, data[4], ','); // Father's name

        int rollNo = stoi(data[0]);
        string name = data[1];
        string branch = data[2];
        string motherName = data[3];
        string fatherName = data[4];

        Student* student = new Student(rollNo, name, branch, fatherName, motherName);
        students.push_back(student);
    }
}
void EnterStudents(vector<Student*>& students, const string& filePath) {
    int rollNo;
    string name, branch, fatherName, motherName;

    cout << "Enter student roll number: ";
    cin >> rollNo;
    cout << "Enter student name: ";
    cin.ignore(); 
    getline(cin, name);
    cout << "Enter student branch: ";
    getline(cin, branch);
    cout << "Enter father's name: ";
    getline(cin, fatherName);
    cout << "Enter mother's name: ";
    getline(cin, motherName);

    Student* student = nullptr;
    for (auto& s : students) {
        if (s->rollNo == rollNo) {
            student = s;
            break;
        }
    }

    if (!student) {
        student = new Student(rollNo, name, branch, fatherName, motherName);
        students.push_back(student);
    } else {
        student->name = name;
        student->branch = branch;
        student->fatherName = fatherName;
        student->motherName = motherName;
    }

    // Update the student information in the CSV file
    UpdateStudentInCSV(filePath, rollNo, name, branch, motherName, fatherName);
}
void UpdateStudentInCSV(const string& filePath, int rollNo, const string& name, const string& branch, const string& motherName, const string& fatherName) {
    ifstream file(filePath);
    string line;
    vector<string> fileLines;
    bool studentUpdated = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string data[5];

        getline(ss, data[0], ',');  // Roll number
        getline(ss, data[1], ',');  // Name
        getline(ss, data[2], ',');  // Branch
        getline(ss, data[3], ',');  // Mother's name
        getline(ss, data[4], ',');  // Father's name

        if (isNumber(data[0]) && stoi(data[0]) == rollNo) {
            fileLines.push_back(to_string(rollNo) + "," + name + "," + branch + "," + motherName + "," + fatherName);
            studentUpdated = true;
        } else {
            fileLines.push_back(line);
        }
    }
    file.close();

    if (!studentUpdated) {
        fileLines.push_back(to_string(rollNo) + "," + name + "," + branch + "," + motherName + "," + fatherName);
    }

    ofstream outFile(filePath);
    for (const auto& fileLine : fileLines) {
        outFile << fileLine << endl;
    }
    outFile.close();
    cout << "Student updated/added in the CSV file successfully!" << endl;
}

Student* GetStudentByRollNo(const int& rollNo, const vector<Student*>& students) {
    for (Student* student : students) {
        if (student->rollNo == rollNo) {
            return student;
        }
    }
    return nullptr;
}

void DisplayStudentData(Student* student, const map<string, Course*>& courses) {
    cout << "Student Name: " << student->name << endl;
    cout << "Roll No: " << student->rollNo << endl;
    cout << "--------------------------------------" << endl;

    for (const auto& semester : student->semesterGrades) {
        cout << "Semester: " << semester.first << endl;
        cout << left << setw(12) << "Course Code" << setw(8) << "Grade" << setw(8) << "Credits" << endl;
        cout << "--------------------------------------" << endl;

        for (const auto& course : semester.second) {
            cout << left << setw(12) << course.first << setw(8) << course.second << setw(8) << courses.at(course.first)->credits << endl;
        }

        cout << "SGPA: " << student->sgpaPerSemester[semester.first] << endl;
        cout << "--------------------------------------" << endl;
    }

    cout << "CGPA: " << student->cgpa << endl;
    cout << "--------------------------------------" << endl;
}
