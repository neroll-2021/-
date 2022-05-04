#include "ClassManager.hpp"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

double getTotalScore(Student  * student);

static void quickSort(Node ** list, int left, int right,
                      int (*comparator)(Node * a, Node * b));



static int heightOfIDNode(Node * node)
{
    return node == NULL ? 0 : node->IDNodeInfo.height;
}

static int heightOfNameNode(Node * node)
{
    return node == NULL ? 0 : node->nameNodeInfo.height;
}

static void update(Node * node)
{
    if (node == NULL) return;
    node->IDNodeInfo.height = (heightOfIDNode(node->IDNodeInfo.left)
                               > heightOfIDNode(node->IDNodeInfo.right)) ?
                               heightOfIDNode(node->IDNodeInfo.left) + 1 :
                               heightOfIDNode(node->IDNodeInfo.right) + 1;
    node->nameNodeInfo.height = (heightOfIDNode(node->nameNodeInfo.left)
                               > heightOfIDNode(node->nameNodeInfo.right)) ?
                               heightOfIDNode(node->nameNodeInfo.left) + 1 :
                               heightOfIDNode(node->nameNodeInfo.right) + 1;
}

static void leftRotateInID(Tree * ptree, Node * &root)
{
    if (root == NULL || root->IDNodeInfo.right == NULL)
        return;
    Node * rightChild = root->IDNodeInfo.right;
    root->IDNodeInfo.right = rightChild->IDNodeInfo.left;
    if (rightChild->IDNodeInfo.left != NULL)
        rightChild->IDNodeInfo.left->IDNodeInfo.parent = root;
    root->IDNodeInfo.parent = rightChild->IDNodeInfo.parent;
    if (root->IDNodeInfo.parent == NULL)
        ptree->IDRoot = rightChild;
    else if (root == root->IDNodeInfo.parent->IDNodeInfo.left)
        root->IDNodeInfo.parent->IDNodeInfo.left = rightChild;
    else
        root->IDNodeInfo.parent->IDNodeInfo.right = rightChild;
    rightChild->IDNodeInfo.left = root;
    root->IDNodeInfo.parent = rightChild;
    update(root);
    update(rightChild);
    root = rightChild;
}

static void leftRotateInName(Tree * ptree, Node * &root)
{
    if (root == NULL || root->nameNodeInfo.right == NULL)
        return;
    Node * rightChild = root->nameNodeInfo.right;
    root->nameNodeInfo.right = rightChild->nameNodeInfo.left;
    if (rightChild->nameNodeInfo.left != NULL)
        rightChild->nameNodeInfo.left->nameNodeInfo.parent = root;
    rightChild->nameNodeInfo.parent = root->nameNodeInfo.parent;
    if (root->nameNodeInfo.parent == NULL)
        ptree->nameRoot = rightChild;
    else if (root == root->nameNodeInfo.parent->nameNodeInfo.left)
        root->nameNodeInfo.parent->nameNodeInfo.left = rightChild;
    else
        root->nameNodeInfo.parent->nameNodeInfo.right = rightChild;
    rightChild->nameNodeInfo.left = root;
    root->nameNodeInfo.parent = rightChild;
    update(root);
    update(rightChild);
    root = rightChild;
}

static void rightRotateInID(Tree * ptree, Node * &root)
{
    if (root == NULL || root->IDNodeInfo.left == NULL)
        return;
    Node * leftChild = root->IDNodeInfo.left;
    root->IDNodeInfo.left = leftChild->IDNodeInfo.right;
    if (leftChild->IDNodeInfo.right != NULL)
        leftChild->IDNodeInfo.right->IDNodeInfo.parent = root;
    leftChild->IDNodeInfo.parent = root->IDNodeInfo.parent;
    if (root->IDNodeInfo.parent == NULL)
        ptree->IDRoot = leftChild;
    else if (root == root->IDNodeInfo.parent->IDNodeInfo.left)
        root->IDNodeInfo.parent->IDNodeInfo.left = leftChild;
    else
        root->IDNodeInfo.parent->IDNodeInfo.right = leftChild;
    leftChild->IDNodeInfo.right = root;
    root->IDNodeInfo.parent = leftChild;
    update(root);
    update(leftChild);
    root = leftChild;
}

