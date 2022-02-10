#ifndef __parser_h__
#define __parser_h__

#include <stdbool.h>
#include <stdint.h>

#include "statemodel.h"

bool accept_string (fsm_t *, char **);
bool accept_integer (fsm_t *, int64_t *);
bool accept_value (fsm_t *, bool *, char **, int64_t *);
bool accept_object (fsm_t *, char **);

#endif
