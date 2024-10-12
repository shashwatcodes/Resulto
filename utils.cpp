#include "utils.h"
#include "student.h"
#include "grade.h"
#include "course.h"
void CleanupData(vector<Student*>& students, map<string, Course*>& courses, vector<Teacher*>& teachers) {
    for (auto& student : students) {
        delete student;
    }
    for (auto& course : courses) {
        delete course.second;
    }
    for (auto& teacher : teachers) {
        delete teacher;
    }
}
