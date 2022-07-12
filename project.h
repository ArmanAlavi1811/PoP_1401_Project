#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <unistd.h>

#define elif else if

#define Loop_LT(i, f, n) for(i = f; i < n; i++) //Less than "for" macro
#define Loop_LTE(i, f, n) for(i = f; i <= n; i++) //Less or equal "for" macro

#define Forever while(1)

//editor function modes
#define FILL 1
#define FIND 2
#define EDIT 3
