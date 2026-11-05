#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    const char* server_ip = (argc > 1) ? argv[1] : "172.16.40.1";
    int port = (argc > 2) ? atoi(argv[2]) : 13;

    //создаётся UDP сокет
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // заполняется адресная структура сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    //IPстр переделывается в бинарный формат
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    //для UDP отправляется любой символ
    const char* msg = "ping";
    std::cout << "Sending request to " << server_ip << ":" << port << "..." << std::endl;
    
    if (sendto(sock, msg, strlen(msg), 0, 
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    //принимается ответ
    char buf[256];
    socklen_t addr_len = sizeof(server_addr);
    ssize_t bytes_received = recvfrom(sock, buf, sizeof(buf) - 1, 0, 
                                      (struct sockaddr*)&server_addr, &addr_len);
    
    if (bytes_received < 0) {
        perror("recvfrom");
    } else {
        buf[bytes_received] = '\0';
        std::cout << "Daytime response: " << buf;
    }

    close(sock);
    return 0;
}
