#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "minunit.h"
#include "sc.c"

#define MSG_LEN 512

int g_testsRun = 0;

static char m_errorMsg[MSG_LEN];
static const char *m_input[] = {
    "192.168.15.134/24",
    "172.16.65.23/20",
    "10.169.23.212/23",
    "81.215.25.74/18",
    "127.0.75.88/8"
};

static char * testExtractDecimal(void);
static char * testCreateCidr(void);
static char * testInvalidCidr(void);
static char * testGetBlock(void);
static char * testGetBroadcast(void);
static char * allTests(void);

int main(int argc, char **argv) {
    char *result = allTests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", g_testsRun);

    return result != 0;
}

static char * testExtractDecimal(void) {
    int decimal;

    decimal = ExtractDecimal(m_input[0], 0, 3);
    snprintf(
        m_errorMsg,
        MSG_LEN,
        "ExtractDecimal(%s, 0, 3), Expected: %d, Got: %d",
        m_input[0], 192, decimal);
    mu_assert(m_errorMsg, decimal == 192);

    decimal = ExtractDecimal(m_input[2], 7, 9);
    snprintf(
        m_errorMsg,
        MSG_LEN,
        "ExtractDecimal(%s, 7, 9), Expected: %d, Got: %d",
        m_input[2], 23, decimal);
    mu_assert(m_errorMsg, decimal == 23);

    decimal = ExtractDecimal(m_input[4], 12, 13);
    snprintf(
        m_errorMsg,
        MSG_LEN,
        "ExtractDecimal(%s, 12, 13), Expected: %d, Got: %d",
        m_input[4], 8, decimal);
    mu_assert(m_errorMsg, decimal == 8);

    return 0;
}

static char * testCreateCidr(void) {
    Cidr cidr;

    uint32_t expectedIPs[] = {
        0xC0A80F86,
        0xAC104117,
        0x0AA917D4,
        0x51D7194A,
        0x7F004B58
    };
    uint32_t expectedMasks[] = {
        0xFFFFFF00,
        0xFFFFF000,
        0xFFFFFE00,
        0xFFFFC000,
        0xFF000000
    };

    for (int i = 0; i < 5; ++i) {
        CidrCreate(&cidr, m_input[i]);

        // test ip
        snprintf(
            m_errorMsg,
            MSG_LEN,
            "CidrCreate(%s) Expected IP: %X, Got: %X",
            m_input[i], expectedIPs[i], cidr.ip
        );
        mu_assert(m_errorMsg, cidr.ip == expectedIPs[i]);

        // test mask
        snprintf(
            m_errorMsg,
            MSG_LEN,
            "CidrCreateIP(%s). Expect Mask: %X, Got: %X",
            m_input[i], expectedMasks[i], cidr.mask
        );
        mu_assert(m_errorMsg, cidr.mask == expectedMasks[i]);
    }

    return 0;
}

static char * testInvalidCidr(void) {
    Cidr cidr;
    bool valid;

    const char *invalidInputs[] = {
        "192.a.15.201/24",
        "-81.45.30.18/20",
        "172.16.38.255/33"
    };

    for (int i = 0; i < 3; ++i) {
        valid = CidrCreate(&cidr, invalidInputs[i]);
        snprintf(
            m_errorMsg,
            MSG_LEN,
            "CidrCreateIP(%s) is valid",
            invalidInputs[i]
        );
        mu_assert(m_errorMsg, valid == false);
    }
    return 0;
}

static char * testGetBlock(void) {
    Cidr cidr;
    uint32_t block;
    uint32_t expectedBlocks[] = {
        0xC0A80F00,
        0xAC104000,
        0x0AA91600,
        0x51D70000,
        0x7F000000
    };

    for (int i = 0; i < 5; ++i) {
        CidrCreate(&cidr, m_input[i]);
        block = CidrGetBlock(&cidr);
        snprintf(
            m_errorMsg,
            MSG_LEN,
            "CidrGetBlock(%s), Expect: %X, Got %X",
            m_input[i], expectedBlocks[i], block
        );
        mu_assert(m_errorMsg, block == expectedBlocks[i]);
    }

    return 0;
}

static char * testGetBroadcast(void) {
    Cidr cidr;
    uint32_t broadcast;
    uint32_t expectedBroadcasts[] = {
        0xC0A80FFF,
        0xAC104FFF,
        0x0AA917FF,
        0x51D73FFF,
        0x7FFFFFFF
    };

    for (int i = 0; i < 5; ++i) {
        CidrCreate(&cidr, m_input[i]);
        broadcast = CidrGetBroadcast(&cidr);
        snprintf(
            m_errorMsg,
            MSG_LEN,
            "CidrGetBroadcast(%s), Expect: %X, Got %X",
            m_input[i], expectedBroadcasts[i], broadcast
        );
        mu_assert(m_errorMsg, broadcast== expectedBroadcasts[i]);
    }
    return 0;
}

static char * allTests() {
    mu_run_test(testExtractDecimal);
    mu_run_test(testCreateCidr);
    mu_run_test(testInvalidCidr);
    mu_run_test(testGetBlock);
    mu_run_test(testGetBroadcast);
    return 0;
}
