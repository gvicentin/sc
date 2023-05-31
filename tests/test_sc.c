#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "minunit.h"
#include "sc.c"

char g_assertMsg[1024];
int g_testsRun = 0;

static const char *m_input[] = {"192.168.15.134/24", "172.16.65.23/20",
                                "10.169.23.212/23", "81.215.25.74/18", "127.0.75.88/8"};

static char *testExtractDecimal(void);
static char *testCreateCidr(void);
static char *testInvalidCidr(void);
static char *testGetBlock(void);
static char *testGetBroadcast(void);
static char *allTests(void);

int main(int argc, char **argv) {
    char *result = allTests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", g_testsRun);

    return result != 0;
}

static char *testExtractDecimal(void) {
    int decimal;

    decimal = ExtractDecimal(m_input[0], 0, 3);
    MU_ASSERT_FMT(decimal == 192, "ExtractDecimal(%s, 0, 3), Expected: %d, Got: %d",
                  m_input[0], 192, decimal);

    decimal = ExtractDecimal(m_input[2], 7, 9);
    MU_ASSERT_FMT(decimal == 23, "ExtractDecimal(%s, 7, 9), Expected: %d, Got: %d",
                  m_input[2], 23, decimal);

    decimal = ExtractDecimal(m_input[4], 12, 13);
    MU_ASSERT_FMT(decimal == 8, "ExtractDecimal(%s, 12, 13), Expected: %d, Got: %d",
                  m_input[4], 8, decimal);

    return 0;
}

static char *testCreateCidr(void) {
    Cidr cidr;
    uint32_t expectedIPs[] = {0xC0A80F86, 0xAC104117, 0x0AA917D4, 0x51D7194A,
                              0x7F004B58};
    uint32_t expectedMasks[] = {0xFFFFFF00, 0xFFFFF000, 0xFFFFFE00, 0xFFFFC000,
                                0xFF000000};

    for (int i = 0; i < 5; ++i) {
        CidrCreate(&cidr, m_input[i]);
        // test ip
        MU_ASSERT_FMT(cidr.ip == expectedIPs[i],
                      "CidrCreate(%s) Expected IP: %X, Got: %X", m_input[i],
                      expectedIPs[i], cidr.ip);
        // test mask
        MU_ASSERT_FMT(cidr.mask == expectedMasks[i],
                      "CidrCreateIP(%s). Expect Mask: %X, Got: %X", m_input[i],
                      expectedMasks[i], cidr.mask);
    }

    return 0;
}

static char *testInvalidCidr(void) {
    Cidr cidr;
    bool valid;
    const char *invalidInputs[] = {"192.a.15.201/24", "-81.45.30.18/20",
                                   "172.16.38.255/33"};

    for (int i = 0; i < 3; ++i) {
        valid = CidrCreate(&cidr, invalidInputs[i]);
        MU_ASSERT_FMT(valid == false,
                      "CidrCreateIP(%s) should be invalid but it's valid",
                      invalidInputs[i]);
    }
    return 0;
}

static char *testGetBlock(void) {
    Cidr cidr;
    uint32_t block;
    uint32_t expectedBlocks[] = {0xC0A80F00, 0xAC104000, 0x0AA91600, 0x51D70000,
                                 0x7F000000};

    for (int i = 0; i < 5; ++i) {
        CidrCreate(&cidr, m_input[i]);
        block = CidrGetBlock(&cidr);
        MU_ASSERT_FMT(block == expectedBlocks[i],
                      "CidrGetBlock(%s), Expect: %X, Got %X", m_input[i],
                      expectedBlocks[i], block);
    }

    return 0;
}

static char *testGetBroadcast(void) {
    Cidr cidr;
    uint32_t broadcast;
    uint32_t expectedBroadcasts[] = {0xC0A80FFF, 0xAC104FFF, 0x0AA917FF, 0x51D73FFF,
                                     0x7FFFFFFF};

    for (int i = 0; i < 5; ++i) {
        CidrCreate(&cidr, m_input[i]);
        broadcast = CidrGetBroadcast(&cidr);
        MU_ASSERT_FMT(broadcast == expectedBroadcasts[i],
                      "CidrGetBroadcast(%s), Expect: %X, Got %X", m_input[i],
                      expectedBroadcasts[i], broadcast);
    }
    return 0;
}

static char *allTests() {
    MU_TEST(testExtractDecimal);
    MU_TEST(testCreateCidr);
    MU_TEST(testInvalidCidr);
    MU_TEST(testGetBlock);
    MU_TEST(testGetBroadcast);
    return 0;
}
