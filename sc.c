#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <regex.h>

#define OCTET_BUFF_LEN 16

void printUsageAndExit(const char *binary);

uint8_t octetFromStr(const char *str, size_t start, size_t end);

int main(int argc, char *argv[]) {

    regex_t reegex;
    size_t nmatch = 6;
    regmatch_t pmatch[6];
    int value;

    if (argc < 2) {
        printUsageAndExit(argv[0]);
    }

    const char *string = argv[1];
    const char *pattern = "^([0-9]{1,3})\\.([0-9]{1,3})\\."
                          "([0-9]{1,3})\\.([0-9]{1,3})/([0-9]{1,2})$";
    regcomp(&reegex, pattern, REG_EXTENDED);

    value = regexec(&reegex, string, nmatch, pmatch, 0);
    if (value != 0) {
        printf("Invalid IP provided %s\n", string);
        printUsageAndExit(argv[0]);
    }

    printf("Input: %s\n", string);

    uint8_t fOctet, sOctet, tOctet, foOctet;
    fOctet = octetFromStr(string, pmatch[1].rm_so, pmatch[1].rm_eo);
    sOctet = octetFromStr(string, pmatch[2].rm_so, pmatch[2].rm_eo);
    tOctet = octetFromStr(string, pmatch[3].rm_so, pmatch[3].rm_eo);
    foOctet = octetFromStr(string, pmatch[4].rm_so, pmatch[4].rm_eo);

    printf("Fist octet: %u\n", fOctet);
    printf("Second octet: %u\n", sOctet);
    printf("Third octet: %u\n", tOctet);
    printf("Fourth octet: %u\n", foOctet);

    uint32_t ip = 0x00;
    ip |= fOctet << 24;
    ip |= sOctet << 16;
    ip |= tOctet << 8;
    ip |= foOctet;

    printf("IP Long: %u\n", ip);

    // de-initialization
    regfree(&reegex);

    return 0;
}

void printUsageAndExit(const char *binary) {
    printf("Usage: %s IP\n", binary);
    printf("Example: %s 10.0.0.0/24\n\n", binary);
    exit(1);
}

uint8_t octetFromStr(const char *str, size_t start, size_t end) {
    char buff[OCTET_BUFF_LEN];
    int len = end - start;

    // fill buffer with octet string
    sprintf(buff, "%.*s", len,  &str[start]);
    buff[len] = '\0';

    // convert to integer
    return (uint8_t) atoi(buff);
}
