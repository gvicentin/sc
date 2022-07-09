#include <stdio.h>
#include <stdlib.h>
#include "sc.h"

void printUsageAndExit(const char *binary);

int main(int argc, char *argv[]) {
    Cidr cidr = {0};

    if (argc < 2 || !CidrCreate(&cidr, argv[1])) {
        printUsageAndExit(argv[0]);
    }

    CidrShowReport(&cidr);

    return 0;
}

void printUsageAndExit(const char *binary) {
    printf("Usage: %s IP\n", binary);
    printf("Example: %s 10.0.0.0/24\n\n", binary);
    exit(1);
}

