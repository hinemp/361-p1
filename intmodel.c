#include <stdbool.h>
#include <stdio.h>

#include "intmodel.h"
#include "statemodel.h"

/* Return an FSM that links to these internals */
fsm_t *
int_init (char const *input)
{
  return NULL;
}


// static parse_t const _transition[NUM_STATES][NUM_EVENTS] = {
//    // OPEN_QUOTE CLOSE_QUOTE NONCTRL BACKSLASH ESC_CHAR NO_ESC NIL_CHAR
//    {BUILDING, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR}, // STR_INIT
//    {NON_STR, STR_FINISH, BUILDING, ESCAPE, NON_STR, NON_STR, NON_STR}, // BUILDING
//    {NON_STR, NON_STR, NON_STR, NON_STR, BUILDING, NON_STR, NON_STR}, // ESCAPE
//    {}, // STR_FINISH
//    {}, // STR_ERROR
//    {}, // NON_STR
// };

/* INTEGER PROCESSING STATES
  INT_INIT,   // beginning to build an integer value
  MAGNITUDE,  // building the magnitude of the value
  SIGN,       // encountered a leading - to make negative
  OCTAL,      // converting to octal format
  INT_FINISH, // successfully built a string
  INT_ERROR,  // encountered an invalid digit
  NON_INT
*/

/* INTEGER PROCESING EVENTS
  ZERO,      // 0 [leading 0 for octal format]
  HYPHEN,    // - [leading - to negate value]
  NZ_DIGIT,  // 1-9 [leading means non-octal]
  DIGIT,     // 0-9 [at least one leading digit before]
  TERM_INT,  // end of number (triggered by , } whitespace or \0
  NON_DIGIT, // invalid character
  NIL_INT
 */

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */
