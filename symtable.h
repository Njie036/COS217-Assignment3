/*--------------------------------------------------------------------*/
/* SymTable.h                                                         */
/* Author: Ndongo Njie                                                */
/* This file, symtable.h, defines the functions used to manipulate    */
/* the SymbolTable.                                                   */
/*--------------------------------------------------------------------*/

#ifndef SymTable_INCLUDED
#define SymTable_INCLUDED
#include <stddef.h>

/* SymTable_T is a data structure used to associate values with keys */

typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* Handles the new symtable function. Takes no argument but Return a 
new SymTable_T object that contains no bindings, or NULL if insufficient
memory is available. */

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Handles the function that frees the symbol table. Takes oSymTable 
as an argument and free all memory occupied by it. It does not return
anything. */

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Handles the function that gets the length of the symbol table. 
Takes oSymTable as an argument and return the number of bindings in 
oSymTable. */

size_t SymTable_getLength(SymTable_T oSymTable);


/*--------------------------------------------------------------------*/

/* Handles the put function of the symbol table. Adds a new binding to
oSymTable consisting of key pcKey and value pvValue and return 1 (TRUE).
Otherwise the function leaves oSymTable unchanged and return 0(FALSE). 
Leave oSymTable unchanged if insufficient memory and return 0. */

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* Handles the replace function of the symbol table. If oSymTable 
contains a binding with key pcKey, then SymTable_replace must replace 
the binding's value with pvValue and return the old value. Otherwise it 
must leave oSymTable unchanged and return NULL */

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);


/*--------------------------------------------------------------------*/

/* Handles the contain function of the symbol table. Takes two arguments 
oSymTable and a constant pointer to a character pcKey. Return 1 (TRUE) 
if oSymTable contains a binding whose key is pcKey, and 0 (FALSE) 
otherwise */

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);


/*--------------------------------------------------------------------*/

/* Hanldes the get function of the symbol table. Takes two arguments 
oSymTable and a constant pointer to a character pcKey. Return the value 
of the binding within oSymTable whose key is pcKey, or NULL if no such 
binding exists. */

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);


/*--------------------------------------------------------------------*/

/* Handles the remove function of the symbol table. Takes two arguments 
oSymTable and a constant pointer to a character pcKey. If oSymTable 
contains a binding with key pcKey, remove that binding from oSymTable 
and return the binding's value. Otherwise the function must not change 
oSymTable and return NULL. */

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Handles the map function of the symbol table. Apply function *pfApply 
to each binding in oSymTable, passing pvExtra as an extra parameter. 
That is, the function must call (*pfApply)(pcKey, pvValue, pvExtra) for 
each pcKey/pvValue binding in oSymTable*/

void SymTable_map(SymTable_T oSymTable,
        void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
        const void *pvExtra);

#endif