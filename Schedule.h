#pragma once
#include "Semester.h"
#include "Course.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Schedule
{
private:
	vector<Course*> coursesRequired;
	vector<Course*> allTaken;
	vector<Semester*> allSemesters;
	string major;
	string majorDesc;

public:
	Schedule() {};
	Schedule(string major, string majorDesc);
	Schedule(string major, string majorDesc, vector<Course*>& coursesRequired, vector<Semester*> allSemesters);

	void setMajor(string major);
	void setMajorDesc(string majorDesc);
	void setRequiredCourses(vector<Course*>& coursesRequired);
	void addToTaken(Course& course);
	void setSemesters(vector<Semester*>& allSemesters);

	string getMajor() const;
	string getMajorDesc() const;
	vector<Course*> getRequiredCourses() const;
	vector<Course*> getAllCourses() const;
	vector<Semester*> getSemesters() const;

	friend ostream& operator<< (ostream& output, const Schedule* schedule);

	void optimizeSchedule();
	bool doStandingsMatch(vector<Course*>& coursesTaken, Semester* currSem, int index);

	static void setSchedule(vector<Course*>& desiredCourses, Schedule& schedule, vector<Semester*>& allSemesters);
};
