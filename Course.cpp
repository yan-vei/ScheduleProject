#include "Course.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Course::Course(string name, string courseCode)
{
	this->name = name;
	this->courseCode = courseCode;
}

Course::Course(const Course& course)
{
	name = course.name;
	courseCode = course.courseCode;
	prerequisites = course.prerequisites;
}

Course::Course(string name, string courseCode, vector<Course*>& prerequisites)
{
	this->name = name;
	this->courseCode = courseCode;
	this->prerequisites = prerequisites;
}

void Course::setName(string name)
{
	this->name = name;
}

void Course::setCourseCode(string code)
{
	this->courseCode = code;
}

void Course::setStanding(int standing)
{
	this->standingRequired = standing;
}

void Course::setPrerequisites(vector<Course*>& prerequisites)
{
	this->prerequisites = prerequisites;
}

bool Course::operator==(const Course& course) const
{
	if ((this->name == course.name) && (this->courseCode == course.courseCode))
		return true;
	return false;
}

vector<Course*> Course::getPrerequisites() const
{
	return prerequisites;
}

string Course::getName() const
{
	return name;
}

string Course::getCourseCode() const
{
	return courseCode;
}

int Course::getStanding() const
{
	return standingRequired;
}

void Course::sortByPrerequisites(vector<Course*>& sortedCourses, vector<Course*>& desiredCourses, map<string, vector<Course*>> mapOfCourses)
{
	sortedCourses.clear();

	// sort the courses beforehand
	Course::sortByCode(desiredCourses);

	string courseName;
	vector<Course*> coursesAfter;
	queue<Course*> coursesForSorting;

	// if a course has no prerequisites, move it to the queue
	for (int i = 0; i < desiredCourses.size(); i++)
	{
		if (desiredCourses[i]->getNumPrerequisites() == 0)
			coursesForSorting.push(desiredCourses[i]);
	}

	while (!coursesForSorting.empty())
	{
		// get a course from the front of the queue and all the courses that go after it
		Course* removedCourse = coursesForSorting.front();
		coursesForSorting.pop();

		courseName = removedCourse->getCourseCode();
		coursesAfter = mapOfCourses.at(courseName);

		// for each course after, lower the indegree of that course
		for (int i = 0; i < coursesAfter.size(); i++)
		{
			coursesAfter[i]->lowerNumPrerequisites();

			// if an indegree of the course is 0, put it on the queue
			if (coursesAfter[i]->getNumPrerequisites() == 0)
				coursesForSorting.push(coursesAfter[i]);
		}

		sortedCourses.push_back(removedCourse);
	}

}

// binary search based on course code
int Course::search(vector<Course*> courses, string code)
{
	int lower = 0, higher = courses.size() - 1;
	while (lower <= higher) {
		int middle = lower + (higher - lower) / 2;

		if ((*courses[middle]).getCourseCode().substr(3, 3) == code.substr(3, 3))
			return middle;

		if ((*courses[middle]).getCourseCode().substr(3, 3) < code.substr(3, 3))
			lower = middle + 1;

		else
			higher = middle - 1;
	}

	return -1;
}

void Course::sortByCode(vector<Course*>& allCourses, vector<Course*>& tempCourses, int left, int right)
{
	if (left < right)
	{
		int center = (left + right) / 2;
		sortByCode(allCourses, tempCourses, left, center);
		sortByCode(allCourses, tempCourses, center + 1, right);
		merge(allCourses, tempCourses, left, center + 1, right);
	}
}

void Course::sortByCode(vector<Course*>& allCourses)
{
	vector<Course*> tmpItems(allCourses.size());
	sortByCode(allCourses, tmpItems, 0, allCourses.size() - 1);
}

void Course::merge(vector<Course*>& allCourses, vector<Course*>& tempCourses, int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	while (leftPos <= leftEnd && rightPos <= rightEnd)
		if (allCourses[leftPos]->getCourseCode().substr(3, 3) < allCourses[rightPos]->getCourseCode().substr(3, 3))
			tempCourses[tmpPos++] = allCourses[leftPos++];
		else
			tempCourses[tmpPos++] = allCourses[rightPos++];

	while (leftPos <= leftEnd)
		tempCourses[tmpPos++] = allCourses[leftPos++];

	while (rightPos <= rightEnd)
		tempCourses[tmpPos++] = allCourses[rightPos++];

	for (int i = 0; i < numElements; i++, rightEnd--)
		allCourses[rightEnd] = tempCourses[rightEnd];
}

vector<Course*> Course::getCoursesAfter() const
{
	return coursesAfter;
}

vector<string> Course::getNamesOfPrer() const
{
	return namesOfPrerequisites;
}

void Course::setCourseAfter(Course& course)
{
	coursesAfter.push_back(&course);
}
void Course::setPrerequisitesNames()
{
	for (int i = 0; i < this->getPrerequisites().size(); i++)
	{
		namesOfPrerequisites.push_back(this->getPrerequisites()[i]->getCourseCode());
	}
}
int Course::getNumPrerequisites() const
{
	return numOfPrerequisites;
}
void Course::setNumPrerequisites()
{
	numOfPrerequisites = prerequisites.size();
}
void Course::lowerNumPrerequisites()
{
	--numOfPrerequisites;
}

