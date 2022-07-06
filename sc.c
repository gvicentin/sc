#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <regex.h>

void printUsageAndExit(const char *binary);
int readCidrNotation(const char *cidr, uint32_t *ip, uint8_t *mask);

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

    printf("IP: %s\n", string);
    for (int i = 1; i < nmatch; ++i) {
        char numStr[10];
        int len = pmatch[i].rm_eo - pmatch[i].rm_so;
        sprintf(numStr, "%.*s", len,  &string[pmatch[i].rm_so]);
        numStr[len] = '\0';
        int num = atoi(numStr);
        printf("%d\n", num);
    }

    return 0;
}

void printUsageAndExit(const char *binary) {
    printf("Usage: %s IP\n", binary);
    printf("Example: %s 10.0.0.0/24\n\n", binary);
    exit(1);
}
