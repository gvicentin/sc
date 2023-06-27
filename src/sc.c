#include "sc.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OCTET_MAX  4
#define NMATCH_MAX 6

#define CIDR_BUFF_LEN  64
#define OCTET_BUFF_LEN 16

static int ExtractDecimal(const char *str, size_t start, size_t end) {
    char buff[OCTET_BUFF_LEN];
    int len = end - start;

    // fill buffer with octet string
    sprintf(buff, "%.*s", len, &str[start]);
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
    const char *octetIndex[] = {"first", "second", "third", "fourth"};

    regcomp(&cidrRegex,
            "^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})/"
            "([0-9]{1,2})$",
            REG_EXTENDED);

    regexValue = regexec(&cidrRegex, input, nmatch, pmatch, 0);
    regfree(&cidrRegex);

    if (regexValue != 0) {
        // invalid CIDR ip
        fprintf(stderr, "Invalid IP provided %s\n", input);
        return false;
    }

    for (int i = 0; i < OCTET_MAX; ++i) {
        // extract all octets
        octet[i] = ExtractDecimal(input, pmatch[i + 1].rm_so, pmatch[i + 1].rm_eo);
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

    memset(cidr->ip, 0, sizeof(uint8_t) * IP_BUFF_LEN);
    cidr->ip[0] = octet[3];
    cidr->ip[1] = octet[2];
    cidr->ip[2] = octet[1];
    cidr->ip[3] = octet[0];

    memset(cidr->mask, 0xFF, sizeof(uint8_t) * IP_BUFF_LEN);
    // cidr->mask = cidr->mask << (32 - maskBits);

    return true;
}

void CidrShowReport(Cidr *cidr) {
    char ipBuff[CIDR_BUFF_LEN], maskBuff[CIDR_BUFF_LEN];
    char cidrBlockBuff[2 * CIDR_BUFF_LEN], broadcastBuff[CIDR_BUFF_LEN];
    uint32_t broadcast, maskBits, nHosts;

    maskBits = CidrGetMaskBits(cidr);
    nHosts = CidrGetNumOfHosts(cidr);
    broadcast = CidrGetBroadcast(cidr);

    IpToStr(ipBuff, cidr->ip);
    IpToStr(maskBuff, cidr->mask);

    CidrBlockToStr(cidrBlockBuff, cidr);
    IpToStr(broadcastBuff, broadcast);

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

void CidrGetBlock(Cidr *cidr, Ip *ipBlock) {
    for (uint8_t i = 0; i < IP_BUFF_LEN; ++i) {
        ipBlock->ip[i] = cidr->ip[i] & cidr->mask[i];
    }
}

void CidrGetBroadcast(Cidr *cidr, Ip *broadcast) {
    for (uint8_t i = 0; i < IP_BUFF_LEN; ++i) {
        broadcast->ip[i] = cidr->ip[i] | ~(cidr->mask[i]);
    }
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

void IpToStr(char *str, Ip *ip) {
    sprintf(str, "%u.%u.%u.%u", ip->ip[0], ip->ip[1], ip->ip[2], ip->ip[3]);
}

void CidrBlockToStr(char *str, Cidr *cidr) {
    Ip ip;
    char cidrBuff[CIDR_BUFF_LEN];
    CidrGetBlock(cidr, &ip);
    IpToStr(cidrBuff, &ip);
    sprintf(str, "%s/%u", cidrBuff, CidrGetMaskBits(cidr));
}
