#ifndef CLASSMANAGER_HPP_INCLUDED
#define CLASSMANAGER_HPP_INCLUDED

#define NAME_LENGTH     30

#define ID_LENGTH       20


#define LEFT                        0

#define RIGHT                       1

#define ID_TREE                     0

#define NAME_TREE                   1

#define INVALID_PARAM_TREE         -1

#define INVALID_PARAM_NAMEORID     -2

#define VALID_PARAM                 1

#define FOUND                       1

#define NOT_FOUND                   0

typedef struct student
{
    char name[NAME_LENGTH];
    char id[ID_LENGTH];
    double mathScore;
    double chineseScore;
    double englishScore;
} Student;

typedef struct nodeInfo
{
    int height;
    struct node * parent;
    struct node * left;
    struct node * right;
} NodeInfo;

typedef struct node
{
    Student student;
    NodeInfo IDNodeInfo;
    NodeInfo nameNodeInfo;
} Node;

typedef struct tree
{
    Node * IDRoot;
    Node * nameRoot;
    int size;
} Tree;

typedef Node** StudentList;

bool removeByID(Tree * ptree, char * id);

bool removeByName(Tree * ptree, char * name);

Node * searchByID(Tree * ptree, char * id);

Node * searchByName(Tree * ptree, char * name);

bool isExisted(const Tree * ptree, const char * id);

bool insert(Tree * ptree, Student * student);

void inOrderTraverseInID(Node * root, Tree * ptree, Node ** list,
                         void (*pfun)(Tree * ptree,
                                      Student * student,
                                      Node ** list));

bool initTree(Tree * ptree);

int getRank(Tree * ptree, Student * student, Node ** list);



Node ** makeStudentListSortedByScore(Tree * ptree);

void destroyTree(Tree * ptree);

Node * insertReturnNodePointer(Tree * ptree, Student * student);

#endif // CLASSMANAGER_HPP_INCLUDED
