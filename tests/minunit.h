#ifndef MINUNIT_H_INCLUDED
#define MINUNIT_H_INCLUDED

#include <stdio.h>
#include <unistd.h>

extern int tests_run;
extern int tests_failed;

#define ANSI_GREEN  (isatty(1)?"\033[0;32m":"")
#define ANSI_RED    (isatty(1)?"\033[0;31m":"")
#define ANSI_BGREEN (isatty(1)?"\033[1;32m":"")
#define ANSI_BRED   (isatty(1)?"\033[1;31m":"")
#define ANSI_NORMAL (isatty(1)?"\033[0m":"")

#define mu_assert(test, message) do { if (!(test)) return message; } while (0)

#define START_TEST(__testname) \
  static char* __testname() { \

#define END_TEST return NULL; }

#define mu_run_test(__test) do { char *__message = __test(); tests_run++; \
    if (__message) {							\
      printf("%s[Failed]%s Test \'%s\' (n°%d) : %s%s\n",		\
	     ANSI_BRED, ANSI_RED, #__test, tests_run,			\
	     __message, ANSI_NORMAL);					\
      tests_failed++;							\
    } else {								\
      printf("%s[OK]%s Test \'%s\' (n°%d) %s\n",			\
	     ANSI_BGREEN, ANSI_GREEN, #__test, tests_run,		\
	     ANSI_NORMAL);						\
    }									\
  } while (0)

#define mu_summary() do {						\
    if(!tests_failed) {							\
      printf("%s[OK]%s All (%d) tests passed !%s\n",			\
	     ANSI_BGREEN, ANSI_GREEN, tests_run, ANSI_NORMAL);		\
    } else {								\
      printf("%s[Fail]%s %d test%s failed.%s\n",			\
	     ANSI_BRED, ANSI_RED, tests_failed,				\
	     (tests_failed>1)?"s":"", ANSI_NORMAL);			\
    }									\
  } while (0)

#endif
