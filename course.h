#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <map>
using namespace std;

struct Course {
    string courseCode;
    string courseName;
    int credits;

    Course(const string& code, const string& name, int cr) : courseCode(code), courseName(name), credits(cr) {}
};

void LoadCourseData(const string& filePath, map<string, Course*>& courses);
void EnterCourses(map<string, Course*>& courses, const string& filePath);
void UpdateCourseInCSV(const string& filePath, int semester, const string& subjectCode, const string& subjectName, int credit);
#endif
