/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Ndongo Njie                                                */
/* This file, symtablehash.c, implements symbol table using hash table.*/
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include "symtable.h"
#include <string.h>

/*--------------------------------------------------------------------*/
/*The sizes of the expanding hash table */
static const size_t auBucketCounts[] = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};

/* Each item is stored in a SymTableNode.  SymTableNodes are linked to
   form a list.  */

struct SymTableNode
{
   /* The key */
   const char *pcKey;

    /* The value */
   const void *pvValue;

   /* The address of the next SymTableNode. */
   struct SymTableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTableNode. */

struct SymTable
{
   /* The address of the first SymTableNode. */
   struct SymTableNode **psFirstNode;

   /* The number of Bindings/Nodes */
   size_t numBindings;
   /*Number of linked lists in the hash table */
   size_t numOfLinkedlists;
};


/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}


/* Resize the symbol table if necessary */
static int Resize_if_needed(SymTable_T oSymTable) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    size_t maxIndexOfHash;
    size_t newBucketIndex;
    
    assert(oSymTable != NULL);

    maxIndexOfHash = (size_t)(sizeof(auBucketCounts)) / (sizeof(auBucketCounts[0]) - 1);

    if (oSymTable->numOfLinkedlists == auBucketCounts[maxIndexOfHash]) {
        return 0; 
    }

    newBucketIndex = 0;
    while ((newBucketIndex < maxIndexOfHash) && (auBucketCounts[newBucketIndex]
     <= oSymTable->numOfLinkedlists)) {
        newBucketIndex++;
    }

    if (newBucketIndex < maxIndexOfHash) {
        size_t newBucketCount = auBucketCounts[newBucketIndex];
        struct SymTableNode **newTable = calloc(newBucketCount, sizeof(struct SymTableNode *));
        
        if (newTable == NULL) {
            return 0; 
        }

        size_t i;

        /* Transfer all elements into the new hash table */
        for (i = 0; i < oSymTable->numOfLinkedlists; i++) {
            psCurrentNode = oSymTable->psFirstNode[i];
            while (psCurrentNode != NULL) {
                psNextNode = psCurrentNode->psNextNode;
                size_t newIndex = SymTable_hash(psCurrentNode->pcKey, newBucketCount);
                psCurrentNode->psNextNode = newTable[newIndex];
                newTable[newIndex] = psCurrentNode;
                psCurrentNode = psNextNode;
            }
        }

        free(oSymTable->psFirstNode);
        oSymTable->psFirstNode = newTable;
        oSymTable->numOfLinkedlists = newBucketCount;
    }

    return 1; /* Successfully resized not needed */
}

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstNode = calloc(auBucketCounts[0], sizeof(struct SymTableNode*));
   if (oSymTable->psFirstNode == NULL) {
    free(oSymTable);
    return NULL;
   }
   oSymTable->numBindings = 0;
   oSymTable->numOfLinkedlists = auBucketCounts[0];
   return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   struct SymTableNode *psCurrentNode;
   struct SymTableNode *psNextNode;
   size_t index;

   assert(oSymTable != NULL);

    for (index = 0; index < oSymTable->numOfLinkedlists; index++) {
        for (psCurrentNode = oSymTable->psFirstNode[index];
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
   {
      psNextNode = psCurrentNode->psNextNode;
      /*Free where the key points to before freeing the main node */
      free((char*)psCurrentNode->pcKey); 
      free(psCurrentNode);
   }

    }
    free(oSymTable->psFirstNode);
    free(oSymTable);
}


/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable)
{
    assert(oSymTable != NULL);
    return oSymTable->numBindings;
}


