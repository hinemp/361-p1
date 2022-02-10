#include <stdbool.h>
#include <stdio.h>

#include "intmodel.h"
#include "statemodel.h"

/* Return an FSM that links to these internals */
fsm_t *
int_init (char const *input)
{
  return NULL;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */
