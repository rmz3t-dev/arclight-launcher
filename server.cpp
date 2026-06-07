#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <string.h>
#include "cJSON/cJSON.h"

#define HTTP_TEXT "HTTP/1.1 200 OK\r\nContent-Type: plain/text\r\n\r\n"

int opt = 1, serverStatus = 0, onlineMode = 0, geyserMC = 0;

void make_config(char *config) {
    cJSON *json = cJSON_CreateObject();
    int json_files = open(config, O_WRONLY | O_CREAT | O_TRUNC, 0777);

    cJSON_AddStringToObject(json, "serverStatus", "disable");
    cJSON_AddStringToObject(json, "onlineMode", "disable");
    cJSON_AddStringToObject(json, "geyserMC", "disable");

    char *json_string = cJSON_Print(json);

    write(json_files, json_string, strlen(json_string));
    free(json_string);
}

char *get_value_config(char buff[], char *key) {
    return cJSON_GetObjectItemCaseSensitive(cJSON_Parse(buff), key)->valuestring;
}

bool compare_str(char *str1, char* str2) {
    if (strcmp(str1, str2) == 0)
        return true;
    return false;
}

void make_af_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = { AF_INET, htons(9898), 0 };
    addr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(sockfd, (struct sockaddr *)&addr, sizeof(sockaddr));
    listen(sockfd, 0);

    while (true) {
        int request = accept(sockfd, NULL, NULL);

        if (request > 0) {
            int request_tmp = request;
            char config[3000];

            mkdir("./.cache", 0777);
            snprintf(config, sizeof(config), "./.cache/config-%d", request_tmp);
                
            make_config(config);
            sendfile(request, open("./index.html", O_RDONLY), 0, 5000);

            while (recv(request_tmp, NULL, 1, MSG_PEEK | MSG_DONTWAIT) != 0) {
                char readed_config[3000];
                int config_file = open(config, O_RDWR | O_CREAT, 0777);
    
                read(config_file, readed_config, sizeof(readed_config));
                sleep(1);
            }

            request = 0;
        }
    }
}

int main() {
    make_af_socket();
}