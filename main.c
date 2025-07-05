#define  _CRT_SECURE_NO_WARNINGS // Supress deppreciated warning

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <direct.h>

#pragma comment(lib, "ws2_32.lib")


#define PORT 8080
#define BUFFER_SIZE 8192

const char* get_content_type(const char* path){
    const char* ext = strrchr(path, '.');

    if(!ext) return "text/plain";
    if(strcmp(ext, ".html") == 0) return "text/html";
    if(strcmp(ext, ".css") == 0) return "text/css";
    if(strcmp(ext, ".js") == 0) return "application/javascript";
    if(strcmp(ext, ".png") == 0) return "image/png";
    if(strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if(strcmp(ext, ".txt") == 0) return "text/plain";
    return "application/octel-stream";
}

void serve_file(SOCKET client_socket, const char* path){
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "public%s", strcmp(path, "/") == 0 ? "/index.html" : path);

    FILE* file = fopen(full_path, "rb");
    if(!file){
        const char* not_found =
        "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n""<h1>404 Not Found</h1>";
        send(client_socket, not_found, strlen(not_found), 0);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* file_data = malloc(file_size);
    fread(file_data, 1, file_size, file);
    fclose(file);

    char header[256];
    snprintf(
        header,
        sizeof(header),
        "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",
        get_content_type(full_path),
        file_size
    );

    send(client_socket, header, strlen(header), 0);
    send(client_socket, file_data, file_size, 0);

    free(file_data);
    printf("[+] Served: %s\n", full_path);
}

int main(){
    struct WSAData wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len;
    char buffer[BUFFER_SIZE];

    WSAStartup(MAKEWORD(2, 2), &wsa);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR){
        printf("[-] Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        return 1;
    }

    listen(server_socket, 5);
    printf("[*] Serving on http://localhost:%d\n", PORT);

    while(1){
        client_len = sizeof(client);
        client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);

        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if(bytes <= 0){
            closesocket(client_socket);
            continue;
        }

        buffer[bytes] = '\0';
        printf("[>] Request: \n%s\n", buffer);

        char method[8], path[256];
        sscanf(buffer, "%s %s", method, path);

        if(strcmp(method, "GET") == 0){
            serve_file(client_socket, path);
        }
        else{
            const char* bad_req =
            "Allowed\r\nContent-Type: text/plain\r\n\r\nOnly GET is supported.";
            send(client_socket, bad_req, strlen(bad_req), 0);
        }

        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;

}
