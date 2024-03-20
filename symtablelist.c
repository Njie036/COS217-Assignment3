/*--------------------------------------------------------------------*/
/* SymTable.c (Version 6)                                             */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include "symtable.h"
#include <string.h>

/*--------------------------------------------------------------------*/

/* Each item is stored in a SymTableNode.  SymTableNodes are linked to
   form a list.  */

struct SymTableNode
{
   /* The key */
   const char *pcKey;

    /* The value*/
   const void *pvValue;

   /* The address of the next SymTableNode. */
   struct SymTableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTableNode. */

struct SymTable
{
   /* The address of the first SymTableNode. */
   struct SymTableNode *psFirstNode;

   /* The number of Bindings/Nodes */
   size_t numBindings;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstNode = NULL;
   oSymTable->numBindings = 0;
   return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   struct SymTableNode *psCurrentNode;
   struct SymTableNode *psNextNode;

   assert(oSymTable != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
   {
      psNextNode = psCurrentNode->psNextNode;
      /*Free where the key points to before freeing the main node */
      free((char*)psCurrentNode->pcKey); 
      free(psCurrentNode);
   }
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

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    /*Searching for duplicate key*/
    for (psCurrentNode = oSymTable->psFirstNode;
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

    psNewNode = oSymTable->psFirstNode;
    oSymTable->psFirstNode = psNewNode;
    oSymTable->numBindings++;
    return 1; /*Successfully inserted a new node*/
}


/*--------------------------------------------------------------------*/

/*Similar to get but saves the old value, replaces it and returns the old value*/
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) { 
    struct SymTableNode *psCurrentNode;
    const void *oldValue;


    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode =psCurrentNode->psNextNode)
    {
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) {
            oldValue = psCurrentNode->pcKey;
            psCurrentNode = pvValue;

            return oldValue;
        }
    }
    return NULL; /*Does not find the pcKey */
}  


/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
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

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
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

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    psPrevNode = NULL;

    /*Searching for key to remove*/
    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
    {   
        if(strcmp(pcKey,psCurrentNode->pcKey) == 0) {
            /*We found the key to remove*/
            value = psCurrentNode->pvValue;
            if (psPrevNode == NULL) {
                oSymTable->psFirstNode = psCurrentNode->psNextNode;
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

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
      (*pfApply)(psCurrentNode->pcKey, (void*)psCurrentNode->pvValue, (void*)pvExtra);
}