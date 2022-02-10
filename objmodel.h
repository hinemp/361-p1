#ifndef __objmodel_h__
#define __objmodel_h__

#include "statemodel.h"

// Object processing events
typedef enum
{
  OPEN_CB,    // encountered a leading { to start an object
  OBJ_WS,     // whitespace character to skip over
  START_ID,   // encountered a " to start an identifier
  END_ID,     // identifier string was accepted
  BAD_ID,     // syntax error in the identifier string
  COLON,      // encountered the : between identifier and value
  NON_COLON,  // something that wasn't : where it should be
  GOOD_VALUE, // string or integer value was accepted
  OBJ_BV,     // value was not successfully built
  BAD_TOKEN,  // after value, something other than } or ,
  COMMA,      // encountered , between multiple key-value pairs
  CLOSE_CB,   // encountered } to close the object
  NIL_OBJ
} objevt_t;
#define NOBJ_EVENTS NIL_OBJ

// Object processing states
typedef enum
{
  OBJ_INIT,    // beginning to build a JSON object
  OBJ_SKIP,    // skipping over leading whitespace
  BUILD_ID,    // building the key-value pair identifier string
  PEND_VALUE,  // waiting on the : to start finding the value
  BUILD_VALUE, // building the value of the pair
  SCANNING,    // looking for more pairs or end-of-object } character
  OBJ_FINISH,  // successfully built an object
  OBJ_ERROR,   // encountered one of many errors
  NON_OBJ
} objst_t;
#define NOBJ_STATES NON_OBJ

fsm_t *object_init (char const *);

#endif
