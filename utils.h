#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <map>
#include "student.h"
#include "course.h"
#include "teacher.h"

void CleanupData(vector<Student*>& students, map<string, Course*>& courses, vector<Teacher*>& teachers);

#endif
