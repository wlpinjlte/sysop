#define SIZE 256

typedef enum{
    INIT,
    STOP,
    LIST,
    TO_ONE,
    TO_ALL,
    GET,
    SERVER_FULL,

} Type;

typedef struct{
    Type type;
    char sender[SIZE];
    char reciver[SIZE];
    char content[SIZE];
}Msg;