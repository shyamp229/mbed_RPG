/*
 Student Name:  Shyam Patel
 Date: 11 / 6 / 2018

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {

  HashTableEntry* newEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));    // Allocates mem for new HashTableEntry struct on heap

  // Initializing data
  newEntry->key = key;              // init key
  newEntry->value = value;          // init value
  newEntry->next = NULL;            // next entry is NULL

  return newEntry;                  // returns newEntry
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {

    HashTableEntry* node = hashTable->buckets[hashTable->hash(key)];        // node = finds HashTableEntry that contains specific key

    while(node){                                                            // doing while to search for key = specific key
      if(node->key == key)                                                  // if key == specific key
        return node;                                                        // returns key
      node = node->next;                                                    // checks next key if specific key is not found
    }
    return NULL;                                                            // if not key is not found then returns as NULL

}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {

    for (int i=0;i<(hashTable->num_buckets); i++){          // for loop to check every bucket
      HashTableEntry* node = hashTable -> buckets[i];       // node = current positioning of bucket

    if (node){                                              // if node isn't equal to NULL then free values

        HashTableEntry* newnode = node -> next;              // newnode = next node in line

        while(newnode){                                     // while newnode isn't equal to NULL
          free(node->value);                                // frees value in current node
          free(node);                                       // frees node
          node = newnode;                                   // new node is equal to node
          newnode = newnode->next;                          // INC newnode
        }
        free(node);                                         // frees tail
       }
    }
    free(hashTable);                                        // frees hashTable to destory it
    return;

}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {

    int index = hashTable->hash(key);                          // index = index for buckets
    HashTableEntry* FoundNode = findItem(hashTable,key);         // gets FoundNode through findItem

    if(FoundNode){                                               // if FoundNode isn't equal to NULL then overwrite
      void* Foundvalue;                                          // creating Foundvalue to save FoundNode values
      Foundvalue = FoundNode->value;                             // Foundvalue points to  existing values
      FoundNode->value = value;                                  // Overwrites value
      return Foundvalue;                                         // Returns Foundvalue
    }

    // newEntry = newHastTableEntry (values and key)
    HashTableEntry* newEntry=createHashTableEntry(key,value);           // newEntry to insert

    newEntry->next=hashTable->buckets[index];            // gets new head
    hashTable->buckets[index] = newEntry;                // new head is pointed to newEntry in HT

    return NULL;                                         // Returns null
}

void* getItem(HashTable* hashTable, unsigned int key) {

   HashTableEntry* node = findItem(hashTable,key);      // node = use findItem to find specific key
   if(node)                                             // if node isn't equal to NULL
    return node->value;                                 // then returns nodes value
  return NULL;                                          // returns null if node = NULL

}

void* removeItem(HashTable* hashTable, unsigned int key) {
/**
 * removeItem
 *
 * Remove the item in hash table based on the key and return the value stored in it.
 * In other words, return the value and free the hash table entry from heap.
 *
 * @param myHashTable The pointer to the hash table.
 * @param key The key that corresponds to the item.
 * @return the pointer of the value corresponding to the key, or NULL if the key is not present
 **/

  int index=hashTable->hash(key);                               //index of bucket

  HashTableEntry* node = hashTable->buckets[index];             // node = hashTable pointing to that bucket aka head

  if(node && node->key==key){                                   // if node exists and its key

    void* RemoveValue = node->value;                            //  RemoveValue = value of current index
    hashTable->buckets[index] = node -> next;                   // new head
    free(node);                                                 // frees node

    return RemoveValue;                                         // Returns RemoveValue
  }

  while(node && node->next){                                    // while node exists next node isn't null

    if(node->next->key == key){                                 // checks next nodes if its not a NULL

      HashTableEntry* NeedRemoved = node->next;                 // NeedRemoved saves value to remove in next node
      void* deleteValue = NeedRemoved->value;                   // deletes value that needs to be removed
      node->next=node->next->next;                              // node looks ahead
      free(NeedRemoved);                                        // frees the next node
      return deleteValue;                                       // since next node is not a NULL so it returns value of next node
    }
    node = node->next;                                          // Goes to next node
  }
  return NULL;                                                  // returns NULL
}

void deleteItem(HashTable* hashTable, unsigned int key){
/*  deleteItem
  1.) check if item exists, if not then its NULL
  2.) if node key matches specific key then free the value and node
  3.) see if we need to delete twice
  4.) make sure to point around the deleted bucket
*/

  int index = hashTable->hash(key);                             // index of bucket

  HashTableEntry* node = hashTable->buckets[index];             // node = hashTable pointing to a specific bucket

  if (findItem(hashTable,key) == NULL)                          // uses findItem to check if item is NULL
    return;

  if (node && node -> key == key){                              // if node exists & nodes key = specific key then delete it
    hashTable->buckets[index] = node -> next;                   // changes head
    free(node->value);                                          // deletes the node value
    free(node);                                                 // deletes the node
    return;                                                     // return
  }
  while(node && node -> next){                                  // while node exists & next node isn't null

    if(node->next->key == key){                                 // if next node key matches specific key then deleteItem
      HashTableEntry* deleteNextNode = node->next;              // deleteNextNode saves value of next node
      node->next = node -> next -> next;                        // pointing the node around the one im deleting
      free(deleteNextNode->value);                              // deletes the values of next node
      free(deleteNextNode);                                     // deletes next node
      return;                                                   //return
    }
    node = node->next;                                          // goes to next node
  }
}
