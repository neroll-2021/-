#include "function.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

// extern bool isExisted(const Tree * ptree, const char * id);

extern bool IDIsValid(const char * id);

extern bool nameIsValid(const char * name);

static void s_gets(char * s, int n);

static double getScore(void);

static bool isInBound(double score);

void eatline(void)
{
    while (getchar() != '\n')
        continue;
}

char showMainMenu(void)
{
    printf("%30c====================================================\n",' ');
    printf("%40c 1) 添加学生\n", ' ');
    printf("%40c 2) 按学号顺序显示所有学生\n", ' ');
    printf("%40c 3) 按成绩排名显示所有学生\n", ' ');
    printf("%40c 4) 根据姓名查找学生，并显示学生信息\n", ' ');
    printf("%40c 5) 根据学号查找学生，并显示学生信息\n", ' ');
    printf("%40c 6) 根据姓名查找学生，并删除学生信息\n", ' ');
    printf("%40c 7) 根据学号查找学生，并删除学生信息\n", ' ');
    printf("%40c 8) 根据学号查找学生，并修改学生信息\n", ' ');
    printf("%40c 9) 统计学生人数\n", ' ');
    printf("%40c q) 退出\n", ' ');
    printf("%30c=====================================================\n",' ');
    return getChoice();
}

char getChoice(void)
{
    char choice = getchar();

    while (strchr("12345678q", choice) == NULL)
    {
        eatline();
        printf("请输入1,2,3,4,5,6,7,8 或 q\n");
        choice = getchar();

    }
    eatline();
    return choice;
}

/**
 * 添加学生
 * @param 树的指针
 * @return 若添加成功，则返回true，否则返回false
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
 /*
bool addStudent(Tree * ptree, Node *** list)
{
    Student student;
    char id[ID_LENGTH];
    printf("请输入学生的学号(输入q退出):\n");
    s_gets(id, ID_LENGTH);
    if (id[0] == 'q') return false;
    bool isValid;
    bool existed;
    while (!(isValid = IDIsValid(id)) || (existed = isExisted(ptree, id)))
    {
        if (!isValid)
        {
            printf("学号只能是纯数字，请重试(输入q退出):\n");
        }
        else if (existed)
        {
            printf("该学生已存在，请重试(输入q退出):\n");
        }
        s_gets(id, ID_LENGTH);
        if (id[0] == 'q') return false;
    }
    strcpy(student.id, id);
    printf("请输入学生的姓名:\n");
    s_gets(student.name, NAME_LENGTH);
    while (!nameIsValid(student.name))
    {
        printf("学生姓名不合法，请重试:\n");
        s_gets(student.name, NAME_LENGTH);
    }
    printf("请输入学生的语文成绩:\n");
    student.chineseScore = getScore();
    printf("请输入学生的数学成绩:\n");
    student.mathScore = getScore();
    printf("请输入学生的英语成绩:\n");
    student.englishScore = getScore();
    bool ret = insert(ptree, &student);
    if (ret == false) return ret;
    if (*list != NULL)
        free(*list);
    *list = makeStudentListSortedByScore(ptree);
    return ret;
}*/

/**
 * 添加学生
 * @param 树的指针
 * @return 若添加成功，则返回true，否则返回false
 * @version 0.2.0
 * @date 2022-05-03
 * @author Neroll
 * 注意，这里曾经出了bug，原因是三级指针list被错误地解引用
 */
bool addStudents(Tree * ptree, Node *** list)
{
    Student student;
    char id[ID_LENGTH];
    printf("请输入学生的学号(输入q退出):\n");
    s_gets(id, ID_LENGTH);
    int counter = 0;
    while (id[0] != 'q')
    {
        bool isValid;
        bool existed;
        while (!(isValid = IDIsValid(id)) || (existed = isExisted(ptree, id)))
        {
            if (!isValid)
            {
                printf("学号只能是纯数字，请重试(输入q退出):\n");
            }
            else if (existed)
            {
                printf("该学生已存在，请重试(输入q退出):\n");
            }
            s_gets(id, ID_LENGTH);
            if (id[0] == 'q') return false;
        }
        strcpy(student.id, id);
        printf("请输入学生的姓名:\n");
        s_gets(student.name, NAME_LENGTH);
        while (!nameIsValid(student.name))
        {
            printf("学生姓名不合法，请重试:\n");
            s_gets(student.name, NAME_LENGTH);
        }
        printf("请输入学生的语文成绩:\n");
        student.chineseScore = getScore();
        printf("请输入学生的数学成绩:\n");
        student.mathScore = getScore();
        printf("请输入学生的英语成绩:\n");
        student.englishScore = getScore();
        if (insert(ptree, &student))
        {
            printf("添加成功!\n");
            printf("请输入下一位学生的学号(输入q退出):\n");
            s_gets(id, ID_LENGTH);
            counter++;
        }
        else
        {
            printf("Failed to insert in %d in function.cpp", __LINE__);
            return false;
        }
        if (counter % 5 == 0)
        {
            if (*list != NULL)
                free(*list);
            *list = makeStudentListSortedByScore(ptree);
            counter = 0;
        }
    }
    if (counter % 5 != 0)
    {
        if (*list != NULL)
            free(*list);
        *list = makeStudentListSortedByScore(ptree);
    }
    //************************************
    //puts("eeeeeeeeee");
    //assert(ptree->size == counter);
    //printf("tree size=%d\n",ptree->size);
    /*
    for (int i=0;i<ptree->size;i++)
    {
        //printf("i=%d  ",i);
        printf("%s %s %.0lf %.0lf %.0lf %.0lf %d\n",
               ((*list)[i])->student.id,((*list)[i])->student.name,
               ((*list)[i])->student.chineseScore,
               ((*list)[i])->student.mathScore,
               ((*list)[i])->student.englishScore,
               getTotalScore(&(((*list)[i])->student)),
               getRank(ptree,&(((*list)[i])->student),*list));
    }*/
    //************************************
    return true;
}


