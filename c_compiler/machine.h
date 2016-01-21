/*
 * File:	machine.h
 *
 * Description:	This file contains the values of various parameters for the
 *		target machine architecture.
 */

# ifndef MACHINE_H
# define MACHINE_H
# include <string>

# define SIZEOF_CHAR 1
# define SIZEOF_INT 4
# define SIZEOF_LONG 8
# define SIZEOF_PTR 8

# define SIZEOF_ARG 8
# define NUM_ARGS_IN_REGS 6
# define INIT_ARG_OFFSET 16
# define STACK_ALIGNMENT 16

struct Machine {
    std::string label_prefix;
    std::string global_prefix;
    std::string global_suffix;
};




extern Machine Linux, OS_X, *target;


# endif /* MACHINE_H */
