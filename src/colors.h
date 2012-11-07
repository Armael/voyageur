#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED

#include <unistd.h>

#define ANSI_BLACK  (isatty(1)?"\033[0;03m":"")
#define ANSI_RED    (isatty(1)?"\033[0;31m":"")
#define ANSI_GREEN  (isatty(1)?"\033[0;32m":"")
#define ANSI_YELLOW (isatty(1)?"\033[0;33m":"")
#define ANSI_BLUE   (isatty(1)?"\033[0;34m":"")
#define ANSI_PURPLE (isatty(1)?"\033[0;35m":"")
#define ANSI_CYAN   (isatty(1)?"\033[0;36m":"")
#define ANSI_WHITE  (isatty(1)?"\033[0;37m":"")

#define ANSI_BBLACK  (isatty(1)?"\033[1;03m":"")
#define ANSI_BRED    (isatty(1)?"\033[1;31m":"")
#define ANSI_BGREEN  (isatty(1)?"\033[1;32m":"")
#define ANSI_BYELLOW (isatty(1)?"\033[1;33m":"")
#define ANSI_BBLUE   (isatty(1)?"\033[1;34m":"")
#define ANSI_BPURPLE (isatty(1)?"\033[1;35m":"")
#define ANSI_BCYAN   (isatty(1)?"\033[1;36m":"")
#define ANSI_BWHITE  (isatty(1)?"\033[1;37m":"")

#define ANSI_NORMAL (isatty(1)?"\033[0m":"")

#endif
