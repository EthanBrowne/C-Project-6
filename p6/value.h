/**
@file value
@author Ethan Browne, efbrowne
Deals with int, double, and string values
*/

#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

/** Give a short name to the Value struct defined below. */
typedef struct ValueStruct Value;

/** Abstract type used to represent an arbitrary value. All Values
    support four basic operations. */
struct ValueStruct {
  /** Convert the given value to a dynamically allocated string.
      @param v Pointer to the value object to string-ify.
      @return dynamically allocated strign representation for v. */
  char *(*toString)( Value const *v );
  
  /** Subclass-specific behavior performing a += operation, adding
      two different values of the same type.
      @param v Pointer to the value we're modifying (adding to).
      @param x Pointer to the value we're adding to v.
      @return true if the types of v and x permit addition. */
  bool (*plus)( Value *v, Value const *x );
  
  /** Free any memory used to store this value.
      @param v Pointer to the value object to free. */
  void (*destroy)( Value *v );
};

/** Parse the given strign as an integer and create a dynamically allocated
    instance of Value for it.
    @param str string to parse as an integer.
    @return new value or NULL if str can't be parsed as an integer. */
Value *parseInteger( char const *str );

/**
Parse the given string as a double and create a dynamically allocated instance of Value for it.
@param str the string to parse the value from
@return the parsed value
*/
Value *parseDouble( char const *str );

/**
Parse the given string as a string and create a dynamically allocated instance of Value for it.
@param str the string to parse the value from
@return the parsed value
*/
Value *parseString( char const *str );

#endif
