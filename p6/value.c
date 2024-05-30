/**
@file value
@author Ethan Browne, efbrowne
Deals with int, double, and string values
*/


#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/** Buffer Size*/
#define BUFFER_SIZE 2

/** Type used to represent a subclass of Value that holds an integer. */
typedef struct {
  // Superclass fields.
  char *(*toString)( Value const *v );
  bool (*plus)( Value *v, Value const *x );
  void (*destroy)( Value *v );

  // Subclass fields.
  int val;
} IntegerValue;

/** Maximum length of a 32-bit integer as a string. */
#define INTEGER_LENGTH 11

// toString method for integers
static char *integerToString( Value const *v )
{
  // Get v as a pointer to the subclass struct.
  IntegerValue *this = (IntegerValue *) v;

  // Convert to a dynamically allocated string.
  char *str = (char *) malloc( INTEGER_LENGTH + 1 );
  sprintf( str, "%d", this->val );
  return str;
}

// Plus method for integers.
static bool integerPlus( Value *v, Value const *x )
{
  // We can use a function pointer to make sure x is also an
  // integrer.
  if ( x->toString != integerToString )
    return false;
  
  // Get the parameters as IntegerValue poitners.
  IntegerValue *this = (IntegerValue *) v;
  IntegerValue *that = (IntegerValue *) x;

  // Add the value in x to v.
  this->val += that->val;
  return true;
}

// destroy method for integers
static void integerDestroy( Value *v )
{
  // All the memory for an integer is in one big block.
  free( v );
}

Value *parseInteger( char const *str )
{
  // Try to parse an integer from str.  The buffer is to make sure
  // there's no extra, non-space characters after the integer value.
  int ival;
  char buffer[ BUFFER_SIZE ];

  if ( sscanf( str, "%d%1s", &ival, buffer ) != 1 )
    return NULL;

  // Make a new instance of an integer value.
  IntegerValue *v = (IntegerValue *) malloc( sizeof( IntegerValue ) );
  v->toString = integerToString;
  v->plus = integerPlus;
  v->destroy = integerDestroy;
  v->val = ival;

  // Return as a pointer to the superclass.
  return (Value *)v;
}








/** Type used to represent a subclass of Value that holds an double. */
typedef struct {
  // Superclass fields.
  char *(*toString)( Value const *v );
  bool (*plus)( Value *v, Value const *x );
  void (*destroy)( Value *v );

  // Subclass fields.
  double val;
} DoubleValue;


/** This is the maximum number of characters I could get from a double value,
    printed with %f. */
#define DOUBLE_LENGTH 317

// toString method for doubles
/**
Convert val field into a C string
@param v the value to get the toString of
@return the String representation of the DoubleValue
*/
static char *doubleToString( Value const *v )
{
  // Get v as a pointer to the subclass struct.
  DoubleValue *this = (DoubleValue *) v;

  // Convert to a dynamically allocated string.
  char *str = (char *) malloc( DOUBLE_LENGTH + 1 );
  sprintf( str, "%f", this->val );
  return str;
}

// Plus method for doubles.
/**
Function to add another DoubleValue to this value
@param v the value to add to
@param x the value to add
@return if the values are the same type
*/
static bool doublePlus( Value *v, Value const *x )
{
  // We can use a function pointer to make sure x is also an
  // double.
  if ( x->toString != doubleToString )
    return false;
  
  // Get the parameters as DoubleValue poitners.
  DoubleValue *this = (DoubleValue *) v;
  DoubleValue *that = (DoubleValue *) x;

  // Add the value in x to v.
  this->val += that->val;
  return true;
}

// destroy method for doubles
/**
Frees all memmory for DoubleValue
@param v the value to free
*/
static void doubleDestroy( Value *v )
{
  // All the memory for an double is in one big block.
  free( v );
}

/**
Parse the given string as a double and create a dynamically allocated instance of Value for it.
@param str the string to parse the value from
@return the parsed value
*/
Value *parseDouble( char const *str )
{
  // Try to parse an double from str.  The buffer is to make sure
  // there's no extra, non-space characters after the double value.
  double dval;
  char buffer[ BUFFER_SIZE ];

  if ( sscanf( str, "%lf%1s", &dval, buffer ) != 1 )
    return NULL;

  // Make a new instance of an integer value.
  DoubleValue *v = (DoubleValue *) malloc( sizeof( DoubleValue ) );
  v->toString = doubleToString;
  v->plus = doublePlus;
  v->destroy = doubleDestroy;
  v->val = dval;

  // Return as a pointer to the superclass.
  return (Value *)v;
}







/** Type used to represent a subclass of Value that holds an string. */
typedef struct {
  // Superclass fields.
  char *(*toString)( Value const *v );
  bool (*plus)( Value *v, Value const *x );
  void (*destroy)( Value *v );

  // Subclass fields.
  char *val;
} StringValue;

// toString method for string
/**
Convert val field into a C string
@param v the value to get the toString of
@return the String representation of the StringValue
*/
static char *stringToString( Value const *v )
{
  // Get v as a pointer to the subclass struct.
  StringValue *this = (StringValue *) v;

  // Convert to a dynamically allocated string.
  char *str = (char *) malloc( strlen(this->val) + 1 );
  sprintf( str, "%s", this->val );
  return str;
}

// Plus method for string.
/**
Function to add another StringValue to this value
@param v the value to add to
@param x the value to add
@return if the values are the same type
*/
static bool stringPlus( Value *v, Value const *x )
{
  // We can use a function pointer to make sure x is also an
  // string.
  if ( x->toString != stringToString )
    return false;
  
  // Get the parameters as DoubleValue poitners.
  StringValue *this = (StringValue *) v;
  StringValue *that = (StringValue *) x;

  // Add the value in x to v.
  this->val[strlen(this->val) - 1] = '\0';
  this->val = (char *)realloc( this->val, (strlen(this->val) + strlen(that->val) + 1) * sizeof( char ) );
  strcat(this->val, &(that->val[1]));
  return true;
}

// destroy method for doubles
/**
Frees all memmory for StringValue
@param v the value to free
*/
static void stringDestroy( Value *v )
{
  free(((StringValue *) v)->val);
  free( v );
}

/**
Parse the given string as a string and create a dynamically allocated instance of Value for it.
@param str the string to parse the value from
@return the parsed value
*/
Value *parseString( char const *str )
{
  // Try to parse an str from str.  The buffer is to make sure
  // there's no extra, non-space characters after the str value.
  char* sval = (char *) malloc( (strlen(str) + 1) * sizeof(char) );
  int i = 0;
  while(str[i] != '\"' && i < strlen(str) - 1) {
    i++;
  }
  strcpy(sval, &(str[i]));
  sval[strlen(sval)] = ' ';

  i = strlen(str) - i;
  while (i >= 0 && sval[i] != '\"') {
    sval[i] = '\0';
    i--;
  }

  sval[strlen(sval)] = '\0';
  if (sval[0] != '\"' || sval[strlen(sval) - 1] != '\"') {
    return NULL;
  }
  // Make a new instance of an integer value.
  StringValue *v = (StringValue *) malloc( sizeof( StringValue ) );
  v->toString = stringToString;
  v->plus = stringPlus;
  v->destroy = stringDestroy;
  v->val = sval;

  // Return as a pointer to the superclass.
  return (Value *)v;
}
