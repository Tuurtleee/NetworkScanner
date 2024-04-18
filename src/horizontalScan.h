#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <fcntl.h>
#include <sys/select.h>
#include <netinet/ip_icmp.h>

int horizontalScan(char *scanTarget, char *scanMask, char** ips, int*ipscount);