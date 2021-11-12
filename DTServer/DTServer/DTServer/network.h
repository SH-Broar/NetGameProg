#pragma once
#include "protocol.h"

void err_quit(char* msg);

void err_display(char* msg);

int recvn(SOCKET s, char* buf, int len, int flags);
