#ifndef __stringmodel_h__
#define __stringmodel_h__

#include "statemodel.h"

// String processing states
typedef enum
{
  STR_INIT,   // beginning to build a string
  BUILDING,   // appending characters
  ESCAPE,     // encountered a backslash
  STR_FINISH, // successfully built a string
  STR_ERROR,  // encountered an invalid escape code
  NON_STR
} strst_t;
#define NSTR_STATES NON_STR

// String processing events
typedef enum
{
  OPEN_QUOTE,  // triggered by " at the beginning of a string
  CLOSE_QUOTE, // read a " character at the end
  NON_CTRL,    // read a non-" or \ character
  BACKSLASH,   // read a \ character
  ESC_CHAR,    // allowable escape character (\" \\ \n \t \r)
  NO_ESC,      // non-allowable escape
  NIL_CHAR
} strevt_t;
#define NSTR_EVENTS NIL_CHAR

fsm_t *string_init (char const *);

#endif
