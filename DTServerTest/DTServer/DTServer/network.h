#pragma once
#include <chrono>
#include "protocol.h"

void err_quit(const char* msg);

void err_display(const char* msg);

int recvn(SOCKET s, char* buf, int len, int flags);
