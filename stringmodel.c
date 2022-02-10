#include <stdbool.h>
#include <stdlib.h>

#include "statemodel.h"
#include "stringmodel.h"

static void AdvancePointer (fsm_t *);
static void AllocateBuffer (fsm_t *);
static void AppendCharacter (fsm_t *);
static void ReplaceCharacter (fsm_t *);
static void SyntaxError (fsm_t *);

/* Return an FSM that links to these internals */
fsm_t *
string_init (char const *input)
{
  return NULL;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

// Use the following print format string if a syntax error is
// encountered. The two characters should be the last one
// successfully processed and the next character causing the
// error.
// printf ("SYNTAX ERROR: '%c%c' is not a valid escape code\n",

// /////////////////// EFFECT FUNCTIONS ////////////////////

/* Used to move beyond the quote at the end of the string */
static void
AdvancePointer (fsm_t *fsm)
{
}

/* Create a dynamically allocated buffer for storing the string as it is
   being built. Note that you will need to modify the fsm_t struct declaration
   to include whatever fields you may need for managing the buffer. */
static void
AllocateBuffer (fsm_t *fsm)
{
}

/* Append a character from the current string to a buffer */
static void
AppendCharacter (fsm_t *fsm)
{
}

/* Replaces a control sequence (\\ or \") by putting just the
   latter character into the buffer */
static void
ReplaceCharacter (fsm_t *fsm)
{
}

/* Reports an invalid escape-code character */
static void
SyntaxError (fsm_t *fsm)
{
}