static void rightRotateInName(Tree * ptree, Node * &root)
{
    if (root == NULL || root->nameNodeInfo.left == NULL)
        return;
    Node * leftChild = root->nameNodeInfo.left;
    root->nameNodeInfo.left = leftChild->nameNodeInfo.right;
    if (leftChild->nameNodeInfo.right != NULL)
        leftChild->nameNodeInfo.right->nameNodeInfo.parent = root;
    leftChild->nameNodeInfo.parent = root->nameNodeInfo.parent;
    if (root->nameNodeInfo.parent == NULL)
        ptree->nameRoot = leftChild;
    else if (root == root->nameNodeInfo.parent->nameNodeInfo.left)
        root->nameNodeInfo.parent->nameNodeInfo.left = leftChild;
    else
        root->nameNodeInfo.parent->nameNodeInfo.right = leftChild;
    leftChild->nameNodeInfo.right = root;
    root->nameNodeInfo.parent = leftChild;
    update(root);
    update(leftChild);
    root = leftChild;
}

static void leftRightRotateInID(Tree * ptree, Node * &root)
{
    if (root == NULL) return;
    leftRotateInID(ptree, root->IDNodeInfo.left);
    rightRotateInID(ptree, root);
}

static void leftRightRotateInName(Tree * ptree, Node * &root)
{
    if (root == NULL) return;
    leftRotateInName(ptree, root->nameNodeInfo.left);
    rightRotateInName(ptree, root);
}

static void rightLeftRotateInID(Tree * ptree, Node * &root)
{
    if (root == NULL) return;
    rightRotateInID(ptree, root->IDNodeInfo.right);
    leftRotateInID(ptree, root);
}

static void rightLeftRotateInName(Tree * ptree, Node * &root)
{
    if (root == NULL) return;
    rightRotateInName(ptree, root->nameNodeInfo.right);
    leftRotateInName(ptree, root);
}

