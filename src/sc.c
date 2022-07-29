#include "sc.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define OCTET_MAX 4
#define NMATCH_MAX 6

#define CIDR_BUFF_LEN 64
#define OCTET_BUFF_LEN 16

static int ExtractDecimal(const char *str, size_t start, size_t end) {
    char buff[OCTET_BUFF_LEN];
    int len = end - start;

    // fill buffer with octet string
    sprintf(buff, "%.*s", len,  &str[start]);
    buff[len] = '\0';

    // convert to integer
    return atoi(buff);
}

bool CidrCreate(Cidr *cidr, const char *input) {
    // CIDR regex
    regex_t cidrRegex;
    regmatch_t pmatch[NMATCH_MAX];
    size_t nmatch = NMATCH_MAX;
    int regexValue;

    // CIDR
    int octet[OCTET_MAX];
    int maskBits;

    // error message
    const char *octetIndex[] = {
        "first",
        "second",
        "third",
        "fourth"
    };

    regcomp(
        &cidrRegex,
        "^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})/([0-9]{1,2})$",
        REG_EXTENDED
    );

    regexValue = regexec(&cidrRegex, input, nmatch, pmatch, 0);
    regfree(&cidrRegex);

    if (regexValue != 0) {
        // invalid CIDR ip
        fprintf(stderr, "Invalid IP provided %s\n", input);
        return false;
    }

    for (int i = 0; i < OCTET_MAX; ++i) {
        // extract all octets
        octet[i] = ExtractDecimal(input, pmatch[i+1].rm_so, pmatch[i+1].rm_eo);
        if (octet[i] > 255) {
            fprintf(stderr, "The %s octet is invalid (%d)\n", octetIndex[i], octet[i]);
            return false;
        }
    }

    // extract mask
    maskBits = ExtractDecimal(input, pmatch[5].rm_so, pmatch[5].rm_eo);
    if (maskBits > 32) {
        fprintf(stderr, "The mask is invalid (%d)\n", maskBits);
        return false;
    }

    // store ip as integer
    cidr->ip = 0x00;
    cidr->ip |= octet[0] << 24;
    cidr->ip |= octet[1] << 16;
    cidr->ip |= octet[2] << 8;
    cidr->ip |= octet[3];

    // store mask as integer
    cidr->mask = 0xFFFFFFFF;
    cidr->mask = cidr->mask << (32 - maskBits);

    return true;
}

void CidrShowReport(Cidr *cidr) {
    char ipBuff[CIDR_BUFF_LEN], maskBuff[CIDR_BUFF_LEN];
    char cidrBlockBuff[2*CIDR_BUFF_LEN], broadcastBuff[CIDR_BUFF_LEN];
    uint32_t broadcast, maskBits, nHosts;

    maskBits = CidrGetMaskBits(cidr);
    nHosts = CidrGetNumOfHosts(cidr);
    broadcast = CidrGetBroadcast(cidr);

    CidrToStr(ipBuff, cidr->ip);
    CidrToStr(maskBuff, cidr->mask);

    CidrBlockToStr(cidrBlockBuff, cidr);
    CidrToStr(broadcastBuff, broadcast);

    printf("====================================\n");
    printf("|            SUBNET CALC           |\n");
    printf("====================================\n");
    printf("IP:\t\t%20s\n", ipBuff);
    printf("IP Hex:\t\t%20X\n", cidr->ip);
    printf("IP Long:\t%20u\n", cidr->ip);
    printf("CIDR Block:\t%20s\n", cidrBlockBuff);
    printf("Broadcast:\t%20s\n", broadcastBuff);
    printf("------------------------------------\n");
    printf("Mask:\t\t%20s\n", maskBuff);
    printf("Mask Hex:\t%20X\n", cidr->mask);
    printf("Mask Long:\t%20u\n", cidr->mask);
    printf("Mask Bits:\t%20u\n", maskBits);
    printf("Num of Hosts:\t%20u\n", nHosts);
}

uint32_t CidrGetBlock(Cidr *cidr) {
    return cidr->ip & cidr->mask;
}

uint32_t CidrGetBroadcast(Cidr *cidr) {
    return cidr->ip | ~(cidr->mask);
}

uint32_t CidrGetMaskBits(Cidr *cidr) {
    uint32_t count = 0;
    uint32_t m = cidr->mask;
    while (m != 0) {
        m = m << 1;
        ++count;
    }
    return count;
}

uint32_t CidrGetNumOfHosts(Cidr *cidr) {
    return 1 << (32 - CidrGetMaskBits(cidr));
}

void CidrToStr(char *str, uint32_t ip) {
    uint8_t octet[OCTET_MAX];

    octet[0] = ip >> 24;
    octet[1] = ip >> 16;
    octet[2] = ip >> 8;
    octet[3] = ip;

    sprintf(str, "%u.%u.%u.%u", octet[0], octet[1], octet[2], octet[3]);
}

void CidrBlockToStr(char *str, Cidr *cidr) {
    char cidrBuff[CIDR_BUFF_LEN];

    CidrToStr(cidrBuff, CidrGetBlock(cidr));
    sprintf(str, "%s/%u", cidrBuff, CidrGetMaskBits(cidr));
}
