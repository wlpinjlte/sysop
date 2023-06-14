#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "lib.h"

#define MAX_CLIENTS 10

typedef struct{
    int fd;
    char name[SIZE];
    int active;
} Client;

typedef struct{
    enum Type {socket_event, client_event} type;
    Client *client;
    int socekt;
} Event_data;



Client clients[MAX_CLIENTS];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int epoll_fd;

void delete_client(Client *client){
    client->active=-1;
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,client->fd,NULL);
    close(client->fd);
    printf("Client %s deleted\n",client->name);
}

void socet_init(int sock,void *addr,int len){
    bind(sock,(struct sockaddr*) addr, len);
    listen(sock,MAX_CLIENTS);
    struct epoll_event event;
    event.events=EPOLLIN|EPOLLPRI;
    Event_data *e_data=malloc(sizeof(Event_data));
    e_data->type=socket_event;
    e_data->socekt=sock;
    event.data.ptr=e_data;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock,&event);
}

void *ping(void *arg){
    Msg *msg=malloc(sizeof(Msg));
    msg->type=PING;
    while(1){
        sleep(30);
        pthread_mutex_lock(&mutex);
        printf("ping\n");
        for(int i=0;i<MAX_CLIENTS;i++){
            if(clients[i].active==-1) {
                continue;
            }
            if(clients[i].active==0){
                delete_client(&clients[i]);
            }else{
                clients[i].active=0;
                write(clients[i].fd,msg,sizeof(Msg));
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}

Client *create_client(int fd){
    pthread_mutex_lock(&mutex);
    Client *client=NULL;
    for(int i=0;i<MAX_CLIENTS;i++){
        if(clients[i].active==-1){
            client=&clients[i];
            break;
        }
    }
    if(client==NULL){
        return NULL;
    }
    client->fd=fd;
    client->active=-1;
    pthread_mutex_unlock(&mutex);
    return client;
}

void message_handler(Client *client){
    if(client->active==-1){
        int size_name=read(client->fd,client->name,sizeof(client->name)-1);
        pthread_mutex_lock(&mutex);
        int flag=0;
        for(int i=0;i<MAX_CLIENTS;i++){
            if(clients[i].active!=-1&&strcmp(clients[i].name,client->name)==0){
                flag=1;
                break;
            }
        }
        if(flag) {
            Msg *msg = malloc(sizeof(Msg));
            msg->type = NAME_TAKEN;
            write(client->fd, msg, sizeof(Msg));
            delete_client(client);
        }else{
            client->active=1;
            client->name[size_name]='\0';
            printf("new client %s\n",client->name);
        }
        pthread_mutex_unlock(&mutex);
    }else{
        Msg *msg=malloc(sizeof(Msg));
        read(client->fd,msg,sizeof(Msg));
        if(msg->type==PING){
            pthread_mutex_lock(&mutex);
            printf("%s\n",client->name);
            client->active=1;
            pthread_mutex_unlock(&mutex);
        }else if(msg->type==STOP){
            pthread_mutex_lock(&mutex);
            delete_client(client);
            pthread_mutex_unlock(&mutex);
        }else if(msg->type==TO_ALL){
            char content[SIZE]="";
            strcat(content,client->name);
            strcat(content,": ");
            strcat(content,msg->content);
            for(int i=0;i<MAX_CLIENTS;i++){
                if(clients[i].active!=-1&& &clients[i]!=client){
                    Msg *msg=malloc(sizeof(Msg));
                    msg->type=GET;
                    strcpy(msg->content,content);
                    write(clients[i].fd,msg,sizeof(Msg));
                }
            }
        }else if(msg->type==LIST){
            for(int i=0;i<MAX_CLIENTS;i++){
                if(clients[i].active!=-1){
                    Msg *msg=malloc(sizeof(Msg));
                    msg->type=GET;
                    strcpy(msg->content,clients[i].name);
                    write(client->fd,msg,sizeof(Msg));
                }
            }

        }else if(msg->type==TO_ONE){
            char content[SIZE]="";
            strcat(content,client->name);
            strcat(content,": ");
            strcat(content,msg->content);
            for(int i=0;i<MAX_CLIENTS;i++){
                if(clients[i].active!=-1){
                    if(strcmp(clients[i].name,msg->reciver)==0){
                        Msg *msg=malloc(sizeof(Msg));
                        msg->type=GET;
//                        memcpy(msg->content,content,SIZE*sizeof(char));
                        strcpy(msg->content,content);
                        write(clients[i].fd,msg,sizeof(Msg));
                    }
                }
            }
        }
    }
}

int main(int argc,char **argv){
    if(argc!=3){
        printf("invalid number of arguments\n");
        exit(0);
    }
    for(int i=0;i<MAX_CLIENTS;i++){
        clients[i].active=-1;
    }

    int port = atoi(argv[1]);
    char* socket_path = argv[2];
    epoll_fd = epoll_create1(0);

    struct sockaddr_un local_addr;
    local_addr.sun_family = AF_UNIX;
    strncpy(local_addr.sun_path, socket_path, sizeof(local_addr.sun_path));

    struct sockaddr_in web_addr;
    web_addr.sin_family = AF_INET;
    web_addr.sin_port = htons(port);
    web_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    unlink(socket_path);
    int local_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    socet_init(local_socket, &local_addr, sizeof(local_addr));

    int web_socket = socket(AF_INET, SOCK_STREAM, 0);
    socet_init(web_socket, &web_addr, sizeof(web_addr));

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping, NULL);
    printf("Server started\n");
    struct epoll_event events[10];

    while(1){
        int n=epoll_wait(epoll_fd,events,10,-1);
        for(int i=0;i<n;i++){
            Event_data *data=events[i].data.ptr;
            if(data->type==socket_event){
                int client_fd=accept(data->socekt,NULL,NULL);
                Client *client= create_client(client_fd);
                if(client==NULL){
                    Msg *msg=malloc(sizeof(Msg));
                    msg->type=SERVER_FULL;
                    write(client_fd,msg,sizeof(Msg));
                    close(client_fd);
                    continue;
                }
                Event_data *event_data=malloc(sizeof(Event_data));
                event_data->type=client_event;
                event_data->client=client;
                struct epoll_event event = { .events = EPOLLIN | EPOLLET | EPOLLHUP, .data = { event_data } };
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            }else if(data->type==client_event){
                if(events[i].events&EPOLLHUP){
                    pthread_mutex_lock(&mutex);
                    delete_client(data->client);
                    pthread_mutex_unlock(&mutex);
                }else{
                    message_handler(data->client);
                }
            }
        }
    }
    return 0;
}