/**
 * 检查传递给searchPos函数的参数是否合法
 * 在以下情况时判断参数不合法:
 * 1. 当tree不为0且不为1时
 * 2. 当tree为0且nameOrID中有非数字字符时，
 *    或当tree为1且nameOrID中有字母、数字、符号时
 * @param nameOrID 学生的姓名或学号
 * @param tree 指示要搜索ID树还是name树，0为ID树，1为name树
 * @return 若参数合法，则返回1，
 *        否则,若出现情况1，返回-1，若出现情况2，返回-2
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
static int validParamInSearchPos(const char * nameOrID,
                                const int tree)
{
    if (tree != ID_TREE && tree != NAME_TREE)
        return INVALID_PARAM_TREE;
    const char * s = nameOrID;
    if (tree == NAME_TREE)
    {
        while (*s)
        {
            if (ispunct(*s) || isalnum(*s))
                return INVALID_PARAM_NAMEORID;
            s++;
        }
        return VALID_PARAM;
    }
    s = nameOrID;
    while (*s)
    {
        if (!isdigit(*s))
            return INVALID_PARAM_NAMEORID;
        s++;
    }
    return VALID_PARAM;
}

/**
 * 在树中根据姓名或学号查找一个学生
 * 根据参数指定的条件在ID树或name树中搜索
 * 使用前应先判断root是否为空
 * @param root 要查找的树的根结点，可以是ID树，也可以是name树
 * @param find 如果在树中找到了学生，返回true，否则返回false
 * @param insertPos 若没找到学生，则返回其应插入的位置，
 *        若它应是其父节点的左孩子，则返回0，否则返回1
 * @param nameOrID 学生姓名或学号，该函数将依照该字符串搜索学生
 * @param parentNode 若没找到学生，返回指向该学生应插入的位置的
 *        父结点的指针
 * @param tree 指示要搜索ID树还是name树，0为ID树，1为name树
 * @return 若操作错误则返回错误码，如参数不合法等，否则返回1
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
static int searchPos(Node * root, bool * find,
                      int * insertPos, const char * nameOrID,
                      Node * &parentNode,const int tree)
{
    int paramValidCode;
    if ((paramValidCode = validParamInSearchPos(nameOrID, tree))
         != VALID_PARAM)
    {
        // here can add an error handler to handle different error
        return paramValidCode;
    }
    Node * parent = NULL;
    Node * cur = root;
    while (cur)
    {
        parent = cur;
        if (tree == ID_TREE)
        {
            if (strcmp(nameOrID, cur->student.id) < 0)
                cur = cur->IDNodeInfo.left;
            else if (strcmp(nameOrID, cur->student.id) > 0)
                cur = cur->IDNodeInfo.right;
            else
            {
                *find = true;
                return FOUND;
            }
        }
        else
        {
            if (strcmp(nameOrID, cur->student.name) < 0)
                cur = cur->nameNodeInfo.left;
            else if (strcmp(nameOrID, cur->student.name) > 0)
                cur = cur->nameNodeInfo.right;
            else
            {
                *find = true;
                return FOUND;
            }
        }
    }
    *find = false;
    if (tree == ID_TREE)
    {
        if (strcmp(nameOrID, parent->student.id) < 0)
            *insertPos = LEFT;
        else
            *insertPos = RIGHT;
    }
    else
    {
        if (strcmp(nameOrID, parent->student.name) < 0)
            *insertPos = LEFT;
        else
            *insertPos = RIGHT;
    }
    parentNode = parent;
    return NOT_FOUND;
}

static Node * makeNode(Student * student)
{
    Node * node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        fprintf(stderr, "cannot malloc memory in %d in ClassManager.cpp",
                __LINE__);
        exit(EXIT_FAILURE);
    }
    node->student = *student;
    NodeInfo info;
    info.height = 0;
    info.left = info.parent = info.right = NULL;
    node->IDNodeInfo = node->nameNodeInfo = info;
    return node;
}

/**
 * 修复插入结点对ID树的平衡性的影响
 * @param ptree 树的指针
 * @param node 指向新加入的结点的指针
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
static void fixAfterInsertionInID(Tree * ptree, Node * node)
{
    while (node->IDNodeInfo.parent)
    {
        if (heightOfIDNode(node->IDNodeInfo.left)
            - heightOfIDNode(node->IDNodeInfo.right) > 1)
        {
            if (heightOfIDNode(node->IDNodeInfo.left->IDNodeInfo.left)
                > heightOfIDNode(node->IDNodeInfo.left->IDNodeInfo.right))
                rightRotateInID(ptree, node);
            else
                leftRightRotateInID(ptree, node);
        }
        else if (heightOfIDNode(node->IDNodeInfo.right)
                 - heightOfIDNode(node->IDNodeInfo.left) > 1)
        {
            if (heightOfIDNode(node->IDNodeInfo.right->IDNodeInfo.right)
                > heightOfIDNode(node->IDNodeInfo.right->IDNodeInfo.left))
                leftRotateInID(ptree, node);
            else
                rightLeftRotateInID(ptree, node);
        }
        node = node->IDNodeInfo.parent;
    }
}

/**
 * 修复插入结点对name树的平衡性的影响
 * @param ptree 树的指针
 * @param node 指向新加入的结点的指针
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
static void fixAfterInsertionInName(Tree * ptree, Node * node)
{
    while (node->nameNodeInfo.parent)
    {
        if (heightOfNameNode(node->nameNodeInfo.left)
            - heightOfNameNode(node->nameNodeInfo.right) > 1)
        {
            if (node->nameNodeInfo.left->nameNodeInfo.left
                > node->nameNodeInfo.left->nameNodeInfo.right)
                rightRotateInName(ptree, node);
            else
                leftRightRotateInName(ptree, node);
        }
        else if (heightOfNameNode(node->nameNodeInfo.right)
            - heightOfNameNode(node->nameNodeInfo.left) > 1)
        {
            if (heightOfNameNode(node->nameNodeInfo.right->nameNodeInfo.left)
                > heightOfNameNode(node->nameNodeInfo.right->nameNodeInfo.right))
                rightLeftRotateInName(ptree, node);
            else
                leftRotateInName(ptree, node);
        }
        node = node->nameNodeInfo.parent;
    }
}


/**
 * 在树中添加一个学生，若学生已经存在，则不做任何操作
 * @param ptree 指向树的指针
 * @param student 要添加的学生的信息
 * @return 若添加成功，则返回true，若添加失败，
 *        如学生已经存在等，返回false
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
bool insert(Tree * ptree, Student * student)
{
    bool findInTree = false;
    int insertPos = -1;
    Node * parentNode = NULL;
    if (ptree->IDRoot == NULL)
    {
        Node * newNode = makeNode(student);
        ptree->IDRoot = newNode;
        ptree->nameRoot = newNode;
        ptree->size++;
        return true;
    }
    searchPos(ptree->IDRoot, &findInTree, &insertPos, student->id,
              parentNode, ID_TREE);
    if (findInTree)
        return false;
    Node * newNode = makeNode(student);
    if (insertPos == LEFT)
        parentNode->IDNodeInfo.left = newNode;
    else
        parentNode->IDNodeInfo.right = newNode;
    newNode->IDNodeInfo.parent = parentNode;
    fixAfterInsertionInID(ptree, newNode);
    searchPos(ptree->nameRoot, &findInTree, &insertPos, student->name,
              parentNode, NAME_TREE);
    if (findInTree) return false;
    if (insertPos == LEFT)
        parentNode->nameNodeInfo.left = newNode;
    else
        parentNode->nameNodeInfo.right = newNode;
    newNode->nameNodeInfo.parent = parentNode;
    fixAfterInsertionInName(ptree, newNode);
    ptree->size++;
    return true;
}

/**
 * 在树中添加一个学生，若学生已经存在，则不做任何操作
 * @param ptree 指向树的指针
 * @param student 要添加的学生的信息
 * @return 若添加成功，则返回新添加的结点，若添加失败，
 *        如学生已经存在等，返回NULL
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
Node * insertReturnNodePointer(Tree * ptree, Student * student)
{
    bool findInTree = false;
    int insertPos = -1;
    Node * parentNode = NULL;
    if (ptree->IDRoot == NULL)
    {
        Node * newNode = makeNode(student);
        ptree->IDRoot = newNode;
        ptree->nameRoot = newNode;
        ptree->size++;
        return newNode;
    }
    searchPos(ptree->IDRoot, &findInTree, &insertPos, student->id,
              parentNode, ID_TREE);
    if (findInTree)
        return NULL;
    Node * newNode = makeNode(student);
    if (insertPos == LEFT)
        parentNode->IDNodeInfo.left = newNode;
    else
        parentNode->IDNodeInfo.right = newNode;
    newNode->IDNodeInfo.parent = parentNode;
    fixAfterInsertionInID(ptree, newNode);
    searchPos(ptree->nameRoot, &findInTree, &insertPos, student->name,
              parentNode, NAME_TREE);
    if (findInTree) return NULL;
    if (insertPos == LEFT)
        parentNode->nameNodeInfo.left = newNode;
    else
        parentNode->nameNodeInfo.right = newNode;
    newNode->nameNodeInfo.parent = parentNode;
    fixAfterInsertionInName(ptree, newNode);
    ptree->size++;
    return newNode;
}

bool TreeIsEmpty(Tree * ptree)
{
    return ptree->size == 0;
}

/**
 * 根据学号判断一个学生是否已经存在
 * @param ptree 树的指针
 * @param id 学生的学号
 * @return 若学生已存在，则返回true,否则返回false
 * @version 0.1.0
 * @date 2022-05-01
 * @author Neroll
 */
