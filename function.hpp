#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED

#include "ClassManager.hpp"

void eatline(void);

char showMainMenu(void);

char getChoice(void);

bool addStudent(Tree * ptree, Node *** list);

bool addStudents(Tree * ptree, Node *** list);

void showAllStudentsByID(Tree * ptree, Node ** list);

void showAllStudentsByScore(Tree * ptree, Node ** list);

double getTotalScore(Student  * student);

int getRank(Tree * ptree, Student * student, Node ** list);

bool searchStudentByName(Tree * ptree, Node ** list);

bool searchStudentByID(Tree * ptree, Node ** list);

bool removeStudentByID(Tree * ptree);

bool removeStudentByName(Tree * ptree);

#endif // FUNCTION_HPP_INCLUDED
