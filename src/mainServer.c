#include "mainServer.h"
// Port de notre serveur
#define SERV_PORT 5000 // Attention à ne pas choisir un port réservé par notre système (plus que 1024)

int serverSocket;
struct sockaddr_in serv_addr;

int dialogSocket;
int clilen;
struct sockaddr_in cli_addr;

int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int main()
{
    // ! Création de la socket
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("erreur socket");
        exit(1);
    }
    // ! Initialisation de l'adresse du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);
    if (bind(serverSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("servecho: erreur bind\n");
        exit(1);
    }

    // ! Mise en écoute du serveur
    if (listen(serverSocket, SOMAXCONN) < 0)
    {
        perror("servecho: erreur listen\n");
        exit(1);
    }

    // ! Attente de connexion et création de la socket de dialogue
    clilen = sizeof(cli_addr);
    dialogSocket = accept(serverSocket,(struct sockaddr *)&cli_addr,(socklen_t *)&clilen);
    if (dialogSocket < 0)
    {
        perror("servecho : erreur accept\n");
        exit(1);
    }

    // ! Dialogue avec le client
    char buffer[100];
    int n;

    // ? Scanner variables
    int scanType=0; //0: scan horizontal 1: scan vertical 2: scan horizontal et vertical 3: une range de ports est spécifiée
    char* scanTarget;
    // ???? //

    while (1)
    {
        // Réception du message du client
        n = recv(dialogSocket, buffer, 100, 0);
        if (n < 0)
        {
            perror("servecho : erreur recv\n");
            exit(1);
        }
        if (n == 0)
        {
            printf("servecho : fin de la connexion\n");
            close(dialogSocket);
            exit(0);
        }
        buffer[n] = '\0';
        printf("RECEIVED FROM CLIENT: %s\n", buffer);

        // ? interprétation de la commande client

        if(strcmp(buffer,"help\n")==0 || strcmp(buffer,"HELP\n")==0 || strcmp(buffer,"h\n")==0 || strcmp(buffer,"?\n")==0){
            send(dialogSocket, "Available commands:\n- help\n- scan: [0-3](default 0) -target (default localhost)\n- exit\n", 200, 0);
        }
        else if(strcmp(buffer,"exit\n")==0 || strcmp(buffer,"EXIT\n")==0){
            printf("servecho : fin de la connexion\n");
            close(dialogSocket);
            exit(0);
        }
        else if(prefix("scan",buffer)){
            char* token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            if(token!=NULL){
                scanType = atoi(token);
                token = strtok(NULL, " ");
                if(token!=NULL){
                    scanTarget = token;
                }
            }
            if(scanType==0){
                send(dialogSocket, "Horizontal scan\n", 200, 0);
            }
            else if(scanType==1){
                send(dialogSocket, "Vertical scan\n", 200, 0);
            }
            else if(scanType==2){
                send(dialogSocket, "Horizontal and vertical scan\n", 200, 0);
            }
            else if(scanType==3){
                send(dialogSocket, "Range of ports scan\n", 200, 0);
            }
            else{
                send(dialogSocket, "Invalid scan type\n", 200, 0);
            }
        }
        else{
            send(dialogSocket, "Invalid command\n", 200, 0);
        }
    }

}
