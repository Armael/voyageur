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

#define mu_INIT() int tests_run; \
  int tests_failed

#define mu_assert(test, message) do { if (!(test)) return message; } while (0)

#define START_TEST(__testname) \
  static char* __testname() { \

#define END_TEST return NULL; }

#define mu_run_test(__test) do { char *__message = __test(); tests_run++; \
    if (__message) {							\
      printf("%s[Failed]%s Testing \'%s\' (n°%d) : %s\n",		\
	     ANSI_BRED, ANSI_NORMAL, #__test, tests_run,		\
	     __message);						\
      tests_failed++;							\
    } else {								\
      printf("%s[OK]%s Testing \'%s\' (n°%d)\n",			\
	     ANSI_BGREEN, ANSI_NORMAL, #__test, tests_run);		\
    }									\
  } while (0)

#define mu_summary() do {						\
    if(!tests_failed) {							\
      printf("%s[OK]%s All (%d) tests passed !\n",			\
	     ANSI_BGREEN, ANSI_NORMAL, tests_run);			\
    } else {								\
      printf("%s[Fail]%s %d test%s failed.\n",				\
	     ANSI_BRED, ANSI_NORMAL, tests_failed,			\
	     (tests_failed>1)?"s":"");					\
    }									\
  } while (0)

#endif
