#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

struct BinaryTreeNode *newNodeCreate(char *value, int position){
    struct BinaryTreeNode *temp = (struct BinaryTreeNode*) malloc(sizeof(struct BinaryTreeNode));
    temp->key = strdup(value); 
    temp->position = position; 
    temp->left = temp->right = NULL;
    return temp;
}

struct BinaryTreeNode *insertNode(struct BinaryTreeNode* node, char *value, int *nextPos){
    if(node == NULL){
        int pos = *nextPos;
        (*nextPos)++;
        return newNodeCreate(value, pos);
    }
    
    int cmp = strcmp(value, node->key);
    if(cmp < 0)
        node->left = insertNode(node->left, value, nextPos);   
    else if(cmp > 0)
        node->right = insertNode(node->right, value, nextPos); 
    
    return node;
}

struct BinaryTreeNode* searchNode(struct BinaryTreeNode* node, char *value){
    if(node == NULL)
        return NULL;
    if(strcmp(value,node->key) == 0){
        return node;
    }
    else if (strcmp(value, node->key) < 0){
        return searchNode(node->left, value);
    }
    else {
        return searchNode(node->right, value);
    }
}

void inorder(struct BinaryTreeNode* root, FILE *fout){
    if(root != NULL){
        inorder(root->left, fout);
        fprintf(fout, "%d,%s\n", root->position,root->key);  
        inorder(root->right, fout);
    }
}

void freeTree(struct BinaryTreeNode* root){
    if(root != NULL){
        freeTree(root->left);
        freeTree(root->right);
        free(root->key);  
        free(root);
    }
}