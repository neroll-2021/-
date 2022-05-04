#include <iostream>
#include "function.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
using namespace std;


int main(void)
{
    char choice;
    Tree tree;
    initTree(&tree);
    StudentList list = NULL;
    //showAllStudentsByID(&tree, list);
    readFromFile(&tree, &list);
    while ((choice = showMainMenu()) != 'q')
    {
        switch (choice)
        {
        case '1':
            addStudents(&tree, &list);
            break;
        case '2':
            showAllStudentsByID(&tree, list);
            break;
        case '3':
            showAllStudentsByScore(&tree, list);
            break;
        case '4':
            searchStudentByName(&tree, list);
            break;
        case '5':
            searchStudentByID(&tree, list);
            break;
        case '6':
            removeStudentByName(&tree, &list);
            break;
        case '7':
            removeStudentByID(&tree, &list);
            break;
        case '8':
            modifyStudent(&tree);
            break;
        case '9':
            showStudentNum(&tree);
            break;
        default:
            printf("switching error in main.\n");
            break;
        }
    }
    destroyTree(&tree);
    printf("bye");
    return 0;
}


