#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "lib.h"

int sock;

int unix_connect(char *path){
    struct sockaddr_un addr;
    memset(&addr,0,sizeof(addr));
    addr.sun_family=AF_UNIX;
    strcpy(addr.sun_path,path);
    int socketToReturn=socekt(AF_UNIX,SOCK_STREAM,0);
    connect(socketToReturn,(struct sockaddr*) &addr,sizeof(addr));
    return socketToReturn;
}

int web_connect(char *ipv4,int port){
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_pton(AF_INET, ipv4, &addr.sin_addr);
    int socketToReturn=socket(AF_INET,SOCK_STREAM,0);
    connect(socketToReturn,(struct sockaddr*) &addr,sizeof(addr));
    return socketToReturn;
}

void signal_handler(int signum){
    Msg *msg=malloc(sizeof(Msg));
    msg->type=STOP;
    write(sock,msg,sizeof(Msg));
    exit(0);
}

int main(int argc,char **argv){
    if(strcmp(argv[2],"unix")==0&&argc==4){
        sock=unix_connect(argv[3]);
    }else if(strcmp(argv[2],"web")==0&&argc==5){
        sock=web_connect(argv[3],atoi(argv[4]));
    }else{
        printf("invalid arguments\n");
        exit(0);
    }

    signal(SIGINT,signal_handler);
    write(sock,argv[1],strlen(argv[1]));

    int epoll_fd=epoll_create1(0);

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLPRI;
    event.data.fd = STDIN_FILENO;

    epoll_ctl(epoll_fd,EPOLL_CTL_ADD, STDIN_FILENO, &event);

    struct epoll_event socket_event;
    socket_event.events = EPOLLIN | EPOLLPRI | EPOLLHUP;
    socket_event.data.fd = sock;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &socket_event);

    struct epoll_event events[2];

    while(1){
        int n=epoll_wait(epoll_fd,events,2,1);
        for(int i=0;i<n;i++){
            if(events[i].data.fd==STDIN_FILENO){
                char buffer[512]={};
                int x=read(STDIN_FILENO);
                buffer[x]=0;

                char *token;
                token=strtok(buffer,"\n");
                Type type=-1;
//                char receiver[SIZE]={};
//                char content[SIZE]={};
                Msg *msg=malloc(sizeof(Msg));

                if(token==NULL){
                    continue;
                }
                if(strcmp(token,"LIST")==0){
                    type=LIST;
                }else if(strcmp(token,"2ALL")==0){
                    type = TO_ALL;
                    token = strtok(NULL, "\n");
//                    memcpy(content, token, strlen(token)*sizeof(char));
//                    content[strlen(token)] = 0;
                    strcpy(msg->content,token);
                }else if(strcmp(token,"2ONE")==0){
                    type=TO_ONE;
                    token=strtok(NULL,' ');
//                    memcpy(receiver,token,strlen(token)*sizeof(char));
//                    reciver[strlen(token)]=0;
                    strcpy(msg->receiver,token);
                    token=strtok(NULL,'\n');
//                    memcpy(content,token,strlen(token)*sizeof(char));
//                    content[strlen(token)]=0;
                    strcpy(msg->content,token);
                }else if(strcmp(token,"STOP")==0){
                    type=STOP;
                }else{
                    puts("invalid command");
                    continue;
                }
                msg->type=type;
                write(sock,msg,sizeof(Msg));
            }else{
                Msg *msg=malloc(sizeof(Msg));
                read(sock,msg,sizeof(Msg));

                if(msg->type==NAME_TAKEN){
                    puts("Name is taken");
                    close(sock);
                    exit(0);
                }else if(msg->type==SERVER_FULL){
                    puts("Server is Full");
                    close(sock);
                    exit(0);
                }else if(msg->type==PING){
                    write(sock,msg,strlen(Msg));
                }else if(msg->type==STOP){
                    close(sock);
                    exit(0);
                }else if(msg->type=GET){
                    puts(msg->content);
                }else if(events[i].events& EPOLLHUP){
                    puts("Disconnect");
                    exit(0);
                }
            }
        }
    }
    return 0;
}