/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNewNode;
    size_t hashIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashIndex = SymTable_hash(pcKey, oSymTable->numOfLinkedlists);

    /* Check if resize is needed before insertion */
    if (!Resize_if_needed(oSymTable)) {
        return 0; /* Maximum size reached */
    }

    /*Searching for duplicate key*/
    for (psCurrentNode = oSymTable->psFirstNode[hashIndex];
        psCurrentNode != NULL;
        psCurrentNode =psCurrentNode->psNextNode)
    {
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) return 0;
    }
    /*It is not a duplicate, make space for the new node and key copy*/
    psNewNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
    if (psNewNode == NULL)
      return 0;

    psNewNode->pcKey = malloc(strlen(pcKey)+1);
    if (psNewNode->pcKey == NULL) {
        free(psNewNode);
        return 0;
    }

    /*We have a space and should copy the key and value and insert the new node*/
    psNewNode->pcKey = strcpy((char*)psNewNode->pcKey, pcKey);
    psNewNode->pvValue = pvValue;

    psNewNode->psNextNode = oSymTable->psFirstNode[hashIndex];
    oSymTable->psFirstNode[hashIndex] = psNewNode;
    oSymTable->numBindings++;
    return 1; /*Successfully inserted a new node*/
}


/*--------------------------------------------------------------------*/

/*Similar to get but saves the old value, replaces it and returns the old value*/
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) { 
    struct SymTableNode *psCurrentNode;
    const void *oldValue;
    size_t hashIndex;


    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashIndex = SymTable_hash(pcKey, oSymTable->numOfLinkedlists);

    for (psCurrentNode = oSymTable->psFirstNode[hashIndex];
        psCurrentNode != NULL;
        psCurrentNode =psCurrentNode->psNextNode)
    {
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) {
            oldValue = psCurrentNode-> pvValue;
            psCurrentNode->pvValue = pvValue;
            return (void*)oldValue;
        }
    }
    return NULL; /*Does not find the pcKey */
}  


/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    size_t hashIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    /*  assert(psCurrentNode->pcValue != NULL);*/

    hashIndex = SymTable_hash(pcKey, oSymTable->numOfLinkedlists);

    for (psCurrentNode = oSymTable->psFirstNode[hashIndex];
        psCurrentNode != NULL;
        psCurrentNode =psCurrentNode->psNextNode)
    {
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) return 1;
    }
    return 0; /*Does not find the pcKey */
}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
   struct SymTableNode *psCurrentNode;
   size_t hashIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

   hashIndex = SymTable_hash(pcKey, oSymTable->numOfLinkedlists);

    for (psCurrentNode = oSymTable->psFirstNode[hashIndex];
        psCurrentNode != NULL;
        psCurrentNode =psCurrentNode->psNextNode)
    {
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) {
            return (void*)psCurrentNode -> pvValue;
        }
    }
    return NULL; /*Does not find the pcKey */    
}

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psPrevNode;
    const void *value;
    size_t hashIndex;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    psPrevNode = NULL;

    hashIndex = SymTable_hash(pcKey, oSymTable->numOfLinkedlists);

    /*Searching for key to remove*/
    for (psCurrentNode = oSymTable->psFirstNode[hashIndex];
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
    {   
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) {
            /*We found the key to remove*/
            value = psCurrentNode->pvValue;
            if (psPrevNode == NULL) {
                oSymTable->psFirstNode[hashIndex] = psCurrentNode->psNextNode;
            }
            else {
                psPrevNode->psNextNode = psCurrentNode->psNextNode;
            }
            free((char*)psCurrentNode->pcKey); 
            free(psCurrentNode);
            oSymTable->numBindings--;
            return (void*)value;
        }
        psPrevNode = psCurrentNode;
    }
    return NULL; 
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
               void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
               const void *pvExtra)
{
   struct SymTableNode *psCurrentNode;
   size_t index;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (index = 0; index < oSymTable->numOfLinkedlists; index++) {
    for (psCurrentNode = oSymTable->psFirstNode[index];
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
      (*pfApply)(psCurrentNode->pcKey, (void*)psCurrentNode->pvValue, (void*)pvExtra);
   }
}