bool isExisted(const Tree * ptree, const char * id)
{
    const Node * cur = ptree->IDRoot;
    while (cur)
    {
        if (strcmp(id, cur->student.id) < 0)
            cur = cur->IDNodeInfo.left;
        else if (strcmp(id, cur->student.id) > 0)
            cur = cur->IDNodeInfo.right;
        else
            return true;
    }
    return false;
}

/**
 * 在ID树中找到一个结点的后继
 * @param node 要找前驱的结点
 * @return 指定的结点的前驱
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
static Node * getSuccessorInID(Node * node)
{
    if (node == NULL || node->IDNodeInfo.right == NULL)
        return NULL;
    node = node->IDNodeInfo.right;
    while (node->IDNodeInfo.left)
        node = node->IDNodeInfo.left;
    return node;
}

/**
 * 在name树中找到一个结点的后继
 * @param node 要找前驱的结点
 * @return 指定的结点的前驱
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
static Node * getSuccessorInName(Node * node)
{
    if (node == NULL || node->nameNodeInfo.right == NULL)
        return NULL;
    node = node->nameNodeInfo.right;
    while (node->nameNodeInfo.left)
        node = node->nameNodeInfo.left;
    return node;
}

/**
 * 在ID树中用substitute子树代替root树
 * @param ptree 树的指针
 * @param root 要被替代的结点
 * @param substitute 代替root的结点
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
static void transplantInID(Tree * ptree, Node * root, Node * substitute)
{
    if (root->IDNodeInfo.parent == NULL)
        ptree->IDRoot = substitute;
    else if (root == root->IDNodeInfo.parent->IDNodeInfo.left)
        root->IDNodeInfo.parent->IDNodeInfo.left = substitute;
    else
        root->IDNodeInfo.parent->IDNodeInfo.right = substitute;
    if (substitute != NULL)
        substitute->IDNodeInfo.parent = root->IDNodeInfo.parent;
}

/**
 * 在name树中用substitute子树代替root树
 * @param ptree 树的指针
 * @param root 要被替代的结点
 * @param substitute 代替root的结点
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
static void transplantInName(Tree * ptree, Node * root, Node * substitute)
{
    if (root->nameNodeInfo.parent == NULL)
        ptree->nameRoot = substitute;
    else if (root == root->nameNodeInfo.parent->nameNodeInfo.left)
        root->nameNodeInfo.parent->nameNodeInfo.left = substitute;
    else
        root->nameNodeInfo.parent->nameNodeInfo.right = substitute;
    if (substitute != NULL)
        substitute->nameNodeInfo.parent = root->nameNodeInfo.parent;
}

/**
 * 在ID树中删除一个结点，不释放内存，只更改结点间的关系
 * @param ptree 树的指针
 * @param node 要删除的结点
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
static void removeNodeInID(Tree * ptree, Node * node)
{
    if (node->IDNodeInfo.left == NULL)
        transplantInID(ptree, node, node->IDNodeInfo.right);
    else if (node->IDNodeInfo.right == NULL)
        transplantInID(ptree, node, node->IDNodeInfo.left);
    else
    {
        Node * successor = getSuccessorInID(node);
        if (successor->IDNodeInfo.parent != node)
        {
            transplantInID(ptree, successor, successor->IDNodeInfo.right);
            successor->IDNodeInfo.right = node->IDNodeInfo.right;
            successor->IDNodeInfo.right->IDNodeInfo.parent = successor;
        }
        transplantInID(ptree, node, successor);
        successor->IDNodeInfo.left = node->IDNodeInfo.left;
        successor->IDNodeInfo.left->IDNodeInfo.parent = successor;
    }
}

/**
 * 在name树中删除一个结点，不释放内存，只更改结点间的关系
 * @param ptree 树的指针
 * @param node 要删除的结点
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
static void removeNodeInName(Tree * ptree, Node * node)
{
    if (node->nameNodeInfo.left == NULL)
        transplantInName(ptree, node, node->nameNodeInfo.right);
    else if (node->nameNodeInfo.right == NULL)
        transplantInName(ptree, node, node->nameNodeInfo.left);
    else
    {
        Node * successor = getSuccessorInName(node);
        if (successor->nameNodeInfo.parent != node)
        {
            transplantInName(ptree, successor, successor->nameNodeInfo.right);
            successor->nameNodeInfo.right = node->nameNodeInfo.right;
            successor->nameNodeInfo.right->nameNodeInfo.parent = successor;
        }
        transplantInName(ptree, node, successor);
        successor->nameNodeInfo.left = node->nameNodeInfo.left;
        successor->nameNodeInfo.left->nameNodeInfo.parent = successor;
    }
}

/**
 * 在树中根据学生学号删除学生
 * @param root 树的指针
 * @param id 学生的学号
 * @return 若删除成功，则返回true，否则返回false
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
bool removeByID(Tree * ptree, char * id)
{
    if (ptree->IDRoot == NULL) return false;
    Node * parent = NULL;
    Node * cur = ptree->IDRoot;
    while (cur && strcmp(cur->student.id, id) != 0)
    {
        parent = cur;
        if (strcmp(id, cur->student.id) < 0)
            cur = cur->IDNodeInfo.left;
        else if (strcmp(id, cur->student.id) > 0)
            cur = cur->IDNodeInfo.right;
    }
    if (cur == NULL) return false;
    if (parent == NULL)
    {
        free(cur);
        ptree->IDRoot = NULL;
        ptree->nameRoot = NULL;
        ptree->size--;
        return true;
    }
    removeNodeInID(ptree, cur);
    fixAfterInsertionInID(ptree, parent);
    removeNodeInName(ptree, cur);
    fixAfterInsertionInName(ptree, parent);
    ptree->size--;
    return true;
}

/**
 * 在树中根据学生姓名删除学生
 * @param root 树的指针
 * @param id 学生的学号
 * @return 若删除成功，则返回true，否则返回false
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
bool removeByName(Tree * ptree, char * name)
{
    if (ptree->nameRoot == NULL) return false;
    Node * parent = NULL;
    Node * cur = ptree->nameRoot;
    while (cur)
    {
        parent = cur;
        if (strcmp(name, cur->student.name) < 0)
            cur = cur->nameNodeInfo.left;
        else if (strcmp(name, cur->student.name) > 0)
            cur = cur->nameNodeInfo.right;
        else
            break;
    }
    if (parent == NULL)
    {
        free(cur);
        ptree->IDRoot = ptree->nameRoot = NULL;
        ptree->size--;
        return true;
    }
    if (cur == NULL) return false;
    removeNodeInID(ptree, cur);
    fixAfterInsertionInID(ptree, parent);
    removeNodeInName(ptree, cur);
    fixAfterInsertionInName(ptree,parent);
    ptree->size--;
    return true;
}

/**
 * 根据学生学号在树中查找学生
 * @param ptree 树的指针
 * @param id 学生的学号
 * @return 若找到学生，则返回指向该学生结点的指针，否则返回NULL
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
Node * searchByID(Tree * ptree, char * id)
{
    Node * cur = ptree->IDRoot;
    while (cur)
    {
        if (strcmp(id, cur->student.id) < 0)
            cur = cur->IDNodeInfo.left;
        else if (strcmp(id, cur->student.id) > 0)
            cur = cur->IDNodeInfo.right;
        else
            break;
    }
    return cur;
}

/**
 * 根据学生姓名在树中查找学生
 * @param ptree 树的指针
 * @param name 学生的姓名
 * @return 若找到学生，则返回指向该学生结点的指针，否则返回NULL
 * @version 0.1.0
 * @date 2022-05-02
 * @author Neroll
 */
