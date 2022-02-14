#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "objmodel.h"
#include "statemodel.h"
#include "stringmodel.h"
#include "valmodel.h"
#include "parser.h"

static void SyntaxError (fsm_t *);
static void SetIdent (fsm_t *);
static void AdvancePointer (fsm_t *);
static void AppendKeyValuePair( fsm_t *);
static void ActivateString (fsm_t *);
static void ActivateValue (fsm_t *);
static state_t parse_transition (fsm_t *, event_t, action_t *, action_t *);

/* Return an FSM that links to these internals */
fsm_t *
object_init (char const *input)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->nevents = NOBJ_EVENTS;
  fsm->state = OBJ_INIT;
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

static state_t const _transition[NOBJ_STATES][NOBJ_EVENTS] = {
// OPEN_CB  OBJ_WS  START_ID  END_ID  BAD_ID  COLON   NON_COLON   GOOD_VALUE  OBJ_BV  BAD_TOKEN COMMA CLOSE_CB
  {OBJ_SKIP, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, }, // OBJ_INIT
  {NON_OBJ, OBJ_SKIP, BUILD_ID, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, OBJ_ERROR, NON_OBJ, NON_OBJ,}, // OBJ_SKIP
  {NON_OBJ, NON_OBJ, NON_OBJ, PEND_VALUE, OBJ_ERROR, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,}, // BUILD_ID
  {NON_OBJ, PEND_VALUE, NON_OBJ, NON_OBJ, NON_OBJ, BUILD_VALUE, OBJ_ERROR, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,}, // PEND_VALUE
  {NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, SCANNING, OBJ_ERROR, NON_OBJ, NON_OBJ, NON_OBJ,}, // BUILD_VALUE
  {NON_OBJ, SCANNING, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, OBJ_ERROR, OBJ_SKIP, OBJ_FINISH}, // SCANNING
  {NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,}, // OBJ_FINISH
  {NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,}, // OBJ_ERROR
};

static action_t const _effect[NOBJ_STATES][NOBJ_EVENTS] = {
// OPEN_CB  OBJ_WS  START_ID  END_ID  BAD_ID  COLON   NON_COLON   GOOD_VALUE  OBJ_BV  BAD_TOKEN COMMA CLOSE_CB
  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // OBJ_INIT
  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, SyntaxError, NULL, NULL}, // OBJ_SKIP
  {NULL, NULL, NULL, SetIdent, SyntaxError, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // BUILD_ID
  {NULL, NULL, NULL, NULL, NULL, AdvancePointer, SyntaxError, NULL, NULL, NULL, NULL, NULL}, // PEND_VALUE
  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, AppendKeyValuePair, SyntaxError, NULL, NULL, NULL}, // BUILD_VALUE
  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, SyntaxError, NULL, NULL}, // SCANNING
  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // OBJ_FINISH
  {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // OBJ_ERROR
};

static action_t const _entry[NOBJ_STATES] = {
  NULL, NULL, ActivateString, NULL, ActivateValue, NULL, NULL, NULL
};

/* Given FSM instance and event, perform the table lookups */
static state_t
parse_transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  if (fsm->state >= NON_OBJ || event >= NIL_OBJ || _transition[fsm->state][event] == NON_OBJ) 
    return -1;
  
  *effect = _effect[fsm->state][event];
  state_t next = _transition[fsm->state][event];
  if (next != NON_OBJ)
    *entry = _entry[next];
  
  return next;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */
static void 
SetIdent (fsm_t *fsm)
{
  fsm->key_str = fsm->buffer;
  // This is done in ActivateString, idk how else to do it
}

static void 
AdvancePointer (fsm_t *fsm)
{
  fsm->current++;
}

static void 
ActivateString (fsm_t *fsm)
{
  fsm_t *str_machine = string_init (fsm->current);
  fsm->is_val_ok = accept_string (str_machine, &fsm->buffer);
  if (fsm->is_val_ok)
  {
    fsm->current = str_machine->current;
  }
  else 
  {
    free (str_machine->buffer);
  }
  free (str_machine);
}

static void 
ActivateValue (fsm_t *fsm)
{
  fsm_t *valfsm = value_init (fsm->current);
  bool is_string = false;
  char *str = NULL;
  int64_t integer = 0;
  fsm->is_val_ok = accept_value (valfsm, &is_string, &str, &integer);
  if (fsm->is_val_ok)
  {
    fsm->current = valfsm->current;
    if (fsm->is_val_str) // If val is a string
    {
      fsm->val_str = str;
    } else  // If val is an int
    {
      fsm->val_int = integer;
    }
  } else // Bad value
  {
    printf ("Accept value failed \n");
  }
  free (valfsm);
}

static void 
AppendKeyValuePair (fsm_t *fsm) 
{
  // key comes from fsm key_str
  int ret_val = 100;
  fsm->kvbuffer = (char *) calloc (99, sizeof (char));
  // realloc()
  if (fsm->is_val_str)
  {
    ret_val = snprintf (fsm->kvbuffer, 100, "KEYS[%s] = %s\n", fsm->key_str, fsm->val_str);
  } else 
  {
    ret_val = snprintf (fsm->kvbuffer, 100, "KEYS[%s] = %ld\n", fsm->key_str, fsm->val_int);
  }
  
}
// In AppendKeyValuePair, use a combination of strncat() and
// snprintf() to create format strings like the following:
//   printf ("KEYS[%s] = %" PRId64 "\n", ...
//   printf ("KEYS[%s] = %s\n", ...
// These strings should be concatenated with previous key-value
// pairs and stored somewhere that the accept_object can
// retrieve them later. Note that you can use realloc() to
// resize an existing dynamically
//  allocated string to make space to concatenate.

static void 
SyntaxError (fsm_t *fsm)
{
  fsm->current--;
  char curr = fsm->current[0];
  char err;
  if (fsm->current[1] == '\n')
  {
    err = '-';
  } else 
  {
    err = fsm->current[1];
  }
  printf ("SYNTAX ERROR: Could not process text beginning at '%c%c'\n", curr, err);
}
// For syntax errors, if there is a newline character ('\n'),
// replace it with a null byte ('\0'), then use this format
// string:

// printf ("SYNTAX ERROR: Could not process text beginning at '%s'\n",