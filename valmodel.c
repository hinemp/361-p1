#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intmodel.h"
#include "parser.h"

#include "statemodel.h"
#include "stringmodel.h"
#include "valmodel.h"

static void ActivateString (fsm_t *);
static void ActivateInteger (fsm_t *);
static void SyntaxError (fsm_t *);
static state_t parse_transition (fsm_t *, event_t, action_t *, action_t *);

/* Return an FSM that links to these internals */
fsm_t *
value_init (char const *input)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->nevents = NVAL_EVENTS;
  fsm->state = VAL_INIT;
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

static state_t const _transition[NVAL_STATES][NVAL_EVENTS] = {
  //  START_VALUE WHITESPACE  START_STR  END_STR     START_INT  END_INT
  //  BAD_VALUE
  { VAL_SKIP, NON_VAL, NON_VAL, NON_VAL, NON_VAL, NON_VAL,
    NON_VAL }, // VAL_INIT
  { NON_VAL, VAL_SKIP, BUILD_STR, NON_VAL, BUILD_INT, NON_VAL,
    VAL_ERROR }, // VAL_SKIP
  { NON_VAL, NON_VAL, NON_VAL, VAL_FINISH, NON_VAL, NON_VAL,
    VAL_ERROR }, // BUILD_STR
  { NON_VAL, NON_VAL, NON_VAL, NON_VAL, NON_VAL, VAL_FINISH,
    VAL_ERROR }, // BUILD_INT
  { NON_VAL, NON_VAL, NON_VAL, NON_VAL, NON_VAL, NON_VAL,
    NON_VAL }, // VAL_FINISH
  { NON_VAL, NON_VAL, NON_VAL, NON_VAL, NON_VAL, NON_VAL,
    NON_VAL }, // VAL_ERROR
};

static action_t const _effect[NVAL_STATES][NVAL_EVENTS] = {
  //  START_VALUE WHITESPACE  START_STR  END_STR   START_INT  END_INT BAD_VALUE
  {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
  }, // VAL_INIT
  {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      SyntaxError,
  }, // VAL_SKIP
  {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
  }, // BUILD_STR
  {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
  }, // BUILD_INT
  {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
  }, // VAL_FINISH
  {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
  }, // VAL_ERROR
};

static action_t const _entry[NVAL_STATES] = {
  //  VAL_INIT    VAL_SKIP    BUILD_STR       BUILD_INT         VAL_FINISH
  //  VAL_ERROR
  NULL, NULL, ActivateString, ActivateInteger, NULL, NULL,
};

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

/* Given FSM instance and event, perform the table lookups */
static state_t
parse_transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  if (fsm->state >= NON_VAL || event >= NIL_VAL
      || _transition[fsm->state][event] == NON_VAL)
    return -1;
  *effect = _effect[fsm->state][event];
  state_t next = _transition[fsm->state][event];
  if (next != NON_VAL)
    *entry = _entry[next];
  return next;
}

static void
ActivateString (fsm_t *fsm)
{
  fsm_t *str_fsm = string_init (fsm->current);
  fsm->is_val_ok = accept_string (str_fsm, &fsm->buffer);
  if (!fsm->is_val_ok)
    {
      fsm->buffer = NULL;
      free (str_fsm->buffer);
    }
  fsm->is_val_str = true;
  fsm->current = str_fsm->current;
  fsm->current++;
  fsm->current++;
  free (str_fsm);
}

static void
ActivateInteger (fsm_t *fsm)
{
  fsm_t *int_fsm = int_init (fsm->current);
  fsm->is_val_ok = accept_integer (int_fsm, &fsm->build_int);
  fsm->is_val_str = false;
  fsm->current = int_fsm->current;
  free (int_fsm);
}

// Use this format string for syntax errors:
// printf ("SYNTAX ERROR: '%c' is an invalid token\n", ...
static void
SyntaxError (fsm_t *fsm)
{
  printf ("SYNTAX ERROR: '%c' is an invalid token\n", fsm->current[0]);
}