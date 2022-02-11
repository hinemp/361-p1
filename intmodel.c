#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "intmodel.h"
#include "statemodel.h"

static void SetNegative (fsm_t *);
static void SetMultiplier (fsm_t *, int);
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
//  ZERO      HYPHEN    NZ_DIGIT    DIGIT      TERM_INIT   NON_DIGIT
   {OCTAL,    SIGN,     MAGNITUDE,  NON_INT,   NON_INT,    NON_INT,},   // INT_INIT
   {NON_INT,  NON_INT,  NON_INT,    MAGNITUDE, INT_FINISH, INT_ERROR,}, // MAGNITUDE
   {OCTAL,    NON_INT,  MAGNITUDE,  NON_INT,   NON_INT,    INT_ERROR,}, // SIGN
   {OCTAL,    NON_INT,  MAGNITUDE,  NON_INT,   INT_FINISH, INT_ERROR,}, // OCTAL
   {NON_INT , NON_INT,  NON_INT,    NON_INT,   NON_INT,    NON_INT,},   // INT_FINISH
   {NON_INT , NON_INT,  NON_INT,    NON_INT,   NON_INT,    NON_INT,},   // INT_ERROR
  };

static void
SetNegative (fsm_t *fsm)
{
  fsm->is_negative = true;
}

static void
SetMultiplier (fsm_t *fsm, int multiplier)
{
  fsm->multiplier = multiplier;
}

static void
MultAndAdd (fsm_t *fsm)
{
  fsm->build_int *= fsm->multiplier;
  int to_add = fsm->current[0] - '0';
  fsm->build_int += to_add;
}

static void
SyntaxError (fsm_t *fsm)
{
  
}