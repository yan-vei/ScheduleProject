#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include "Course.h"
#include "Semester.h"
#include "Schedule.h"

using namespace std;

void getCourseData(string);
void initializeCourses();
void setRequiredCourses(string, Schedule*);

void getSemesterData(string);
void initializeSemester(int);
void setupSemesters();

vector<string> names, codes, prerequisites, standings, semesterNames, semesterCodes;
vector<Course*> allCourses, sortedCourses, desiredCourses;
vector<Semester*> allSemesters;
map<string, vector<Course*>> mapOfCourses;

int main()
{
	int userInput;
	Schedule* schedule = new Schedule();

	getCourseData("Courses.csv");
	initializeCourses();
	Course::sortByCode(allCourses);
	Course::sortByPrerequisites(sortedCourses, allCourses, mapOfCourses);

	getSemesterData("Semester");
	setupSemesters();

	cout << "Choose major: 0 - COS, 1 - INF. Enter -99 to exit" << endl;
	cin >> userInput;

	while (userInput != -99)
	{
		if (userInput == 0)
		{
			schedule->setMajor("COS");
			schedule->setMajorDesc("The Computer Science program focuses on helping students gain the necessary skills to become productive leaders in computing and related industries. As such, our graduates are expected to be highly competent, well-qualified computer professionals with broad training in software development.");
			cout << "Mandatory courses for COS major are: COS120, COS150, COS221, COS230, COS235, COS315, COS491" << endl;
			cout << "You can choose up to 5 electives" << endl;
			setRequiredCourses("COS.csv", schedule);
		}
		else if (userInput == 1)
		{
			schedule->setMajor("INF");
			schedule->setMajorDesc("The major in Information Systems is designed to train professionals who are competent to solve problems in a huge and growing area of computer - based information systems and services.The major provides a broad understanding of the role played by modern computer and communication technologies in every area of human activities.");
			cout << "Mandatory courses for INF major are: INF130, INF270, INF280, INF335, INF440, INF492" << endl;
			cout << "You can choose up to 6 electives" << endl;
			setRequiredCourses("INF.csv", schedule);
		}
		else
		{
			cout << "Incorrect input. Try again!" << endl;
		}

		cout << "These are available courses: " << endl;
		for (int i = 0; i < allCourses.size(); i++)
		{
			allCourses[i]->setPrerequisitesNames();
			cout << i << " - " << allCourses[i]->getCourseCode() << " " << allCourses[i]->getName() << " | Prerequisites: ";
			if (allCourses[i]->getNamesOfPrer().size() == 0)
			{
				cout << "None";
			}
			else {
				for (int j = 0; j < allCourses[i]->getNamesOfPrer().size(); j++)
				{
					cout << allCourses[i]->getNamesOfPrer()[j] << " ";
				}
			}
			cout << endl;
		}
		cout << "To choose electives, enter the number of the course (0, 1, 2...). Also, don't forget to add their prerequisites!\nEnter -99 to stop choosing" << endl;
		break;
	}

	while (userInput != -99)
	{
		cin >> userInput;

		if (userInput == -99)
		{
			break;
		}

		else if (userInput <= allCourses.size() && userInput > -1)
		{
			int index = Course::search(desiredCourses, allCourses[userInput]->getCourseCode());

			if (index == -1)
			{
				desiredCourses.push_back(allCourses[userInput]);
			}
			else
			{
				cout << "Already added\n";
			}
		}

		else
		{
			cerr << "Invalid input! Try again";
		}
	}

	Course::sortByPrerequisites(sortedCourses, desiredCourses, mapOfCourses);

	schedule->setSemesters(allSemesters);

	Schedule::setSchedule(sortedCourses, *schedule, allSemesters);

	cout << "Type: 0 - display schedule in the console; 1 - load schedule into a file; 2 - optimize schedule, 3 - to get information about courses & -99 to exit\n";
	cin >> userInput;

	while (userInput != -99)
	{
		if (userInput == 0)
		{
			Semester::displayCoursesTaken(allSemesters);
		}

		else if (userInput == 1)
		{
			ofstream scheduleFile("Schedule.txt");
			scheduleFile << schedule;

			if (scheduleFile.fail())
				cerr << "Error opening file";

			scheduleFile.close();

			cout << "Schedule loaded into a file successfully!\n";
		}

		else if (userInput == 2)
		{
			schedule->optimizeSchedule();
			cout << "Schedule optimized successfully!\n";
		}

		else if (userInput == 3)
		{
			cout << "Enter the number of the course to get information about it:\n";
			cin >> userInput;

			if (userInput > -1 && userInput < allCourses.size())
			{
				cout << allCourses[userInput]->getCourseCode() << " " << allCourses[userInput]->getName() << endl;
				cout << "Standing required: " << allCourses[userInput]->getStanding() << endl;

			}
			else
			{
				cerr << "Invalid input! Try again\n" << endl;
			}
		}

		else
		{
			cerr << "Invalid input, try again!" << endl;
		}

		cin >> userInput;
	}

}