Node * searchByName(Tree * ptree, char * name)
{
    Node * cur = ptree->nameRoot;
    while (cur)
    {
        if (strcmp(name, cur->student.name) < 0)
            cur = cur->nameNodeInfo.left;
        else if (strcmp(name, cur->student.name) > 0)
            cur = cur->nameNodeInfo.right;
        else
            break;
    }
    return cur;
}

/**
 * 检查学生的学号是否合法，若出现非数字字符则为不合法
 * @param id 学生的学号
 * @return 若合法，则返回true，否则返回false
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
bool IDIsValid(const char * id)
{
    while (*id)
    {
        if (!isdigit(*id))
            return false;
        id++;
    }
    return true;
}

/**
 * 检查学生的姓名是否合法，若出现数字、字母、符号则为不合法
 * @param name 学生的姓名
 * @return 若合法，则返回true，否则返回false
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
bool nameIsValid(const char * name)
{
    while (*name)
    {
        if (isalnum(*name) || ispunct(*name))
            return false;
        name++;
    }
    return true;
}

bool initTree(Tree * ptree)
{
    ptree->IDRoot = NULL;
    ptree->nameRoot = NULL;
    ptree->size = 0;
    return true;
}

void inOrderTraverseInID(Node * root, Tree * ptree, Node ** list,
                         void (*pfun)(Tree * ptree,
                                      Student * student,
                                      Node ** list))
{
    if (root)
    {
        inOrderTraverseInID(root->IDNodeInfo.left,
                            ptree, list,
                            pfun);
        pfun(ptree, &(root->student), list);
        inOrderTraverseInID(root->IDNodeInfo.right,
                            ptree, list,
                            pfun);
    }
}

/**
 * 将指向树的结点的指针填入数组中
 * @param root 指向树的根结点的指针
 * @param list 指向树的结点的指针的数组
 * @param index 当前指针在数组中的下标
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
static void makeList(Node * root, Node ** list, int &index)
{
    if (root)
    {
        list[index++] = root;
        makeList(root->IDNodeInfo.left, list, index);
        makeList(root->IDNodeInfo.right, list, index);
    }
}

/**
 * 根据学生的总分和学号对学生进行比较
 * @param a 第一个学生
 * @param b 第二个学生
 * @return a 与 b 比较的结果
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
static int compareStudentByScore(Node * a, Node * b)
{
    if (fabs(getTotalScore(&(a->student))
        - getTotalScore(&(b->student))) > 0.1)
        return (int)(getTotalScore(&(b->student))
                - getTotalScore(&(a->student)));
    return strcmp(a->student.id, b->student.id);
}

/**
 * 分配一定的空间作为一个数组，存放指向树结点的指针
 * @param ptree 指向树的指针
 * @return 元素为指向树结点的指针的数组
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
Node ** makeStudentListSortedByScore(Tree * ptree)
{
    Node ** list = (Node **)malloc(sizeof(Node *) * (ptree->size));
    if (list == NULL)
    {
        fprintf(stderr, "cannot malloc memory in %d in ClassManager.cpp",
                __LINE__);
        exit(EXIT_FAILURE);
    }
    int index = 0;
    makeList(ptree->IDRoot, list, index);
    quickSort(list, 0, ptree->size - 1, compareStudentByScore);
    return list;
}

static void swapNodePointer(Node ** a, Node ** b)
{
    Node * temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 快速排序的排序过程，选定list数组的[left, right]部分中的list[right]
 * 作为基准，比该基准小的排在基准的左边,比基准大的排在基准的右边
 * @param list 元素为指向树结点的指针的数组
 * @param left 数组的起始位置
 * @param right 数组的结束位置
 * @param comparator 比较两个指针指向的结点的大小
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
static int subSort(Node ** list, int left, int right,
                    int (*comparator)(Node * a, Node * b))
{
    int base = right;
    int i;
    int j;
    for (i = left, j = left; j < right; j++)
    {
        if (comparator(list[j], list[base]) < 0)
        {
            swapNodePointer(&list[i], &list[j]);
            i++;
        }
    }
    swapNodePointer(&list[i], &list[base]);
    return i;
}

/**
 * 快速排序，将list数组分成多个小数组，对每一个小数组进行排序
 * @param list 元素为指向树结点的指针的数组
 * @param left list数组的起始位置
 * @param right list数组的结束位置
 * @version 0.1.0
 * @date 2022-05-03
 * @author Neroll
 */
