#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

struct BinaryTreeNode *newNodeCreate(int value){
    struct BinaryTreeNode *temp = (struct BinaryTreeNode*) malloc (sizeof (struct BinaryTreeNode));
    temp->key = value;
    temp->left = temp->right = NULL;
    return temp;
}

struct BinaryTreeNode *insertNode(struct BinaryTreeNode* node, int value){
    if(node == NULL){
        return newNodeCreate(value);
    }
    else {
    if(value < node->key)
        insertNode(node->left, value);
    
    else
        insertNode(node->right,value);

    }
    return node;
}

void inorder(struct BinaryTreeNode* root){
    if(root != NULL){
    inorder(root->left);
    printf("%d", root->key);
    inorder(root->right);
    }
}