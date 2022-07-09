#ifndef SC_H
#define SC_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t ip;
    uint32_t mask;
} Cidr;

bool CidrCreate(Cidr *cidr, const char *input);
void CidrShowReport(Cidr *cidr);

uint32_t CidrGetBlock(Cidr *cidr);
uint32_t CidrGetBroadcast(Cidr *cidr);
uint32_t CidrGetMaskBits(Cidr *cidr);
uint32_t CidrGetNumOfHosts(Cidr *cidr);

void CidrToStr(char *str, uint32_t ip);
void CidrBlockToStr(char *str, Cidr *cidr);

#endif // SC_H
