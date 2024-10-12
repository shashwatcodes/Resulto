#include "student.h"
#include "grade.h"
#include "course.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
class Grade {
public:
    static map<string, int> gradePoints;
};
map<string, int> Grade::gradePoints = {
    { "AA", 10 }, { "AB", 9 }, { "BB", 8 },
    { "BC", 7 }, { "CC", 6 }, { "CD", 5 },
    { "DD", 4 }, { "FF", 0 },{"DP",0},{"PP",0}
};
 bool isNumber(const string& s) {
    return !s.empty() && find_if(s.begin(), s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}
void EnterGrades(vector<Student*>& students, map<string, Course*>& courses, const string& filePath) {
    int rollNo;
    string courseCode, grade;
    int semester;

    cout << "Enter student roll number: ";
    cin >> rollNo;
    cout << "Enter course code: ";
    cin >> courseCode;
    cout << "Enter grade: ";
    cin >> grade;
    cout << "Enter semester: ";
    cin >> semester;

    Student* student = nullptr;
    for (auto& s : students) {
        if (s->rollNo == rollNo) {
            student = s;
            break;
        }
    }

    if (!student) {
        student = new Student(rollNo, "Student Name", "Branch", "Father Name", "Mother Name");
        students.push_back(student);
    }
    if (courses.find(courseCode) == courses.end()) {
        courses[courseCode] = new Course(courseCode, "Course Name", 0);  
    }
    student->semesterGrades[semester][courseCode] = grade;
    student->sgpaPerSemester[semester] = CalculateSGPA(student, semester, courses);
    student->cgpa = CalculateCGPA(student, courses);
    UpdateGradeInCSV(filePath, rollNo, semester, courseCode, grade);
}
void UpdateGradeInCSV(const string& filePath, int rollNo, int semester, const string& courseCode, const string& grade) {
    ifstream file(filePath);
    string line;
    vector<string> fileLines;
    bool gradeUpdated = false;
    while (getline(file, line)) {
        stringstream ss(line);
        string data[4];

        getline(ss, data[0], ',');  // Semester
        getline(ss, data[1], ',');  // Roll number
        getline(ss, data[2], ',');  // Course code
        getline(ss, data[3], ',');  // Grade

        if (isNumber(data[0]) && isNumber(data[1])) {
            if (stoi(data[0]) == semester && stoi(data[1]) == rollNo && data[2] == courseCode) {
                fileLines.push_back(to_string(semester) + "," + to_string(rollNo) + "," + courseCode + "," + grade);
                gradeUpdated = true;
            } else {
                fileLines.push_back(line);
            }
        } else {
            fileLines.push_back(line);
        }
    }
    file.close();
    if (!gradeUpdated) {
        fileLines.push_back(to_string(semester) + "," + to_string(rollNo) + "," + courseCode + "," + grade);
    }
    ofstream outFile(filePath);
    for (const auto& fileLine : fileLines) {
        outFile << fileLine << endl;
    }
    outFile.close();
    cout << "Grade updated in the CSV file successfully!" << endl;
}

void LoadGradeData(const string& filePath, vector<Student*>& students, map<string, Course*>& courses) {
    ifstream file(filePath);
    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string data[4];
        getline(ss, data[0], ',');  // Semester
        getline(ss, data[1], ',');  // Roll number
        getline(ss, data[2], ',');  // Course code
        getline(ss, data[3], ',');  // Grade

        if (data[0].empty() || data[1].empty() || data[2].empty() || data[3].empty()) {
    continue;  
}

        int semester = stoi(data[0]);
        int rollNo = stoi(data[1]);
        string courseCode = data[2];
        string grade = data[3];

        Student* student = nullptr;
        for (auto& s : students) {
            if (s->rollNo == rollNo) {
                student = s;
                break;
            }
        }

        if (!student) {
            continue;  
        }
        student->semesterGrades[semester][courseCode] = grade;
        if (courses.find(courseCode) == courses.end()) {
            courses[courseCode] = new Course(courseCode, "Course Name", 0); 
        }
    }
}
double CalculateSGPA(Student* student, int semester, const map<string, Course*>& courses) {
    double totalGradePoints = 0;
    int totalCredits = 0;

for (const auto& kvp : student->semesterGrades[semester]) {
    string courseCode = kvp.first;
    string grade = kvp.second;
     if (grade == "DP" || grade == "PP") {
            continue;
        }
    // Check if the student failed this course ("FF") in a previous semester
    bool failedPreviously = false;
    for (const auto& prevSem : student->semesterGrades) {
        if (prevSem.first < semester && prevSem.second.find(courseCode) != prevSem.second.end() && prevSem.second.at(courseCode) == "FF") {
            failedPreviously = true;
            break;
        }
    }
        // Check if the student failed this course ("FF") in a previous semester

        // If the student failed this course before and is now reappearing and passing, ignore for SGPA calculation
        if (failedPreviously && Grade::gradePoints[grade] > 0) continue;

        // Check if the grade exists in the gradePoints map
        if (Grade::gradePoints.find(grade) != Grade::gradePoints.end()) {
            int gradePoint = Grade::gradePoints[grade];
if (courses.find(courseCode) != courses.end()) {
    int credits = courses.at(courseCode)->credits;
    totalGradePoints += gradePoint * credits;
    totalCredits += credits;
} else {
    cout << "Warning: Course " << courseCode << " not found for roll number " << student->rollNo << endl;
}

            // Check if the course exists in the courses map
        } else {
            cout << "Warning: Grade " << grade << " not recognized for roll number " << student->rollNo << endl;
        }
    }

    return (totalCredits > 0) ? (totalGradePoints / totalCredits) : 0;
}
double CalculateCGPA(Student* student, const map<string, Course*>& courses) {
    double totalGradePoints = 0;
    int totalCredits = 0;
    map<string, bool> courseHandled;  

    for (const auto& semester : student->semesterGrades) {
        for (const auto& kvp : semester.second) {
            string courseCode = kvp.first;
            string grade = kvp.second;
if (grade == "DP" || grade == "PP") {
            continue;
        }
            if (courseHandled[courseCode]) continue;
            if (Grade::gradePoints.find(grade) != Grade::gradePoints.end()) {
                int gradePoint = Grade::gradePoints[grade];
                if (gradePoint > 0) { 
                    int credits = courses.at(courseCode)->credits;
                    totalGradePoints += gradePoint * credits;
                    totalCredits += credits;
                    courseHandled[courseCode] = true;  
                }
            }
        }
    }

    return (totalCredits > 0) ? (totalGradePoints / totalCredits) : 0;
}
void GenerateHTMLReportForStudent(Student* student, const map<string, Course*>& courses) {
    ofstream file("report_"+to_string(student->rollNo)+ ".html");
    if (file.is_open()) {
        file << "<!DOCTYPE html><html><head><title>Student Report</title>";
        file << "<style>body {font-family: Arial, sans-serif; margin: 20px;} "
                ".student {border: 1px solid #ddd; padding: 20px; margin-bottom: 40px;} "
                ".student-header {display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px;} "
                ".logo {width: 100px; height: auto;} "
                "h1, h2, h3 {margin: 0; padding: 0;} "
                "h1 {color: #000000; text-align: center;} "
                "h2 {color: #333;} "
                ".info-table, .grade-table {width: 100%; border-collapse: collapse; margin-top: 20px;} "
                "table, th, td {border: 1px solid #ddd; padding: 8px; text-align: left;} "
                "th {background-color: #f2f2f2;} "
                ".info-table th, .info-table td {width: 50%;} "
                ".summary {font-weight: bold; margin-top: 20px;} "
                "</style></head><body>";

        file << "<h1>IIIT MANIPUR - Examination Report</h1>";

        file << "<div class='student'>";
        file << "<div class='student-header'>";
        file << "<img src='college_logo.png' alt='College Logo' class='logo'>"; // Placeholder for college logo
        file << "<div><h2>IIIT MANIPUR</h2><h3>Department: " << "Computer Science" << "</h3></div>";
        file << "</div>";
        file << "<table class='info-table'>";
        file << "<tr><th>Student Name:</th><td>" << student->name << "</td></tr>";
        file << "<tr><th>Father's Name:</th><td>" << student->fatherName << "</td></tr>";
        file << "<tr><th>Mother's Name:</th><td>" << student->motherName << "</td></tr>";
        file << "<tr><th>Roll No:</th><td>" << student->rollNo << "</td></tr>";
        file << "<tr><th>Branch:</th><td>" << student->branch << "</td></tr>";
        file << "</table>";

        for (const auto& semester : student->semesterGrades) {
            file << "<div class='semester'>";
            file << "<h3>Semester " << semester.first << "</h3>";
           
            file << "<table class='grade-table'><tr><th>Course Code</th><th>Course Name</th><th>Credits</th><th>Grade</th></tr>";

            int totalCredits = 0;
            int obtainedCredits = 0;

            for (const auto& kvp : semester.second) {
                string courseCode = kvp.first;
                string grade = kvp.second;

                if (courses.find(courseCode) != courses.end()) {
                    Course* course = courses.at(courseCode);
                    int credits = course->credits;
                    totalCredits += credits;

                    if (Grade::gradePoints[grade] > 0) {
                        obtainedCredits += credits;
                    }

                    file << "<tr><td>" << course->courseCode << "</td><td>" << course->courseName << "</td><td>" << credits << "</td><td>" << grade << "</td></tr>";
                }
            }

            file << "</table>";
            file << "<p class='summary'><strong>Total Credits:</strong> " << totalCredits << "</p>";
            file << "<p class='summary'><strong>Total Credits Obtained:</strong> " << obtainedCredits << "</p>";
            file << "</div>";
             file << "<p class='summary'><strong>SGPA:</strong> " << fixed << setprecision(2) << student->sgpaPerSemester[semester.first] << "</p>";

        }
        file << "<p class='summary'><strong>CGPA:</strong> " << fixed << setprecision(2) << student->cgpa << "</p>";

        file << "</div>";
        file << "</body></html>";
        file.close();
        cout << "HTML report generated successfully!" << endl;
    } else {
        cout << "Unable to open file to write the report." << endl;
    }
}
void GenerateReportForStudent(int rollNo, vector<Student*>& students, map<string, Course*>& courses) {
    for (auto& student : students) {
        if (student->rollNo == rollNo) {
            for (const auto& semGrades : student->semesterGrades) {
                int semester = semGrades.first;
                student->sgpaPerSemester[semester] = CalculateSGPA(student, semester, courses);
            }

            student->cgpa = CalculateCGPA(student,courses);

            int studentOption;
            cout << "Select an option: \n1. SGPA for a specific semester\n2. CGPA\n3. Complete result\n";
            cin >> studentOption;

            if (studentOption == 1) {
                int semester;
                cout << "Enter the semester number: ";
                cin >> semester;

                if (student->sgpaPerSemester.find(semester) != student->sgpaPerSemester.end()) {
                    cout << "SGPA for Semester " << semester << ": " << student->sgpaPerSemester[semester] << endl;
                } else {
                    cout << "No grades available for Semester " << semester << endl;
                }
            } else if (studentOption == 2) {
                cout << "CGPA: " << student->cgpa << endl;
            } else if (studentOption == 3) {
                DisplayStudentData(student, courses);
            } else {
                cout << "Invalid option selected." << endl;
            }

            return;
        }
    }

    cout << "Student with roll number " << rollNo << " not found." << endl;
}


