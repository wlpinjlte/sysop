#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<signal.h>

#define HOME getenv("HOME")
#define MAX_NO_CLIENTS 8
struct msgbuff {
    long mtype;         /* typ komunikatu   */
    char mtext[1024];      /* tresc komunikatu */
    key_t clientKey;
    struct tm timeinfo;
    int auxiliaryId;
};