#ifndef TEACHER_H
#define TEACHER_H

#include <vector>
#include <string>
#include "student.h"
using namespace std;

struct Teacher {
    string username;
    string password;

    Teacher(const string& u, const string& p) : username(u), password(p) {}
};

void LoadTeacherData(const string& filePath, vector<Teacher*>& teachers);
bool TeacherLogin(vector<Teacher*>& teachers);
void TeacherLoginn(vector<Student*>& students, map<string, Course*>& courses);

#endif