static void quickSort(Node ** list, int left, int right,
                      int (*comparator)(Node * a, Node * b))
{
    if (left < right)
    {
        int i = subSort(list, left, right, comparator);
        quickSort(list, left, i - 1, comparator);
        quickSort(list, i + 1, right, comparator);
    }
}

int getRank(Tree * ptree, Student * student, Node ** list)
{
    int left = 0;
    int right = ptree->size - 1;
    int mid;
    while (left <= right)
    {
        mid = (left + right) >> 1;
        if (getTotalScore(&(list[mid]->student)) < getTotalScore(student))
        {
            right = mid - 1;

        }
        else if (getTotalScore(&(list[mid]->student)) > getTotalScore(student))
        {
            left = mid + 1;
        }
        else if (fabs(getTotalScore(&(list[mid]->student))
                - getTotalScore(student)) < 1e-5)
        {
            break;
        }
    }
    int rank = mid + 1;
    bool move = false;
    while (mid > 0
           && fabs(getTotalScore(&(list[mid - 1]->student))
           - getTotalScore(student)) < 1e-6)
    {
        mid--;
        rank--;
        move = true;
    }
    return rank;
}

static void destroyAllNodes(Node * root)
{
    if (root)
    {
        destroyAllNodes(root->IDNodeInfo.left);
        destroyAllNodes(root->IDNodeInfo.right);
        free(root);
    }
}

void destroyTree(Tree * ptree)
{
    destroyAllNodes(ptree->IDRoot);
    ptree->IDRoot = ptree->nameRoot = NULL;
    ptree->size = 0;
}

bool initList(Node *** list, int size)
{
    *list = (Node **)malloc(sizeof(Node *)*size);
    if (*list == NULL)
    {
        fprintf(stderr, "cannot malloc memory in %d in ClassManager.cpp\n",
                __LINE__);
        exit(EXIT_FAILURE);
    }
    return true;
}
