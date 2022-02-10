#ifndef __integermodel_h__
#define __integermodel_h__

#include "statemodel.h"

// Integer processing states
typedef enum
{
  INT_INIT,   // beginning to build an integer value
  MAGNITUDE,  // building the magnitude of the value
  SIGN,       // encountered a leading - to make negative
  OCTAL,      // converting to octal format
  INT_FINISH, // successfully built a string
  INT_ERROR,  // encountered an invalid digit
  NON_INT
} intst_t;
#define NINT_STATES NON_INT

// Integer processing events
typedef enum
{
  ZERO,      // 0 [leading 0 for octal format]
  HYPHEN,    // - [leading - to negate value]
  NZ_DIGIT,  // 1-9 [leading means non-octal]
  DIGIT,     // 0-9 [at least one leading digit before]
  TERM_INT,  // end of number (triggered by , } whitespace or \0
  NON_DIGIT, // invalid character
  NIL_INT
} intevt_t;
#define NINT_EVENTS NIL_INT

fsm_t *int_init (char const *);

#endif
