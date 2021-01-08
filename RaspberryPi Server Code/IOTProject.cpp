#include <iostream> 
#include <stdio.h> 
#include <string>
#include <cstring>

#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

using namespace std;

#define PORT 8080 

int main() 
{
    int server_fd, new_socket, server_valread;
    int client_fd;
    struct sockaddr_in serv_addr, cli_addr;
    int opt = 1;
    int serv_addrlen = sizeof (serv_addr);
    char buffer[1024] = {0};
    char* handle; 
    
    char arr1[6][10] = {"red", "green", "blue", "on", "brighten", "dark"};
    char arr2[3][10] = {"off", "darken", "bright"};

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(8888);

    if (inet_pton(AF_INET, "192.168.43.134", &cli_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(client_fd, (struct sockaddr *) &cli_addr, sizeof (cli_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &opt, sizeof (opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *) &serv_addr,
            sizeof (serv_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) 
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &serv_addr,
                (socklen_t*) & serv_addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof (buffer));

        server_valread = read(new_socket, buffer, 1024);
        if (server_valread > 0) {
            printf("%s\n", buffer);

            cout << "Write Data : ";

            //send(server_fd, buffer, strlen(buffer), 0);
            
            for (int i = 0; buffer[i]; i++) {
                buffer[i] = tolower(buffer[i]);
            }

            for (int j = 0; j<sizeof (arr1) / sizeof (arr1[0]); j++) 
            {
                handle = strstr(buffer, arr1[j]);

                if ((handle == buffer) || (handle != NULL && !isalnum((unsigned char) handle[-1]))) 
                {
                    handle += strlen(arr1[j]);
                    if (!isalnum((unsigned char) *handle)) {
                        cout << "handle : "<< *handle << endl << "array : "<<  arr1[j] << endl;
                        
                        if(strcmp(arr1[j], "red") == 0)
                         send(client_fd, "red", strlen("red"), 0);
                        else if(strcmp(arr1[j], "green") == 0)
                         send(client_fd, "green", strlen("green"), 0);
                        else if(strcmp(arr1[j], "blue") == 0)
                         send(client_fd, "blue", strlen("blue"), 0);
                        else
                        {   //send(client_fd, arr1[j], strlen(arr1[j]), 0);
                            send(client_fd, "on", strlen("on"), 0);
                            printf("On\n");
                        }
                        break;
                    }
                }
                    
                handle = strstr(buffer,arr2[j]);

                if((handle==buffer) || (handle!= NULL && !isalnum((unsigned char)handle[-1])))
                {
                    handle += strlen(arr2[j]);
                    if(!isalnum((unsigned char)*handle))
                    {
                        //strcpy(buffer,"Off");
                        send(client_fd, "off", strlen("off"), 0);
                        printf("Off\n");
                        break;
                    }
                }
            }
        }
    }

    return 0;
} 
