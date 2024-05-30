/**
@file map
@author Ethan Browne, efbrowne
Contains all of the map related functions
*/

#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include "value.h"

/** Lowest-numbered symbol ina  key. */
#define FIRST_SYM '!'

/** Number of possible symbols in a key. */
#define SYM_COUNT ( '~' - '!' + 1 )

/** Short name for the node used to build this tree. */
typedef struct NodeStruct Node;

/** Node in the trie data structure. */
struct NodeStruct {
  /** If the substring to the root of the tree up to this node is a
      key, this is the value that goes with it. */
  Value *val;
  
  /** Array of pointers to child nodes. */
  Node *child[ SYM_COUNT ];
};

/** Representation of a trie implementation of a map. */
struct MapStruct {
  /** Root node of this tree. */
  Node *root;
  int size;
};

/**
Makes an empty, dynamically allocated Map, initializing its fields
@return a pointer to the map
*/
Map *makeMap()
{
  Map *m = (Map *) malloc( sizeof( Map ) );
  m->root = NULL;
  m->size = 0;
  return m;
}

/**
Function returns the current number of key / value pairs in the given map
@return the size of the map
*/
int mapSize( Map *m )
{
  return m->size;
}

/**
Allocates space for a node and initializes its fields
@return the node
*/
Node* initializeNode()
{
    Node *n = (Node *) malloc( sizeof( Node ) );
    n->val = NULL;
    for (int i = 0; i < SYM_COUNT; i++){
      (n->child)[i] = NULL;
    }
    return n;
}

/**
Adds the given key / value pair to the given map
If the key is already in the map, it replaces its value with the given value
The map will take ownership of the given value object (but not the key)
It can use the value as part of its representation and it’s responsible for freeing it when it’s no longer needed.
@param m the map
@param key the key
@param val the value
*/
void mapSet( Map *m, char const *key, Value *val )
{
  Node **n = &m->root;
  for (int i = 0; key[i]; i++){
    if (*n == NULL) {
      *n = initializeNode();
    }
    n = &((*n)->child[key[i] - '!']);
  }
  if (*n == NULL) {
    *n = initializeNode();
    m->size++;
  }
  if ((*n)->val != NULL) {
    (*n)->val->destroy((*n)->val);
    (*n)->val = NULL;
  }
  (*n)->val = val;
}

/**
Function returns the value associated with the given key
If the key isn’t in the map, it returns NULL. The returned Value is still considered part of the map representation and is still owned by the map.
@param key the key
@return the value associated with the key
*/
Value *mapGet( Map *m, char const *key )
{
  Node **n = &m->root;
  for (int i = 0; key[i]; i++){
    if (key[i] - '!' > SYM_COUNT || key[i] - '!' < 0) {
      return NULL;
    }
    if (*n == NULL) {
      return NULL;
    }
    n = &((*n)->child[key[i] - '!']);
  }
  if (*n == NULL) {
    return NULL;
  }
  return (*n)->val;
}

/**
This function removes the key / value pair for the given key from the map, freeing all the memory for that pair
@return true if there was a matching key in the map and returns false otherwise.
*/
bool mapRemove( Map *m, char const *key )
{
  Node **n = &m->root;
  for (int i = 0; key[i]; i++){
    if (key[i] - '!' > SYM_COUNT || key[i] - '!' < 0) {
      return false;
    }
    if (*n == NULL) {
      return false;
    }
    n = &((*n)->child[key[i] - '!']);
  }
  if (*n == NULL) {
    return false;
  }
  (*n)->val->destroy((*n)->val);
  (*n)->val = NULL;
  m->size--;
  return true;
}

/**
Recursively frees all the nodes in the map
@param the node to free
*/
void freeMapHelper(Node *n)
{
  for (int i = 0; i < SYM_COUNT; i++){
    if ((n->child)[i] != NULL){
      freeMapHelper((n->child)[i]);
      (n->child)[i] = NULL;
    }
  }
  if (n->val != NULL) {
    n->val->destroy(n->val);
    n->val = NULL;
  }
  if (n != NULL) {
    free(n);
  }
  n = NULL;
}

/**
This function frees all the memory used to store the given map, including the memory used by all the Nodes and the Values inside them.
@param m the map to free
*/
void freeMap( Map *m )
{
  if (m->root != NULL) {
    freeMapHelper(m->root);
  }
  if (m != NULL) {
    free(m);
  }
}