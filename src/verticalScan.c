#include "verticalScan.h"

int *verticalScan(char *scanTarget, int *openPorts)
{
    if (scanTarget[strlen(scanTarget) - 1] == '\n')
    {
        scanTarget[strlen(scanTarget) - 1] = '\0';
    }
    printf("Target: %s\n", scanTarget);
    struct sockaddr_in adresse;
    int sockfd;
    char buffer[200];
    int i;
    for (i = 0; i < 65536; i++)
    {
        printf("%d ", i);
        fflush(stdout);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("socket");
            return openPorts;
        }
        // Mettre le socket en mode non bloquant
        fcntl(sockfd, F_SETFL, O_NONBLOCK);
        memset(&adresse, 0, sizeof(adresse));
        adresse.sin_family = AF_INET;
        adresse.sin_port = htons(i);
        if (inet_pton(AF_INET, scanTarget, &adresse.sin_addr) <= 0)
        {
            printf("|%s|\n", scanTarget);
            perror("inet_pton");
            close(sockfd);
            return openPorts;
        }
        connect(sockfd, (struct sockaddr *)&adresse, sizeof(adresse));

        // Utiliser select pour attendre la connexion ou le délai d'attente
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        struct timeval timeout;
        timeout.tv_sec = 0; 
        timeout.tv_usec = 500 * 1000; 

        if (select(sockfd + 1, NULL, &set, NULL, &timeout) > 0)
        {
            int so_error;
            socklen_t len = sizeof so_error;

            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if (so_error == 0)
            {
                printf("Port %d ouvert\n", i);
                openPorts[i] = 1;
            }
        }

        close(sockfd);
    }
    printf("Vertical scan done\n");
    return openPorts;
}
