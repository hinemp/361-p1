#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "intmodel.h"
#include "statemodel.h"

static void SetNegative (fsm_t *);
static void SetMultiplier_8 (fsm_t *);
static void SetMultiplier_10 (fsm_t *);
static void MultAndAdd (fsm_t *);
static void SyntaxError (fsm_t *);
static state_t parse_transition (fsm_t *, event_t, action_t *, action_t *);

/* Return an FSM that links to these internals */
fsm_t *
int_init (char const *input)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->nevents = NINT_EVENTS;
  fsm->state = INT_INIT;
  fsm->transition = parse_transition;

  // Extra Fields
  fsm->input = input;
  fsm->current = input;
  fsm->length = 0;

  // Int Fields
  fsm->build_int = 0;
  fsm->is_negative = false;

  return fsm;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

static state_t const _transition[NINT_STATES][NINT_EVENTS] = {
//  ZERO      HYPHEN    NZ_DIGIT    DIGIT      TERM_INIT   NON_DIGIT
   {OCTAL,    SIGN,     MAGNITUDE,  NON_INT,   NON_INT,    NON_INT,},   // INT_INIT
   {NON_INT,  NON_INT,  NON_INT,    MAGNITUDE, INT_FINISH, INT_ERROR,}, // MAGNITUDE
   {OCTAL,    NON_INT,  MAGNITUDE,  NON_INT,   NON_INT,    INT_ERROR,}, // SIGN
   {OCTAL,    NON_INT,  MAGNITUDE,  NON_INT,   INT_FINISH, INT_ERROR,}, // OCTAL
   {NON_INT , NON_INT,  NON_INT,    NON_INT,   NON_INT,    NON_INT,},   // INT_FINISH
   {NON_INT , NON_INT,  NON_INT,    NON_INT,   NON_INT,    NON_INT,},   // INT_ERROR
  };

static action_t const _effect[NINT_STATES][NINT_EVENTS] = {
//  ZERO      HYPHEN        NZ_DIGIT            DIGIT       TERM_INT  NON_DIGIT
   {NULL,     SetNegative,  SetMultiplier_10,   NULL,       NULL,     NULL,},   // INT_INIT
   {NULL,     NULL,         NULL,               MultAndAdd, NULL,     SyntaxError,}, // MAGNITUDE
   {NULL,     NULL,         SetMultiplier_10,   NULL,       NULL,     SyntaxError,}, // SIGN
   {NULL,     NULL,         SetMultiplier_8,    NULL,       NULL,     SyntaxError,}, // OCTAL
   {NULL,     NULL,         NULL,               NULL,       NULL,     NULL,},   // INT_FINISH
   {NULL,     NULL,         NULL,               NULL,       NULL,     NULL,},   // INT_ERROR
  };

static action_t const _entry[NINT_STATES] = {
  // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NULL
  NULL, NULL, NULL, NULL, NULL, NULL,
};

/* Given FSM instance and event, perform the table lookups */
static state_t
parse_transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  if (fsm->state >= NON_INT || event >= NIL_INT || _transition[fsm->state][event] == NON_INT) 
    return -1;
  
  *effect = _effect[fsm->state][event];
  state_t next = _transition[fsm->state][event];
  if (next != NON_INT)
    *entry = _entry[next];
  
  return next;
}

static void
SetNegative (fsm_t *fsm)
{
  fsm->is_negative = true;
}

static void
SetMultiplier_8 (fsm_t *fsm)
{
  fsm->multiplier = 8;
}

static void
SetMultiplier_10 (fsm_t *fsm)
{
  fsm->multiplier = 10;
}

static void
MultAndAdd (fsm_t *fsm)
{
  fsm->build_int *= fsm->multiplier;
  int to_add = fsm->current[0] - '0';
  if (fsm->is_negative) 
    {
      fsm->build_int -= to_add;
    }
  else
    {
      fsm->build_int += to_add;
    }
}

static void
SyntaxError (fsm_t *fsm)
{
  printf("SYNTAX ERROR: '%c' is not a valid digit", fsm->current[0]);
}