#include "includes.h"

typedef struct MessageInfo{
    char message[100];
    char timeInterval[10];
    char totalTime[10];
}MessageInfo;

typedef struct NetInfo{
    int socket;
    struct sockaddr_in6 addr;
}NetInfo;

typedef struct ThreadInfo{
    MessageInfo messageInfo;
    NetInfo netInfo;
}ThreadInfo;
