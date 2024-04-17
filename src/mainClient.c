#include "mainClient.h"
#include <pthread.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT "6666"

// ? Variable globale pour le socket
int sockfd;

// ? Fonction exécutée dans le thread de réception
void *receive_messages(void *arg) {
    char buffer[200];
    int n;
    while (1) {
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            break;
        }
        if(strcmp(buffer, "ENDOFSTREAM") == 0){
            printf("\033[0;36mNetworkScanner>>>\033[0;37m");
            fflush(stdout);
        }else{
            printf("%s", buffer);
        }
        
    }
    return NULL;
}

int main(int argc, char *argv[]) {

    // ? Initialisation de la structure de l'adresse du serveur
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((uint16_t) atoi(SERVER_PORT));
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // ? Création du socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cliecho : erreur socket");
        exit(1);
    }
    // ? Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("cliecho : erreur connect");
        exit(1);
    }

    // ? Création du thread de réception
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        perror("cliecho : erreur pthread_create");
        exit(1);
    }

    // ? Communication avec le serveur
    char buffer[200];
    int n;
    while (1) {
        printf("\n\033[0;36mNetworkScanner>>>\033[0;37m");
        fgets(buffer, 200, stdin);
        n = send(sockfd, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }
        if (n < 0) {
            perror("cliecho : erreur send");
            exit(1);
        }
    }

    // ? Fermeture du socket et du thread
    close(sockfd);
    pthread_join(receive_thread, NULL);

    return 0;
}
