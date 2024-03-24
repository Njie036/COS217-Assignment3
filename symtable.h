/*--------------------------------------------------------------------*/
/* SymTable.h                                                         */
/* Author: Ndongo Njie                                                */
/*--------------------------------------------------------------------*/

#ifndef SymTable_INCLUDED
#define SymTable_INCLUDED
#include <stddef.h>

/* A SymTable_T is a last-in-first-out collection of items. */

typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* Return a new SymTable_T object, or NULL if insufficient memory is
   available. */

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Free oSymTable. */

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Takes oSymTable as an arggument and return the number of bindings in 
oSymTable. */

size_t SymTable_getLength(SymTable_T oSymTable);


/*--------------------------------------------------------------------*/

/* Adds a new binding to oSymTable consisting of key pcKey and value 
pvValue and return 1 (TRUE). Otherwise the function leaves oSymTable 
unchanged and return 0(FALSE). Leave oSymTable unchanged if insufficient 
memory and return 0. */

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, then SymTable_replace
 must replace the binding's value with pvValue and return the old value. 
 Otherwise it must leave oSymTable unchanged and return NULL */

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);


/*--------------------------------------------------------------------*/

/* Takes two arguments oSymTable and a constant pointer to a character 
pcKey. Return 1 (TRUE) if oSymTable contains a binding whose key is 
pcKey, and 0 (FALSE) otherwise */

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);


/*--------------------------------------------------------------------*/

/* Takes two arguments oSymTable and a constant pointer to a character 
pcKey. Return the value of the binding within oSymTable whose key is 
pcKey, or NULL if no such binding exists. */

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);


/*--------------------------------------------------------------------*/

/* Takes two arguments oSymTable and a constant pointer to a character 
pcKey. If oSymTable contains a binding with key pcKey, remove that 
binding from oSymTable and return the binding's value. Otherwise the 
function must not change oSymTable and return NULL. */

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each binding in oSymTable, passing pvExtra
 as an extra parameter. That is, the function must call (*pfApply)(pcKey, 
 pvValue, pvExtra) for each pcKey/pvValue binding in oSymTable*/

void SymTable_map(SymTable_T oSymTable,
        void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
        const void *pvExtra);

#endif