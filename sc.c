#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <regex.h>

#define OCTET_MAX 4
#define NMATCH_MAX 6

#define CIDR_BUFF_LEN 64
#define OCTET_BUFF_LEN 16

typedef struct {
    uint32_t ip;
    uint32_t mask;
} Cidr;

void printUsageAndExit(const char *binary);
int extractDecimal(const char *str, size_t start, size_t end);
bool createCidr(Cidr *cidr,const char *input);

int main(int argc, char *argv[]) {
    Cidr cidr = {0};

    if (argc < 2 || !createCidr(&cidr, argv[1])) {
        printUsageAndExit(argv[0]);
    }

    printf("IP Long: %u\n", cidr.ip);
    printf("Mask Long: %u\n", cidr.mask);


    return 0;
}

void printUsageAndExit(const char *binary) {
    printf("Usage: %s IP\n", binary);
    printf("Example: %s 10.0.0.0/24\n\n", binary);
    exit(1);
}

int extractDecimal(const char *str, size_t start, size_t end) {
    char buff[OCTET_BUFF_LEN];
    int len = end - start;

    // fill buffer with octet string
    sprintf(buff, "%.*s", len,  &str[start]);
    buff[len] = '\0';

    // convert to integer
    return atoi(buff);
}

bool createCidr(Cidr *cidr, const char *input) {
    // CIDR regex
    regex_t cidrRegex;
    regmatch_t pmatch[NMATCH_MAX];
    size_t nmatch = NMATCH_MAX;
    int regexValue;

    // CIDR
    int octet[OCTET_MAX];
    int mask;

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
        printf("Invalid IP provided %s\n", input);
        return false;
    }

    for (int i = 0; i < OCTET_MAX; ++i) {
        // extract all octets
        octet[i] = extractDecimal(input, pmatch[i+1].rm_so, pmatch[i+1].rm_eo);
        if (octet[i] > 255) {
            printf("The %s octet is invalid (%d)\n", octetIndex[i], octet[i]);
            return false;
        }
    }

    // extract mask
    mask = extractDecimal(input, pmatch[5].rm_so, pmatch[5].rm_eo);
    if (mask > 32) {
        printf("The mask is invalid (%d)\n", mask);
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
    cidr->mask = cidr->mask << (32 - mask);

    return true;
}
