#include "mainClient.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT "5000"

int main(int argc, char *argv[]){

    // ? Initialisation de la structure de l'adresse du serveur
    bzero((char *)&serv_addr,
          sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((uint16_t) atoi(SERVER_PORT));
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // ? Création du socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cliecho : erreur socket");
        exit(1);
    }
    // ? Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("cliecho : erreur connect");
        exit(1);
    }

    // ? Communication avec le serveur
    char buffer[200];
    int n;
    while (1)
    {
        printf("\n\033[0;36mNetworkScanner>>>\033[0;37m");
        fgets(buffer, 200, stdin);
        n = send(sockfd, buffer, strlen(buffer), 0);
        if(strcmp(buffer, "exit\n") == 0){
            break;
        }
        if (n < 0)
        {
            perror("cliecho : erreur send");
            exit(1);
        }
        n = recv(sockfd, buffer, 200, 0);
        if (n < 0)
        {
            perror("cliecho : erreur recv");
            exit(1);
        }
        printf("%s\n", buffer);
    }
    close(sockfd);
    return 0;
}
