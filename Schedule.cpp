#include "Schedule.h"
#include "Course.h"
#include "Semester.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

Schedule::Schedule(string major, string majorDesc)
{
	this->major = major;
	this->majorDesc = majorDesc;
}

Schedule::Schedule(string major, string majorDesc, vector<Course*>& coursesRequired, vector<Semester*> allSemesters)
{
	this->major = major;
	this->majorDesc = majorDesc;
	this->coursesRequired = coursesRequired;
	this->allSemesters = allSemesters;
}

void Schedule::setMajor(string major)
{
	this->major = major;
}

void Schedule::setMajorDesc(string majorDesc)
{
	this->majorDesc = majorDesc;
}

void Schedule::setRequiredCourses(vector<Course*>& coursesRequired)
{
	this->coursesRequired = coursesRequired;
}

void Schedule::setSemesters(vector<Semester*>& allSemesters)
{
	this->allSemesters = allSemesters;
}

string Schedule::getMajor() const
{
	return major;
}

string Schedule::getMajorDesc() const
{
	return majorDesc;
}

vector<Course*> Schedule::getRequiredCourses() const
{
	return coursesRequired;
}

vector<Course*> Schedule::getAllCourses() const
{
	return allTaken;
}

void Schedule::addToTaken(Course& course)
{
	allTaken.push_back(&course);
}

vector<Semester*> Schedule::getSemesters() const
{
	return allSemesters;
}

void Schedule::optimizeSchedule()
{
	// optimizing schedule according to the standing
	vector<Semester*> allSemesters = this->getSemesters();
	queue<Course*> mismatchedCourses;
	int numErased = 0;

	Semester* currSem = allSemesters[0];

	while (currSem != nullptr)
	{
		vector<Course*> coursesTaken = currSem->getCoursesTaken();

		for (int i = 0; i < coursesTaken.size(); i++)
		{
			bool standingsMatch = doStandingsMatch(coursesTaken, currSem, i);

			// if the standing and the number of the semester do not match, find the next semester to take the course
			if (!standingsMatch)
			{
				// if standings do not match, add the course to the queue
				mismatchedCourses.push(coursesTaken[i]);

				// if there are 2 or more mismatched courses, we should lower the index to stay in the range of that vector
				numErased++;
				int index = i;

				if (numErased > 1)
					index -= numErased - 1;

				currSem->getCoursesTaken().erase(currSem->getCoursesTaken().begin() + index);
			}
		}

		currSem = currSem->getNextSem();
	}

	while (!mismatchedCourses.empty())
	{
		Course* mismatchedCourse = mismatchedCourses.front();

		int standing = mismatchedCourse->getStanding();
		int semNumber;
		bool courseFound = false;
		int foundAt = -1;

		switch (standing)
		{
		case 2:
			semNumber = 3;
			break;
		case 3:
			semNumber = 5;
			break;
		case 4:
			semNumber = 6;
			break;
		default:
			semNumber = 0;
			break;
		}

		currSem = allSemesters[semNumber];

		// this will execute max (N - semNumber), because we don't check semesters before, where N - total number of semesters
		while (currSem != nullptr)
		{
			vector<Course*> coursesOffered = currSem->getCoursesOffered();
			foundAt = Course::search(coursesOffered, mismatchedCourse->getCourseCode());

			if (foundAt != -1)
			{
				currSem->setCourseTaken(*mismatchedCourse);
				courseFound = true;
				mismatchedCourses.pop();
				break;
			}

			currSem = currSem->getNextSem();
		}

		if (!courseFound)
		{
			mismatchedCourses.pop();
			cout << "Cannot be optimized";
		}

	}

	// the algorithm still takes O(N^2) time
}

bool Schedule::doStandingsMatch(vector<Course*>& coursesTaken, Semester* currSem, int index)
{
	bool standingsMatch = true;

	switch (coursesTaken[index]->getStanding())
	{
	case 2:
		if (currSem->getNumber() <= 2)
			standingsMatch = false;
		break;

	case 3:
		if (currSem->getNumber() <= 4)
			standingsMatch = false;
		break;
	case 4:
		if (currSem->getNumber() <= 6)
			standingsMatch = false;
		break;
	}

	return standingsMatch;
}



void Schedule::setSchedule(vector<Course*>& desiredCourses, Schedule& schedule, vector<Semester*>& allSemesters)
{
	// if prerequisite is taken during the same semester that the course we are checking
	bool prerSameSem = false;

	// get first semester from the vector
	Semester* currSem = allSemesters[0];

	while (desiredCourses.size() != 0 && currSem != nullptr)
	{
		// get courses offered during that semester
		vector<Course*> currCourses = currSem->getCoursesOffered();

		for (int i = 0; i < desiredCourses.size(); i++)
		{
			// find the course first in the topological order
			int foundAt = Course::search(currCourses, desiredCourses[i]->getCourseCode());
			vector<Course*> coursesTaken = currSem->getCoursesTaken();

			// if that course not found, we cannot take other courses before that one, so move on to another semester
			if (foundAt == -1)
				break;
			else
			{
				vector<Course*> prerequisites = desiredCourses[i]->getPrerequisites();

				for (int j = 0; j < prerequisites.size(); j++)
				{
					// if the prerequisite of the course is taken during that semester,
					// we cannot take the course during that semester and have to advance to the next one
					int indexTaken = Course::search(coursesTaken, prerequisites[j]->getCourseCode());
					int indexCurr = Course::search(currCourses, prerequisites[j]->getCourseCode());

					if (indexTaken != -1 && indexCurr != -1)
					{
						prerSameSem = true;
						break;
					}
					else
					{
						prerSameSem = false;
					}
				}

			}

			if (!prerSameSem)
			{
				// if there is such course, add it to the schedule, put it to the courses taken during that semester and erase it from desired
				// lower i so we don't jump over courses
				schedule.addToTaken(*currCourses[foundAt]);
				currSem->setCourseTaken(*currCourses[foundAt]);
				desiredCourses.erase(desiredCourses.begin() + i);
				i--;
			}
		}
		prerSameSem = false;

		// advance to the next semester
		currSem = currSem->getNextSem();
	}


	if (currSem == nullptr && desiredCourses.size() != 0)
	{
		cout << "Not possible to take courses: ";
		for (int i = 0; i < desiredCourses.size(); i++)
		{
			cout << desiredCourses[i]->getCourseCode() << " ";
		}
		cout << endl;
	}
}

ostream& operator<<(ostream& output, const Schedule* schedule)
{
	string scheduleRepr = "Major: " + schedule->getMajor() + "\n" + schedule->getMajorDesc() + "\n\n" + "Required courses:" + "\n";

	for (int i = 0; i < schedule->getRequiredCourses().size(); i++)
	{
		scheduleRepr += schedule->getRequiredCourses()[i]->getCourseCode() + " " + schedule->getRequiredCourses()[i]->getName() + "\n";
	}
	scheduleRepr += "\n";

	vector<Semester*> allSemesters = schedule->getSemesters();

	for (int i = 0; i < allSemesters.size(); i++)
	{
		int index = i + 1;
		scheduleRepr += "Semester " + to_string(index);

		vector<Course*> taken = allSemesters[i]->getCoursesTaken();
		for (int j = 0; j < taken.size(); j++)
		{
			scheduleRepr += "\n" + taken[j]->getCourseCode() + " " + taken[j]->getName();
		}
		scheduleRepr += "\n\n";
	}

	output << scheduleRepr;

	return output;
}

