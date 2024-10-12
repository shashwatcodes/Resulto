#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <map>
#include "course.h"

using namespace std;

struct Student {
    int rollNo;
    string name;
    string branch;
    string fatherName;
    string motherName;
    map<int, map<string, string>> semesterGrades; 
    map<int, float> sgpaPerSemester;
    float cgpa;

    Student(int r, const string& n, const string& b, const string& f, const string& m) 
        : rollNo(r), name(n), branch(b), fatherName(f), motherName(m), cgpa(0.0) {}
};
void UpdateStudentInCSV(const string& filePath, int rollNo, const string& name, const string& branch, const string& motherName, const string& fatherName);
void LoadStudentData(const string& filePath, vector<Student*>& students);
void EnterStudents(vector<Student*>& students, const string& filePath);
Student* GetStudentByRollNo(const int& rollNo, const vector<Student*>& students);
void DisplayStudentData(Student* student, const map<string, Course*>& courses);

#endif
