#include "grade.h"
#include "student.h"
#include "course.h"
#include "teacher.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
    vector<Student*> students;
    map<string, Course*> courses;
    vector<Teacher*> teachers;

// Load data from CSV files
    LoadCourseData("subjects.csv", courses);
    LoadStudentData("students.csv", students);
    LoadGradeData("grades.csv", students, courses);
    LoadTeacherData("teacher_credentials.csv", teachers);

    int option;
    do {
        cout << endl << "\033[1mIIIT MANIPUR RESULT SYSTEM\033[0m" << endl << endl;
        cout << "type the option number\n1. Teacher login \n2. Generate Report for Student\n3. Exit\n";
        cin >> option;

        switch (option) {
            case 1:
     // Ensure teacher login before allowing grade entry
                if (TeacherLogin(teachers)) {  
                    TeacherLoginn(students, courses);
                } else {
                    cout << "Login required to enter grades." << endl;
                }
                break;
                
            case 2: {
                int rollNo;
                cout << "Enter roll number: ";
                cin >> rollNo;
                Student* student = GetStudentByRollNo(rollNo, students);

                if (student != nullptr) {
                    //Generate the HTML report for the specific student
                   GenerateReportForStudent(rollNo, students, courses);
                    GenerateHTMLReportForStudent(student, courses);
                } else {
                    cout << "Student with roll number " << rollNo << " not found." << endl;
                }
                break;
            }
            case 3:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid option, please try again." << endl;
                break;
        }
    } while (option != 3);

    CleanupData(students, courses, teachers);

    return 0;
}
