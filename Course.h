#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <queue>
#include <string>

using namespace std;

class Course
{
private:
	string name, courseCode;
	vector<Course*> prerequisites;
	vector<Course*> coursesAfter;
	vector<string> namesOfPrerequisites;
	int numOfPrerequisites = 0;
	int standingRequired;

public:
	Course() {};
	Course(string name, string courseCode);
	Course(const Course& course);
	Course(string name, string courseCode, vector<Course*>& prerequisites);

	void setName(string name);
	void setCourseCode(string code);
	void setStanding(int standing);
	void setPrerequisites(vector<Course*>& prerequisites);
	void setNumPrerequisites();
	void setCourseAfter(Course& course);
	void setPrerequisitesNames();

	vector<Course*> getPrerequisites() const;
	vector<Course*> getCoursesAfter() const;
	vector<string> getNamesOfPrer() const;
	string getName() const;
	string getCourseCode() const;
	int getStanding() const;
	int getNumPrerequisites() const;

	bool operator==(const Course& course) const;

	void lowerNumPrerequisites();

	// topological sort by prerequisites
	static void sortByPrerequisites(vector<Course*>& sortedCourses, vector<Course*>& allCourses, map<string, vector<Course*>> mapOfCourses);

	// binary search
	static int search(vector<Course*> courses, string code);

	// merge sort by course code
	static void sortByCode(vector<Course*>& allCourses, vector<Course*>& tempCourses, int left, int right);
	static void sortByCode(vector<Course*>& allCourses);
	static void merge(vector<Course*>& allCourses, vector<Course*>& tempCourses, int leftPos, int rightPos, int rightEnd);
};
