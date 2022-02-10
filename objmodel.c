#include <stdbool.h>
#include <stdlib.h>

#include "objmodel.h"
#include "statemodel.h"

/* Return an FSM that links to these internals */
fsm_t *
object_init (char const *input)
{
  return NULL;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

// In AppendKeyValuePair, use a combination of strncat() and
// snprintf() to create format strings like the following:
//   printf ("KEYS[%s] = %" PRId64 "\n", ...
//   printf ("KEYS[%s] = %s\n", ...
// These strings should be concatenated with previous key-value
// pairs and stored somewhere that the accept_object can
// retrieve them later. Note that you can use realloc() to
// resize an existing dynamically
//  allocated string to make space to concatenate.

// For syntax errors, if there is a newline character ('\n'),
// replace it with a null byte ('\0'), then use this format
// string:

// printf ("SYNTAX ERROR: Could not process text beginning at '%s'\n",
