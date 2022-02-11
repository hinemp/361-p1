#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "statemodel.h"
#include "stringmodel.h"


static void AdvancePointer (fsm_t *);
static void AllocateBuffer (fsm_t *);
static void AppendCharacter (fsm_t *);
static void ReplaceCharacter (fsm_t *);
static void SyntaxError (fsm_t *);
static state_t parse_transition (fsm_t *, event_t, action_t *, action_t *);

/* Return an FSM that links to these internals */
fsm_t *
string_init (char const *input)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->nevents = NSTR_EVENTS;
  fsm->state = STR_INIT;
  fsm->transition = parse_transition;

  // Extra Fields
  fsm->input = input;
  fsm->current = input;

  fsm->length = 0;

  return fsm;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

static state_t const _transition[NSTR_STATES][NSTR_EVENTS] = {
// OPEN_QUOTE CLOSE_QUOTE NONCTRL   BACKSLASH ESC_CHAR  NO_ESC 
   {BUILDING, NON_STR   , NON_STR , NON_STR , NON_STR , NON_STR,}, // STR_INIT
   {NON_STR , STR_FINISH, BUILDING, ESCAPE  , NON_STR , NON_STR,}, // BUILDING
   {NON_STR , NON_STR   , NON_STR , NON_STR , BUILDING, STR_ERROR,}, // ESCAPE
   {NON_STR , NON_STR   , NON_STR , NON_STR , NON_STR , NON_STR,}, // STR_FINISH
   {NON_STR , NON_STR   , NON_STR , NON_STR , NON_STR , NON_STR,}, // STR_ERROR
  };

static action_t const _effect[NSTR_STATES][NSTR_EVENTS] = {
   // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NULL
   {AllocateBuffer, NULL, NULL, NULL, NULL, NULL,}, // STR_INIT
   {NULL, AdvancePointer, AppendCharacter, NULL, NULL, NULL,}, // BUILDING
   {NULL, NULL, NULL, NULL, ReplaceCharacter, SyntaxError,}, // ESCAPE
   {NULL, NULL, NULL, NULL, NULL, NULL,}, // STR_FINISH
   {NULL, NULL, NULL, NULL, NULL, NULL,}, // STR_ERROR
};

static action_t const _entry[NSTR_STATES] = {
  // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NULL
  NULL, NULL, NULL, NULL, NULL,
};

/* Given FSM instance and event, perform the table lookups */
static state_t
parse_transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
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
  fsm->buffer = (char *) calloc (100, sizeof (char));
  memset (fsm->buffer, 0, 100 * sizeof (char));
}

/* Append a character from the current string to a buffer */
static void
AppendCharacter (fsm_t *fsm)
{
  assert (fsm->length < 1024 - 1);
  fsm->buffer[fsm->length++] = fsm->current[0];
  fsm->current++;
}

/* Replaces a control sequence (\\ or \") by putting just the
   latter character into the buffer */
static void
ReplaceCharacter (fsm_t *fsm)
{
  assert (fsm->length < 1024 - 1);
  fsm->buffer[fsm->length++] = fsm->current[0];
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
  // printf ("SYNTAX ERROR: '%c%c' is not a valid escape code\n",

  // Print two chars:
  // last char successfully processed, 
  // next char causing error, 
  fsm->current--;
  char curr = fsm->current[0];
  char err = fsm->current[1];

  printf("SYNTAX ERROR: '%c%c' is not a valid escape code\n", curr, err);
}

