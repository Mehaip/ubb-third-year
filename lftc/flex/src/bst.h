#ifndef bst_h
#define bst_h

struct BinaryTreeNode{
    int key;
    struct BinaryTreeNode *left, *right;
};

struct BinaryTreeNode *newNodeCreate(int value);
struct BinaryTreeNode *insertNode(struct BinaryTreeNode* node, int value);
void inorder(struct BinaryTreeNode* root);



#endif