static double getScore(void)
{
    double score;
    bool isValid;
    while (!(isValid = scanf("%lf", &score) == 1)
            || !isInBound(score))
    {
        if (!isValid)
        {
            printf("输入不合法，请重试:\n");
            eatline();
        }
        else
        {
            printf("分数只能在0-100之间，请重试:\n");
        }
    }
    eatline();
    return score;
}

static bool isInBound(double score)
{
    return score >= 0 && score <= 100;
}

double getTotalScore(Student  * student)
{
    //puts("xxxxx");
    return student->chineseScore
           + student->englishScore + student->mathScore;
}

static void s_gets(char * s, int n)
{
    s = fgets(s, n, stdin);
    while (*s)
    {
        if (*s == '\n')
        {
            *s = '\0';
        }
        s++;
    }
}

static void printLine(void)
{
    printf("%8c", ' ');
    for (int i = 1; i <= 101; i++)
        printf("-");
    putchar('\n');
}

static void printTableHeader(void)
{
    printLine();
    printf("%8c %-15s %-15s %-15s %-15s %-15s %-15s %-8s\n",
           ' ', "学号", "姓名", "语文", "数学",
           "英语", "总分", "排名");
    printLine();
}

static void printStudentInfo(Tree * ptree, Student * student,
                             StudentList list)
{
    printf("%8c %-15s %-16s %-15.0lf %-15.0lf %-15.0lf %-15.0lf %-8d\n",
           ' ', student->id, student->name, student->chineseScore,
           student->mathScore, student->englishScore,
           getTotalScore(student), getRank(ptree, student, list));
    // get rank of a student
}

void showAllStudentsByID(Tree * ptree, Node ** list)
{
    if (ptree->size == 0)
    {
        printf("当前还没有学生\n");
        return;
    }
    printTableHeader();
    inOrderTraverseInID(ptree->IDRoot, ptree, list, printStudentInfo);
    printLine();
}

void showAllStudentsByScore(Tree * ptree, Node ** list)
{
    if (ptree->size == 0)
    {
        printf("当前还没有学生\n");
        return;
    }
    printTableHeader();
    for (int i = 0; i < ptree->size; i++)
    {
        printStudentInfo(ptree, &(list[i]->student), list);
    }
    printLine();
}

bool searchStudentByName(Tree * ptree, Node ** list)
{
    if (ptree->size == 0)
    {
        printf("当前还没有学生\n");
        return false;
    }
    printf("请输入学生的姓名(输入q退出):\n");
    char name[NAME_LENGTH];
    s_gets(name, NAME_LENGTH);
    if (name[0] == 'q')
        return false;
    while (!nameIsValid(name))
    {
        printf("姓名不合法，请重试(输入q退出):\n");
        s_gets(name, NAME_LENGTH);
        if (name[0] == 'q')
            return false;
    }
    Node * node;
    if ((node = searchByName(ptree, name)) != NULL)
    {
        printTableHeader();
        printStudentInfo(ptree, &(node->student), list);
        printLine();
        return true;
    }
    else
    {
        printf("没有找到该学生\n");
        return false;
    }
}

bool searchStudentByID(Tree * ptree, Node ** list)
{
    if (ptree->size == 0)
    {
        printf("当前还没有学生\n");
        return false;
    }
    printf("请输入学生的学号(输入q退出):\n");
    char id[ID_LENGTH];
    s_gets(id, ID_LENGTH);
    if (id[0] == 'q')
        return false;
    while (!IDIsValid(id))
    {
        printf("学号不合法，请重试(输入q退出):\n");
        s_gets(id, ID_LENGTH);
        if (id[0] == 'q')
            return false;
    }
    Node * node;
    if ((node = searchByID(ptree, id)) != NULL)
    {
        printTableHeader();
        printStudentInfo(ptree, &(node->student), list);
        printLine();
        return true;
    }
    else
    {
        printf("没有找到该学生\n");
        return false;
    }
}

bool removeStudentByID(Tree * ptree)
{
    if (ptree->size == 0)
    {
        printf("当前还没有学生\n");
        return false;
    }
    printf("请输入学生的学号(输入q退出):\n");
    char id[ID_LENGTH];
    s_gets(id, ID_LENGTH);
    if (id[0]=='q') return false;
    while (!IDIsValid(id))
    {
        printf("学号不合法，请重试:\n");
        s_gets(id, ID_LENGTH);
        if (id[0] == 'q') return false;
    }
    if (removeByID(ptree, id))
    {
        printf("删除成功!\n");
        return true;
    }
    else
    {
        printf("没有找到该学生\n");
        return false;
    }
}

bool removeStudentByName(Tree * ptree)
{
    if (ptree->size == 0)
    {
        printf("当前还没有学生\n");
        return false;
    }
    printf("请输入学生的姓名(输入q退出):\n");
    char name[NAME_LENGTH];
    s_gets(name, NAME_LENGTH);
    if (name[0]=='q') return false;
    while (!nameIsValid(name))
    {
        printf("姓名不合法，请重试:\n");
        s_gets(name, NAME_LENGTH);
        if (name[0] == 'q') return false;
    }
    if (removeByName(ptree, name))
    {
        printf("删除成功!\n");
        return true;
    }
    else
    {
        printf("没有找到该学生\n");
        return false;
    }
}
