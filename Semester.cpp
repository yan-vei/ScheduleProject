#include "Semester.h"
#include "Course.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

Semester::Semester(int number)
{
	this->number = number;
}

Semester::Semester(int number, vector<Course*>& coursesOffered, Semester* nextSem, Semester* prevSem)
{
	this->number = number;
	this->coursesOffered = coursesOffered;
	this->nextSem = nextSem;
	this->prevSem = prevSem;
}

void Semester::setNumber(int num)
{
	this->number = num;
}

void Semester::setCoursesOffered(vector<Course*>& courses)
{
	this->coursesOffered = courses;
}

void Semester::setCourseTaken(Course& course)
{
	coursesTaken.push_back(&course);
}

void Semester::setNextSem(Semester& sem)
{
	this->nextSem = &sem;
}

void Semester::setPrevSem(Semester& sem)
{
	this->prevSem = &sem;
}

int Semester::getNumber() const
{
	return number;
}

vector<Course*>& Semester::getCoursesOffered()
{
	return coursesOffered;
}

vector<Course*>& Semester::getCoursesTaken()
{
	return coursesTaken;
}

Semester* Semester::getNextSem() const
{
	return nextSem;
}

Semester* Semester::getPrevSem() const
{
	return prevSem;
}

void Semester::displayCoursesTaken(vector<Semester*>& allSemesters)
{
	for (int i = 0; i < allSemesters.size(); i++)
	{
		cout << "Semester " << i + 1 << endl;

		vector<Course*> taken = allSemesters[i]->getCoursesTaken();
		for (int j = 0; j < taken.size(); j++)
		{
			cout << taken[j]->getName() << endl;
		}
	}
}