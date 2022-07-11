#ifndef MINUNIT_H
#define MINUNIT_H

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); g_testsRun++; \
                                if (message) return message; } while (0)
extern int g_testsRun;

# endif // MINUNIT_H
