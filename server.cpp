#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <string.h>

int opt = 1;

int make_af_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { AF_INET, htons(9898), 0 };
    addr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(sockfd, (struct sockaddr *)&addr, sizeof(sockaddr));
    listen(sockfd, 0);

    while (true) {
        int request = accept(sockfd, NULL, NULL);
        char buff[1024];

        if (request > 0) {
            sendfile(request, open("./index.html", O_RDONLY), 0, 4000);

            while (true) {
                request = accept(sockfd, NULL, NULL);
                read(request, buff, sizeof(buff));
                printf("%s\n", buff);
            }
        }
    }
}

int main() {
    make_af_socket();
}