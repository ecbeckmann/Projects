/*
 * File:	generator.h
 *
 * Description:	This file contains the function declarations for the code
 *		generator for Simple C.  Most of the function declarations
 *		are actually member functions provided as part of Tree.h.
 */

# ifndef GENERATOR_H
# define GENERATOR_H
# include "Tree.h"
# include <map>




void generateGlobals(const Symbols &globals);

class Label{
  public:
  unsigned value;
  static unsigned counter;
  Label();
};
typedef std::map<std::string, Label> Strings;


extern Strings strings;

# endif /* GENERATOR_H */
