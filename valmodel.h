#ifndef __valmodel_h__
#define __valmodel_h__

#include "statemodel.h"

// Value processing events
typedef enum
{
  START_VALUE, // starting to look for an integer or string value
  WHITESPACE,  // whitespace character to skip over
  START_STR,   // found a beginning ", so build a string
  END_STR,     // string was built successfully
  START_INT,   // found leading - or digit, so build an integer
  END_INT,     // integer was built successfully
  BAD_VALUE,   // an error occurred while building the integer/string
  NIL_VAL
} valevt_t;
#define NVAL_EVENTS NIL_VAL

// Value processing states
typedef enum
{
  VAL_INIT,   // beginning to build a value
  VAL_SKIP,   // skipping over leading whitespace
  BUILD_STR,  // building a string because " was encountered
  BUILD_INT,  // building an integer based on leading - or digit
  VAL_FINISH, // successfully built a value
  VAL_ERROR,  // encountered an error while building
  NON_VAL
} valst_t;
#define NVAL_STATES NON_VAL

fsm_t *value_init (char const *);

#endif
