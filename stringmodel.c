#include <stdbool.h>
#include <stdlib.h>

#include "statemodel.h"
#include "stringmodel.h"

#define NUM_STATES (NON_STR+1)
#define NUM_EVENTS (NIL_CHAR+1)

static void AdvancePointer (fsm_t *);
static void AllocateBuffer (fsm_t *);
static void AppendCharacter (fsm_t *);
static void ReplaceCharacter (fsm_t *);
static void SyntaxError (fsm_t *);
static strst_t parse_transition (fsm_t *, strevt_t, strevt_t *, strevt_t *);

/* Return an FSM that links to these internals */
fsm_t *
string_init (char const *input)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->nevents = NUM_EVENTS;
  fsm->state = STR_INIT;
  fsm->transition = parse_transition;

  // Extra Fields
  fsm->input = input;
  fsm->current = input[0];
  fsm->length = 0;

  return fsm;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

static strst_t const _transition[NUM_STATES][NUM_EVENTS] = {
   // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NIL_CHAR
   {BUILDING, NON_STR   , NON_STR , NON_STR, NON_STR , NON_STR, NON_STR}, // STR_INIT
   {NON_STR , STR_FINISH, BUILDING, ESCAPE , NON_STR , NON_STR, NON_STR}, // BUILDING
   {NON_STR , NON_STR   , NON_STR , NON_STR, BUILDING, NON_STR, NON_STR}, // ESCAPE
   {NON_STR , NON_STR   , NON_STR , NON_STR, NON_STR , NON_STR, NON_STR}, // STR_FINISH
   {NON_STR , NON_STR   , NON_STR , NON_STR, NON_STR , NON_STR, NON_STR}, // STR_ERROR
  };;

static strevt_t const _effect[NUM_STATES][NUM_EVENTS] = {
   // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NIL_CHAR
   {AllocateBuffer, NULL, NULL, NULL, NULL, NULL, NULL}, // STR_INIT
   {NULL, AdvancePointer, AppendCharacter, NULL, NULL, NULL, NULL}, // BUILDING
   {NULL, NULL, NULL, NULL, NULL, ReplaceCharacter, SyntaxError, NULL}, // ESCAPE
   {NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // STR_FINISH
   {NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // STR_ERROR
};

static strevt_t const _entry[NUM_STATES] = {
  // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NIL_CHAR
  NULL, NULL, NULL, NULL, NULL, NULL,
};

/* Given FSM instance and event, perform the table lookups */
static strst_t
parse_transition (fsm_t *fsm, strevt_t event, strevt_t *effect, strevt_t *entry)
{
  if (fsm->state >= NON_STR || event >= NIL_CHAR || _transition[fsm->state][event] == NON_STR) 
    return -1;
  
  *effect = _effect[fsm->state][event];
  strst_t next = _transition[fsm->state][event];
  if (next != NON_STR)
    *entry = _entry[next];
  
  return next;
}



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
  fsm->current = fsm->input + fsm->length;
}

/* Create a dynamically allocated buffer for storing the string as it is
   being built. Note that you will need to modify the fsm_t struct declaration
   to include whatever fields you may need for managing the buffer. */
static void
AllocateBuffer (fsm_t *fsm)
{
  memset (fsm->buffer, 0, sizeof (fsm->buffer));
}

/* Append a character from the current string to a buffer */
static void
AppendCharacter (fsm_t *fsm)
{
  assert (fsm->length < 1024 - 1);
  fsm->buffer[fsm->length++] = fsm->current;
  fsm->current++;
}

/* Replaces a control sequence (\\ or \") by putting just the
   latter character into the buffer */
static void
ReplaceCharacter (fsm_t *fsm)
{
  fsm->current++;
  assert (fsm->length < 1024 - 1);
  fsm->buffer[fsm->length++] = fsm->current;
  fsm->current++;
}

/* Reports an invalid escape-code character */
static void
SyntaxError (fsm_t *fsm)
{
  // Use the following print format string if a syntax error is
  // encountered. The two characters should be the last one
  // successfully processed and the next character causing the
  // error.

  // Print two chars:
  // last char successfully processed, 
  // next char causing error, 
  char curr = fsm->current;
  char *err = fsm->current + 1;

  printf("SYNTAX ERROR: '%c%c' is not a valid escape code\n", curr);
}

