#include <stdbool.h>
#include <stdlib.h>

#include "statemodel.h"
#include "stringmodel.h"

/* Begins at fsm->current and tries to identify a string in the given
   input data. A string begins and ends with ". In between, only two
   special escape sequences are allowed: \\ and \". If the string is
   successfully built, the result pointer will be changed to point to
   a dynamically allocated copy and return true. Return false if any
   error occurs. The escape sequences in the input will be replaced with
   the appropriate character (e.g., \\ in the input becomes \ in the
   result string). (HINT: Note that the effect functions do not have
   access to local variables here; instead, you should extend the fsm_t
   declaration to include helper variables, such as a buffer to copy the
   input bytes into.) */
bool
accept_string (fsm_t *fsm, char **result)
{
  // Find the first quotation mark
  while (fsm->current[0] != '"') 
  {
    fsm->current++;
  }
  handle_event (fsm, OPEN_QUOTE);
  fsm->current++; // Puts the current pointer on first character of the string
  while (fsm->state < STR_FINISH)   // As long as the fsm hasn't reached an end state
  {
    if (fsm->state == BUILDING)
      {
        // Only options are ", /, or 
        if (fsm->current[0] == '"')
          handle_event (fsm, CLOSE_QUOTE);
        else if (fsm->current[0] == '\\')
          handle_event (fsm, BACKSLASH);
        else 
          handle_event (fsm, NON_CTRL);
      }
    else if (fsm->state == ESCAPE)
      {
        switch (fsm->current[0])
        {
        case '"':
        case '\\':
          handle_event (fsm, ESC_CHAR);
          break;
        default:
          handle_event (fsm, NO_ESC);
          break;
        }
      }
  }
  return fsm->state == STR_FINISH;
}

/* Begins at fsm->current and tries to build a valid integer value. This
   function should accept both positive and negative numbers, and should
   allow leading 0s to indicate that the number is in octal format (e.g.,
   023 should print as the decimal value 19). Whitespace, the } character,
   the , character, and the null byte '\0' can be used to indicate the end
   of a number. Any other non-digit value should be considered a NON_DIGIT
   and result in a syntax error. If the number was successfully built, it
   should be copied into the location pointed to by the call-by-reference
   parameter value and the function should return true. Otherwise, return
   false. */
bool
accept_integer (fsm_t *fsm, int64_t *value)
{
  return false;
}

/* Begins at fsm->current and tries to build a value that can be either
   a string or an integer. Skip over leading whitespace until either a
   leading ", -, or digit is encountered. The " indicates the value will
   be a string, whereas the others indicate an integer. Other characters
   should cause a BAD_VALUE event, triggering a syntax error message.
   Returns true only if a valid string or integer value was accepted.
   If a value was successully parsed, sets the is_string, string, and
   value call-by-reference parameters as appropriate. */
bool
accept_value (fsm_t *fsm, bool *is_string, char **string, int64_t *value)
{
  return false;
}

/* Begins at fsm->current and tries to build a valid JSON object. All JSON
   objects consist of key-value pairs, where the key is a string and the
   value can be either a string or integer. If more than one key-value pair
   is in the object, they are separated by commas. The following examples
   are all valid (note that whitespace is ignored):

     {"a":1}
     { "first": "second", "alpha": "beta"}
     { "integer": 1, "string": "one" }

   These are examples of bad objects (and the corresponding error events):

     { not : "good" }  -- BAD_TOKEN
     { "no \t allowed" : } -- BAD_ID
     { "alpha" } -- NON_COLON
     { "float" : 1.5 } -- BAD_VALUE
     { "a" : "b" : "c" } -- BAD_TOKEN
     { "a" : "b", } -- BAD_TOKEN

   All key-value pairs for an object will be concatenated as a string
   and returned via the keys call-by-reference parameter. The format
   should look exactly as (ignore the leading space):

     KEYS[integer] = 1
     KEYS[string] = one

   AppendKeyValuePair should append a new line (ending in '\n') for
   each key-value pair found. In the case above, the exact string
   returned would be:

     "KEYS[integer] = 1\nKEYS[string] = one\n"

   Return true if the object is successfully parsed, false otherwise.
   */
bool
accept_object (fsm_t *fsm, char **keys)
{
  return false;
}
