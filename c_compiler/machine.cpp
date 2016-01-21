/*
 * File:	machine.cpp
 *
 * Description:	This file contains the values of various parameters for the
 *		target machine architecture.
 */

# include "machine.h"

Machine Linux = {".", "", ""};
Machine OS_X = {"", "_", "(%rip)"};

# if defined(__linux__) && defined(__x86_64__)
Machine *target = &Linux;
# elif defined(__APPLE__) && defined(__x86_64__)
Machine *target = &OS_X;
# else
# error Unsupported architecture
# endif
