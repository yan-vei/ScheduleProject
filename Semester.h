#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Course.h"

using namespace std;

class Semester
{
private:
	int number;
	vector<Course*> coursesOffered;
	vector<Course*> coursesTaken;
	Semester* nextSem = nullptr;
	Semester* prevSem = nullptr;

public:
	Semester(int number);
	Semester(int number, vector<Course*>& coursesOffered, Semester* nextSem = nullptr, Semester* prevSem = nullptr);

	void setNumber(int num);
	void setCoursesOffered(vector<Course*>& courses);
	void setCourseTaken(Course& course);
	void setNextSem(Semester& sem);
	void setPrevSem(Semester& sem);

	int getNumber() const;
	vector<Course*>& getCoursesOffered();
	vector<Course*>& getCoursesTaken();
	Semester* getNextSem() const;
	Semester* getPrevSem() const;

	static void displayCoursesTaken(vector<Semester*>& allSemesters);
};
