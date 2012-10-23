#ifndef MINUNIT_H_INCLUDED
#define MINUNIT_H_INCLUDED

#include <stdio.h>
#include <unistd.h>

extern int __tests_run;
extern int __tests_failed;
extern char __errbuf[];

#define ANSI_GREEN  (isatty(1)?"\033[0;32m":"")
#define ANSI_RED    (isatty(1)?"\033[0;31m":"")
#define ANSI_BGREEN (isatty(1)?"\033[1;32m":"")
#define ANSI_BRED   (isatty(1)?"\033[1;31m":"")
#define ANSI_NORMAL (isatty(1)?"\033[0m":"")

#define ERRBUF_LEN 200

#define mu_INIT() int __tests_run;		\
  int __tests_failed;				\
  char __errbuf[ERRBUF_LEN]


#define mu_assert(test, message, ...) do {			\
  if (!(test)) {						\
    snprintf(__errbuf, ERRBUF_LEN, message, ##__VA_ARGS__);	\
    __errbuf[ERRBUF_LEN-1] = '\0';				\
    return __errbuf;						\
  }								\
} while (0)

#define START_TEST(testname)			\
  static char* __test_##testname() {		\
  
#define END_TEST return NULL; }

#define mu_run_test(test) do { char *__message = __test_##test(); __tests_run++; \
    if (__message) {							\
      printf("%s[Failed]%s Testing \'%s\' (n°%d) : %s\n",		\
	     ANSI_BRED, ANSI_NORMAL, #test, __tests_run,		\
	     __message);						\
      __tests_failed++;							\
    } else {								\
      printf("%s[OK]%s Testing \'%s\' (n°%d)\n",			\
	     ANSI_BGREEN, ANSI_NORMAL, #test, __tests_run);		\
    }									\
  } while (0)

#define mu_summary() do {						\
    if(!__tests_failed) {						\
      printf("%s[OK]%s All (%d) tests passed !\n",			\
	     ANSI_BGREEN, ANSI_NORMAL, __tests_run);			\
    } else {								\
      printf("%s[Fail]%s %d test%s failed.\n",				\
	     ANSI_BRED, ANSI_NORMAL, __tests_failed,			\
	     (__tests_failed>1)?"s":"");				\
    }									\
  } while (0)

#endif
