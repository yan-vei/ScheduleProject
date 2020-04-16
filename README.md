# ScheduleProject

This project is used to create an 8 semester long schedule for a student majoring in Computer Science/Information Systems.
For its purpose, the program employs 3 classes: Course, Semester and Schedule.

Algorithms used are: binary search (by courses' codes), topological sort (in linear time, implemented with a queue) and merge sort (based on
courses' codes).

The console app downloads a list of courses, required standing and their prerequisites from a file, and than it prompts user to pick a major,
so the required courses can be downloaded as well. The user is free to add as many electives as he wishes, however he also has to add
prerequisites.

Then, the schedule is displayed disregarding the standing required. If the user wants to get the schedule according to the standing, 
there is an option to optimize it and save the final variant to a file.

On request, the user can get additional information about courses.
