#ifndef SC_H
#define SC_H

#include <stdbool.h>
#include <stdint.h>

#define IP_BUFF_LEN   16

typedef enum {
    IPV4 = 0,
    IPV6,
    IPV_COUNT
} IpVersion;

typedef struct {
    uint8_t ip[IP_BUFF_LEN];
    IpVersion version;
} Ip;

typedef struct {
    uint8_t ip[IP_BUFF_LEN];
    uint8_t mask[IP_BUFF_LEN];
    IpVersion version;
} Cidr;

bool CidrCreate(Cidr *cidr, const char *input);

void CidrShowReport(Cidr *cidr);

void CidrGetBlock(Cidr *cidr, Ip *ipBlock);

void CidrGetBroadcast(Cidr *cidr, Ip *broadcast);

void CidrGetNumOfHosts(Cidr *cidr, uint8_t *numOfHosts);

uint8_t CidrGetMaskBits(Cidr *cidr);

void IpToStr(char *str, Ip *ip);

void CidrBlockToStr(char *str, Cidr *cidr);


#endif  // SC_H