void getCourseData(string filename)
{
	ifstream courses(filename);
	string line, name, code, prerequisite, standing;;
	vector<string> courseData;

	if (courses.fail())
	{
		cerr << "Error opening file";
		return;
	}

	// skipping the header
	getline(courses, line);

	// extract all data into a vector
	while (getline(courses, line, '\n'))
	{
		courseData.push_back(line);
	}

	courses.close();

	// separate data into different vectors for further initialization of objects
	for (int i = 0; i < courseData.size(); i++)
	{
		stringstream data(courseData[i]);

		getline(data, code, ',');
		getline(data, name, ',');
		getline(data, prerequisite, ',');
		getline(data, standing, ',');

		codes.push_back(code);
		names.push_back(name);
		prerequisites.push_back(prerequisite);
		standings.push_back(standing);
	}
}

void initializeCourses()
{
	// initialze objects of the Course class
	for (int i = 0; i < names.size(); i++)
	{
		Course* course = new Course(names[i], codes[i]);
		allCourses.push_back(course);
	}

	// set standings for each object
	for (int i = 0; i < standings.size(); i++)
	{
		if (standings[i] == "Senior")
		{
			allCourses[i]->setStanding(4);
		}
		else if ((standings[i]) == "Junior")
		{
			allCourses[i]->setStanding(3);
		}
		else if (standings[i] == "Sophomore")
		{
			allCourses[i]->setStanding(2);
		}
		else
		{
			allCourses[i]->setStanding(1);
		}
	}

	// sort courses by code for binary search later
	Course::sortByCode(allCourses);

	for (int i = 0; i < prerequisites.size(); i++)
	{
		if (prerequisites[i] != "")
		{
			// for each course that has prerequisites, set prerequisites and its number
			int mainCourse = Course::search(allCourses, codes[i]);
			int prerCourse = Course::search(allCourses, prerequisites[i]);

			vector<Course*> prerCourses;
			prerCourses.push_back(allCourses[prerCourse]);

			allCourses[mainCourse]->setPrerequisites(prerCourses);
			allCourses[mainCourse]->setNumPrerequisites();
			allCourses[prerCourse]->setCourseAfter(*allCourses[mainCourse]);
		}
	}

	// initialize map of courses used in topological sort
	for (int i = 0; i < allCourses.size(); i++)
	{
		mapOfCourses[allCourses[i]->getCourseCode()] = allCourses[i]->getCoursesAfter();
	}
}

void getSemesterData(string filename)
{
	for (int i = 1; i < 9; i++)
	{
		string nameOfFile = filename + " " + to_string(i) + ".csv";
		ifstream semester(nameOfFile);
		string line, name, code;
		vector<string> semesterData;

		if (semester.fail())
		{
			cerr << "Error opening file";
			return;
		}

		// skipping the header
		getline(semester, line);

		// extract all data into a vector
		while (getline(semester, line, '\n'))
		{
			semesterData.push_back(line);
		}

		semester.close();

		// separate data into different vectors for further initialization of objects
		for (int i = 0; i < semesterData.size(); i++)
		{
			stringstream data(semesterData[i]);

			getline(data, code, ',');
			getline(data, name, ',');

			semesterCodes.push_back(code);
			semesterNames.push_back(name);
		}

		initializeSemester(i);
		semesterCodes.clear();
		semesterNames.clear();
	}
}

void initializeSemester(int semIndex)
{
	Semester* semester = new Semester(semIndex);
	vector<Course*> coursesOffered;

	for (int i = 0; i < semesterNames.size(); i++)
	{
		int courseOffered = Course::search(allCourses, semesterCodes[i]);
		coursesOffered.push_back(allCourses[courseOffered]);
		semester->setCoursesOffered(coursesOffered);
	}

	Course::sortByCode(semester->getCoursesOffered());
	allSemesters.push_back(semester);
}

void setupSemesters()
{
	for (int i = 0; i < allSemesters.size(); i++)
	{
		if (i != 0)
		{
			allSemesters[i]->setPrevSem(*allSemesters[i - 1]);
		}

		if (i != allSemesters.size() - 1)
		{
			allSemesters[i]->setNextSem(*allSemesters[i + 1]);
		}
	}
}

void setRequiredCourses(string filename, Schedule* schedule)
{
	ifstream courses(filename);
	string line;

	if (courses.fail())
	{
		cerr << "Error opening file";
		return;
	}

	while (getline(courses, line, '\n'))
	{
		int index = Course::search(allCourses, line);
		desiredCourses.push_back(allCourses[index]);
	}

	schedule->setRequiredCourses(desiredCourses);
}



