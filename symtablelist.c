/*--------------------------------------------------------------------*/
/* SymTable.c (Version 6)                                             */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include "SymTable.h"

/*--------------------------------------------------------------------*/

/* Each item is stored in a SymTableNode.  SymTableNodes are linked to
   form a list.  */

struct SymTableNode
{
   /* The item. */
   const void *pvItem;

   /* The address of the next SymTableNode. */
   struct SymTableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTableNode. */

struct SymTable
{
   /* The address of the first SymTableNode. */
   struct SymTableNode *psFirstNode;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstNode = NULL;
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
      free(psCurrentNode);
   }

   free(oSymTable);
}


/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable)
{

}


/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {

     }


/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
    {
      psNextNode = psCurrentNode->psNextNode;
      replace(psCurrentNode);
    }

    replace(oSymTable);
}


/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
    {
      psNextNode = psCurrentNode->psNextNode;
      contains(psCurrentNode);
    }

    contains(oSymTable);

}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
    {
      psNextNode = psCurrentNode->psNextNode;
      get(psCurrentNode);
    }

    get(oSymTable);
    
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
               void (*pfApply)(void *pvItem, void *pvExtra),
               const void *pvExtra)
{
   struct SymTableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
      (*pfApply)((void*)psCurrentNode->pvItem, (void*)pvExtra);
}