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
    printf("%40c 1) ���ѧ��\n", ' ');
    printf("%40c 2) ��ѧ��˳����ʾ����ѧ��\n", ' ');
    printf("%40c 3) ���ɼ�������ʾ����ѧ��\n", ' ');
    printf("%40c 4) ������������ѧ��������ʾѧ����Ϣ\n", ' ');
    printf("%40c 5) ����ѧ�Ų���ѧ��������ʾѧ����Ϣ\n", ' ');
    printf("%40c 6) ������������ѧ������ɾ��ѧ����Ϣ\n", ' ');
    printf("%40c 7) ����ѧ�Ų���ѧ������ɾ��ѧ����Ϣ\n", ' ');
    printf("%40c 8) ����ѧ�Ų���ѧ�������޸�ѧ����Ϣ\n", ' ');
    printf("%40c 9) ͳ��ѧ������\n", ' ');
    printf("%40c q) �˳�\n", ' ');
    printf("%30c=====================================================\n",' ');
    return getChoice();
}

char getChoice(void)
{
    char choice = getchar();

    while (strchr("12345678q", choice) == NULL)
    {
        eatline();
        printf("������1,2,3,4,5,6,7,8 �� q\n");
        choice = getchar();

    }
    eatline();
    return choice;
}

/**
 * ���ѧ��
 * @param ����ָ��
 * @return ����ӳɹ����򷵻�true�����򷵻�false
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
 /*
bool addStudent(Tree * ptree, Node *** list)
{
    Student student;
    char id[ID_LENGTH];
    printf("������ѧ����ѧ��(����q�˳�):\n");
    s_gets(id, ID_LENGTH);
    if (id[0] == 'q') return false;
    bool isValid;
    bool existed;
    while (!(isValid = IDIsValid(id)) || (existed = isExisted(ptree, id)))
    {
        if (!isValid)
        {
            printf("ѧ��ֻ���Ǵ����֣�������(����q�˳�):\n");
        }
        else if (existed)
        {
            printf("��ѧ���Ѵ��ڣ�������(����q�˳�):\n");
        }
        s_gets(id, ID_LENGTH);
        if (id[0] == 'q') return false;
    }
    strcpy(student.id, id);
    printf("������ѧ��������:\n");
    s_gets(student.name, NAME_LENGTH);
    while (!nameIsValid(student.name))
    {
        printf("ѧ���������Ϸ���������:\n");
        s_gets(student.name, NAME_LENGTH);
    }
    printf("������ѧ�������ĳɼ�:\n");
    student.chineseScore = getScore();
    printf("������ѧ������ѧ�ɼ�:\n");
    student.mathScore = getScore();
    printf("������ѧ����Ӣ��ɼ�:\n");
    student.englishScore = getScore();
    bool ret = insert(ptree, &student);
    if (ret == false) return ret;
    if (*list != NULL)
        free(*list);
    *list = makeStudentListSortedByScore(ptree);
    return ret;
}*/

/**
 * ���ѧ��
 * @param ����ָ��
 * @return ����ӳɹ����򷵻�true�����򷵻�false
 * @version 0.2.0
 * @date 2022-05-03
 * @author Neroll
 * ע�⣬������������bug��ԭ��������ָ��list������ؽ�����
 */
bool addStudents(Tree * ptree, Node *** list)
{
    Student student;
    char id[ID_LENGTH];
    printf("������ѧ����ѧ��(����q�˳�):\n");
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
                printf("ѧ��ֻ���Ǵ����֣�������(����q�˳�):\n");
            }
            else if (existed)
            {
                printf("��ѧ���Ѵ��ڣ�������(����q�˳�):\n");
            }
            s_gets(id, ID_LENGTH);
            if (id[0] == 'q') return false;
        }
        strcpy(student.id, id);
        printf("������ѧ��������:\n");
        s_gets(student.name, NAME_LENGTH);
        while (!nameIsValid(student.name))
        {
            printf("ѧ���������Ϸ���������:\n");
            s_gets(student.name, NAME_LENGTH);
        }
        printf("������ѧ�������ĳɼ�:\n");
        student.chineseScore = getScore();
        printf("������ѧ������ѧ�ɼ�:\n");
        student.mathScore = getScore();
        printf("������ѧ����Ӣ��ɼ�:\n");
        student.englishScore = getScore();
        if (insert(ptree, &student))
        {
            printf("��ӳɹ�!\n");
            printf("��������һλѧ����ѧ��(����q�˳�):\n");
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
            printf("���벻�Ϸ���������:\n");
            eatline();
        }
        else
        {
            printf("����ֻ����0-100֮�䣬������:\n");
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
           ' ', "ѧ��", "����", "����", "��ѧ",
           "Ӣ��", "�ܷ�", "����");
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
        printf("��ǰ��û��ѧ��\n");
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
        printf("��ǰ��û��ѧ��\n");
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
        printf("��ǰ��û��ѧ��\n");
        return false;
    }
    printf("������ѧ��������(����q�˳�):\n");
    char name[NAME_LENGTH];
    s_gets(name, NAME_LENGTH);
    if (name[0] == 'q')
        return false;
    while (!nameIsValid(name))
    {
        printf("�������Ϸ���������(����q�˳�):\n");
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
        printf("û���ҵ���ѧ��\n");
        return false;
    }
}

bool searchStudentByID(Tree * ptree, Node ** list)
{
    if (ptree->size == 0)
    {
        printf("��ǰ��û��ѧ��\n");
        return false;
    }
    printf("������ѧ����ѧ��(����q�˳�):\n");
    char id[ID_LENGTH];
    s_gets(id, ID_LENGTH);
    if (id[0] == 'q')
        return false;
    while (!IDIsValid(id))
    {
        printf("ѧ�Ų��Ϸ���������(����q�˳�):\n");
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
        printf("û���ҵ���ѧ��\n");
        return false;
    }
}

bool removeStudentByID(Tree * ptree)
{
    if (ptree->size == 0)
    {
        printf("��ǰ��û��ѧ��\n");
        return false;
    }
    printf("������ѧ����ѧ��(����q�˳�):\n");
    char id[ID_LENGTH];
    s_gets(id, ID_LENGTH);
    if (id[0]=='q') return false;
    while (!IDIsValid(id))
    {
        printf("ѧ�Ų��Ϸ���������:\n");
        s_gets(id, ID_LENGTH);
        if (id[0] == 'q') return false;
    }
    if (removeByID(ptree, id))
    {
        printf("ɾ���ɹ�!\n");
        return true;
    }
    else
    {
        printf("û���ҵ���ѧ��\n");
        return false;
    }
}

bool removeStudentByName(Tree * ptree)
{
    if (ptree->size == 0)
    {
        printf("��ǰ��û��ѧ��\n");
        return false;
    }
    printf("������ѧ��������(����q�˳�):\n");
    char name[NAME_LENGTH];
    s_gets(name, NAME_LENGTH);
    if (name[0]=='q') return false;
    while (!nameIsValid(name))
    {
        printf("�������Ϸ���������:\n");
        s_gets(name, NAME_LENGTH);
        if (name[0] == 'q') return false;
    }
    if (removeByName(ptree, name))
    {
        printf("ɾ���ɹ�!\n");
        return true;
    }
    else
    {
        printf("û���ҵ���ѧ��\n");
        return false;
    }
}
