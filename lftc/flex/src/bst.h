#ifndef bst_h
#define bst_h

#include <stdio.h>

struct BinaryTreeNode{
    char *key;
    int position;
    struct BinaryTreeNode *left, *right;
};

struct BinaryTreeNode *newNodeCreate(char *value, int position);
struct BinaryTreeNode *insertNode(struct BinaryTreeNode* node, char *value, int *next_pos);
struct BinaryTreeNode *searchNode(struct BinaryTreeNode* node, char *value);
void inorder(struct BinaryTreeNode* root, FILE *fout);  // Added FILE* parameter
void freeTree(struct BinaryTreeNode* root);  // Added cleanup function


#endif