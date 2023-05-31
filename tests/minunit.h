#ifndef MINUNIT_H
#define MINUNIT_H

#define MU_PASS     return NULL
#define MU_MSG_FMT  "%s | %s::%d -> "
#define MU_MSG_ARGS __FILE__, __FUNCTION__, __LINE__

#define MU_ASSERT(test, message)                                   \
    do {                                                           \
        if (!(test)) {                                             \
            sprintf(g_assertMsg, MU_MSG_FMT message, MU_MSG_ARGS); \
            return g_assertMsg;                                    \
        }                                                          \
    } while (0)

#define MU_ASSERT_FMT(test, msg_format, ...)                                         \
    do {                                                                             \
        if (!(test)) {                                                               \
            sprintf(g_assertMsg, MU_MSG_FMT msg_format, MU_MSG_ARGS, ##__VA_ARGS__); \
            return g_assertMsg;                                                      \
        }                                                                            \
    } while (0)

#define MU_TEST(test)                \
    do {                             \
        char *message = test();      \
        g_testsRun++;                \
        if (message) return message; \
    } while (0)

extern char g_assertMsg[1024];
extern int g_testsRun;

#endif  // MINUNIT_H
