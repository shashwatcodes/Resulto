#ifndef GRADE_H
#define GRADE_H

#include <vector>
#include "student.h"
#include "course.h"


void LoadGradeData(const string& filePath, vector<Student*>& students,map<string, Course*>& courses);
void EnterGrades(vector<Student*>& students, map<string, Course*>& courses, const string& filePath);
double CalculateSGPA(Student* student, int semester, const map<string, Course*>& courses);
double CalculateCGPA(Student* student, const map<string, Course*>& courses);
void GenerateHTMLReportForStudent(Student* student, const map<string, Course*>& courses);
void GenerateReportForStudent(int rollNo,  vector<Student*>& students, map<string, Course*>& courses);
void UpdateGradeInCSV(const string& filePath, int rollNo, int semester, const string& courseCode, const string& grade);
bool isNumber(const string& s);
#endif
