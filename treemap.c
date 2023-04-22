#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
  if(tree->lower_than(key1,key2)==0 &&
  tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  
  TreeMap *localTreeMap = (TreeMap *) malloc(sizeof(TreeMap));
  localTreeMap->lower_than = lower_than;
  localTreeMap->current = NULL;
  localTreeMap->root = NULL;
  return localTreeMap;
}

void insertTreeMap(TreeMap* tree, void* key, void* value) 
{
    if (tree == NULL || key == NULL || value == NULL) return;

    TreeNode* padre = NULL;
    TreeNode* actual = tree->root;
    while (actual != NULL) 
    {
        if (tree->lower_than(actual->pair->key, key) == 0 && tree->lower_than(key, actual->pair->key) == 0) 
        {
           return;
        } else if (tree->lower_than(actual->pair->key, key) == 0 && tree->lower_than(key, actual->pair->key) == 1) 
            {
              padre = actual;
              actual = actual->left;
            } else 
              {
                padre = actual;
                actual = actual->right;
              }
    }

    TreeNode* newNodo = createTreeNode(key, value);
    newNodo->parent = padre;
    if (tree->lower_than(padre->pair->key, key) == 1) 
    {
        padre->right = newNodo;
    } else
      {
        padre->left = newNodo;
      }
    tree->current = newNodo;
}

TreeNode * minimum(TreeNode * x)
{
  while(x->left != NULL)
    x =x->left;
  return x;
}

void removeNode(TreeMap * tree, TreeNode* node) 
{
  if (tree == NULL || node == NULL) return;
  if (node->left == NULL && node->right == NULL)
  {
    if (node->parent != NULL)
    {
      if (node->parent->left == node)
      {
        node->parent->left = NULL;
      }
      else
      {
        node->parent->right = NULL;
      }
    }
    else
    {
      tree->root = NULL;
    }
  }
  else if (node->left == NULL || node->right == NULL)
    {
      TreeNode* hijo = (node->left != NULL) ? node->left : node->right;
      if (node->parent != NULL)
      {
        if (node->parent->left == node)
            node->parent->left = hijo;
        else
            node->parent->right = hijo;
      }
      else 
      {
        tree->root = hijo; 
      }
        hijo->parent = node->parent;
    }
    else
    {
    
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {

    if (tree == NULL || tree->root == NULL || key == NULL) return NULL;
    TreeNode* current = tree->root;
  
    while (current != NULL) 
    {
        if (tree->lower_than(key,current->pair->key)==0 && tree->lower_than(current->pair->key,key)==0) 
        {
            tree->current = current;
            return current->pair;
        } 
        else 
          if(tree->lower_than(current->pair->key, key) == 0)
          {
            current = current->left;
          } 
          else  
            current = current->right;
    }
  
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
  if (tree == NULL || tree->root == NULL || key == NULL) return NULL;

  tree->current = tree->root;
  TreeNode *current=NULL;

  while(tree->current != NULL)
  {  
    int aux = tree->lower_than(tree->current->pair->key, key);
    if(is_equal(tree, tree->current->pair->key, key)) return tree->current->pair;
    else if (aux == 1)
          {
           tree->current = tree->current->right;
          }
          else
            {
              current=tree->current;
              tree->current = tree->current->left;
            }
    
  }
  
  if(current!=NULL) return current->pair;
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
  return minimum(tree->root)->pair;
}


Pair * nextTreeMap(TreeMap * tree) 
{
  if (tree == NULL || tree->current == NULL) return NULL;

  TreeNode* current = tree->current;
  if (current->right != NULL) 
  {
    current = current->right;
    while (current->left != NULL)
    {
        current = current->left;
    }
    tree->current = current;
    return current->pair;
  }
  else
  {
    TreeNode* padre = current->parent;
    while (padre != NULL && current == padre->right) 
    {
      current = padre;
      padre = padre->parent;
    }
     tree->current = padre;
    if (padre == NULL) return NULL;
    return padre->pair;
  }
  
  return NULL;
}