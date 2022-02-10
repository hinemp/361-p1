#include <stdlib.h>

#include "statemodel.h"

bool debug = false;

/* Generic front-end for handling events. Should do nothing more
   than calling the appropriate *_event function based on the type. */
void
handle_event (fsm_t *fsm, event_t event)
{
  /* Confirm the event is valid for the given FSM */
  if (event >= fsm->nevents)
    return;

  /* Use the FSM's lookup tables; if next is -1, the event is not
     Defined for the current state */
  action_t effect = NULL;
  action_t entry = NULL;
  state_t next = fsm->transition (fsm, event, &effect, &entry);
  if (next == -1)
    return;

  /* Perform the effect (if defined) */
  if (effect != NULL)
    effect (fsm);

  /* Perform the new state's entry action (if defined) */
  if (entry != NULL)
    entry (fsm);

  /* Change the state */
  fsm->state = next;
}
