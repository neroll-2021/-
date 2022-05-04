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
 * ��鴫�ݸ�searchPos�����Ĳ����Ƿ�Ϸ�
 * ���������ʱ�жϲ������Ϸ�:
 * 1. ��tree��Ϊ0�Ҳ�Ϊ1ʱ
 * 2. ��treeΪ0��nameOrID���з������ַ�ʱ��
 *    ��treeΪ1��nameOrID������ĸ�����֡�����ʱ
 * @param nameOrID ѧ����������ѧ��
 * @param tree ָʾҪ����ID������name����0ΪID����1Ϊname��
 * @return �������Ϸ����򷵻�1��
 *        ����,���������1������-1�����������2������-2
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
 * �����и���������ѧ�Ų���һ��ѧ��
 * ���ݲ���ָ����������ID����name��������
 * ʹ��ǰӦ���ж�root�Ƿ�Ϊ��
 * @param root Ҫ���ҵ����ĸ���㣬������ID����Ҳ������name��
 * @param find ����������ҵ���ѧ��������true�����򷵻�false
 * @param insertPos ��û�ҵ�ѧ�����򷵻���Ӧ�����λ�ã�
 *        ����Ӧ���丸�ڵ�����ӣ��򷵻�0�����򷵻�1
 * @param nameOrID ѧ��������ѧ�ţ��ú��������ո��ַ�������ѧ��
 * @param parentNode ��û�ҵ�ѧ��������ָ���ѧ��Ӧ�����λ�õ�
 *        ������ָ��
 * @param tree ָʾҪ����ID������name����0ΪID����1Ϊname��
 * @return �����������򷵻ش����룬��������Ϸ��ȣ����򷵻�1
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
 * �޸��������ID����ƽ���Ե�Ӱ��
 * @param ptree ����ָ��
 * @param node ָ���¼���Ľ���ָ��
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
 * �޸��������name����ƽ���Ե�Ӱ��
 * @param ptree ����ָ��
 * @param node ָ���¼���Ľ���ָ��
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
 * ���������һ��ѧ������ѧ���Ѿ����ڣ������κβ���
 * @param ptree ָ������ָ��
 * @param student Ҫ��ӵ�ѧ������Ϣ
 * @return ����ӳɹ����򷵻�true�������ʧ�ܣ�
 *        ��ѧ���Ѿ����ڵȣ�����false
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
 * ���������һ��ѧ������ѧ���Ѿ����ڣ������κβ���
 * @param ptree ָ������ָ��
 * @param student Ҫ��ӵ�ѧ������Ϣ
 * @return ����ӳɹ����򷵻�����ӵĽ�㣬�����ʧ�ܣ�
 *        ��ѧ���Ѿ����ڵȣ�����NULL
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
 * ����ѧ���ж�һ��ѧ���Ƿ��Ѿ�����
 * @param ptree ����ָ��
 * @param id ѧ����ѧ��
 * @return ��ѧ���Ѵ��ڣ��򷵻�true,���򷵻�false
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
 * ��ID�����ҵ�һ�����ĺ��
 * @param node Ҫ��ǰ���Ľ��
 * @return ָ���Ľ���ǰ��
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
 * ��name�����ҵ�һ�����ĺ��
 * @param node Ҫ��ǰ���Ľ��
 * @return ָ���Ľ���ǰ��
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
 * ��ID������substitute��������root��
 * @param ptree ����ָ��
 * @param root Ҫ������Ľ��
 * @param substitute ����root�Ľ��
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
 * ��name������substitute��������root��
 * @param ptree ����ָ��
 * @param root Ҫ������Ľ��
 * @param substitute ����root�Ľ��
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
 * ��ID����ɾ��һ����㣬���ͷ��ڴ棬ֻ���Ľ���Ĺ�ϵ
 * @param ptree ����ָ��
 * @param node Ҫɾ���Ľ��
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
 * ��name����ɾ��һ����㣬���ͷ��ڴ棬ֻ���Ľ���Ĺ�ϵ
 * @param ptree ����ָ��
 * @param node Ҫɾ���Ľ��
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
 * �����и���ѧ��ѧ��ɾ��ѧ��
 * @param root ����ָ��
 * @param id ѧ����ѧ��
 * @return ��ɾ���ɹ����򷵻�true�����򷵻�false
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
 * �����и���ѧ������ɾ��ѧ��
 * @param root ����ָ��
 * @param id ѧ����ѧ��
 * @return ��ɾ���ɹ����򷵻�true�����򷵻�false
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
 * ����ѧ��ѧ�������в���ѧ��
 * @param ptree ����ָ��
 * @param id ѧ����ѧ��
 * @return ���ҵ�ѧ�����򷵻�ָ���ѧ������ָ�룬���򷵻�NULL
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
 * ����ѧ�����������в���ѧ��
 * @param ptree ����ָ��
 * @param name ѧ��������
 * @return ���ҵ�ѧ�����򷵻�ָ���ѧ������ָ�룬���򷵻�NULL
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
 * ���ѧ����ѧ���Ƿ�Ϸ��������ַ������ַ���Ϊ���Ϸ�
 * @param id ѧ����ѧ��
 * @return ���Ϸ����򷵻�true�����򷵻�false
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
 * ���ѧ���������Ƿ�Ϸ������������֡���ĸ��������Ϊ���Ϸ�
 * @param name ѧ��������
 * @return ���Ϸ����򷵻�true�����򷵻�false
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
 * ��ָ�����Ľ���ָ������������
 * @param root ָ�����ĸ�����ָ��
 * @param list ָ�����Ľ���ָ�������
 * @param index ��ǰָ���������е��±�
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
 * ����ѧ�����ֺܷ�ѧ�Ŷ�ѧ�����бȽ�
 * @param a ��һ��ѧ��
 * @param b �ڶ���ѧ��
 * @return a �� b �ȽϵĽ��
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
 * ����һ���Ŀռ���Ϊһ�����飬���ָ��������ָ��
 * @param ptree ָ������ָ��
 * @return Ԫ��Ϊָ��������ָ�������
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
 * ���������������̣�ѡ��list�����[left, right]�����е�list[right]
 * ��Ϊ��׼���ȸû�׼С�����ڻ�׼�����,�Ȼ�׼������ڻ�׼���ұ�
 * @param list Ԫ��Ϊָ��������ָ�������
 * @param left �������ʼλ��
 * @param right ����Ľ���λ��
 * @param comparator �Ƚ�����ָ��ָ��Ľ��Ĵ�С
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
 * �������򣬽�list����ֳɶ��С���飬��ÿһ��С�����������
 * @param list Ԫ��Ϊָ��������ָ�������
 * @param left list�������ʼλ��
 * @param right list����Ľ���λ��
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
