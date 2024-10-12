#include "course.h"
#include "grade.h"
#include "student.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<string>

    // load courses from csv
    void LoadCourseData(const string& filePath, map<string, Course*>& courses) {
    ifstream file(filePath);
    string line;

    // header skip for column info
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string data[4];
        getline(ss, data[0], ',');  // sem
        getline(ss, data[1], ',');  // Course code
        getline(ss, data[2], ',');  // Course name
        getline(ss, data[3], ',');  // Credits

        int sem = stoi(data[0]);
        int credits = stoi(data[3]);

        if (courses.find(data[1]) == courses.end()) {
            courses[data[1]] = new Course(data[1], data[2], credits);
        }
    }
}

void EnterCourses(map<string, Course*>& courses, const string& filePath) {
int semester, credits;
    string courseCode, courseName;

    cout << "Enter course code: ";
    cin >> courseCode;
    cout << "Enter course name: ";
    cin.ignore(); 
    getline(cin, courseName);
    cout << "Enter semester: ";
    cin >> semester;
    cout << "Enter course credits: ";
    cin >> credits;

    if (courses.find(courseCode) == courses.end()) {
        courses[courseCode] = new Course(courseCode, courseName, credits);
    } else {
        courses[courseCode]->courseName = courseName;
        courses[courseCode]->credits = credits;
    }
    // Update course info in csv
    UpdateCourseInCSV(filePath, semester, courseCode, courseName, credits);
}
void UpdateCourseInCSV(const string& filePath, int semester, const string& subjectCode, const string& subjectName, int credit) {
    ifstream file(filePath);
    string line;
    vector<string> fileLines;
    bool courseUpdated = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string data[4];

        getline(ss, data[0], ',');  // Semester
        getline(ss, data[1], ',');  // Subject code
        getline(ss, data[2], ',');  // Subject name
        getline(ss, data[3], ',');  // Credit

        if (isNumber(data[0]) && data[1] == subjectCode && stoi(data[0]) == semester) {
            fileLines.push_back(to_string(semester) + "," + subjectCode + "," + subjectName + "," + to_string(credit));
            courseUpdated = true;
        } else {
            fileLines.push_back(line);
        }
    }
    file.close();

    if (!courseUpdated) {
        fileLines.push_back(to_string(semester) + "," + subjectCode + "," + subjectName + "," + to_string(credit));
    }

    ofstream outFile(filePath);
    for (const auto& fileLine : fileLines) {
        outFile << fileLine << endl;
    }
    outFile.close();
    cout << "Course updated/added in the CSV file successfully!" << endl;
}
