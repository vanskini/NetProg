#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    //проверяется количество аргументов
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        return 1;
    }

    //создается TCP сокет
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { 
        perror("socket"); 
        return 1; 
    }

    //заполняется адресная структура сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2])); // берется порт из аргументов
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) { // берется IP из аргументов
        perror("inet_pton");
        close(sock);
        return 1;
    }

    //устанавливается соединение с сервером
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    std::cout << "Connected. Enter messages (empty line to quit):\n";
    std::string line;
    
    //цикл обмена данными с сервером
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;

        //отправляются данные на сервер
        ssize_t sent = send(sock, line.c_str(), line.length(), 0);
        if (sent < 0) { 
            perror("send"); 
            break; 
        }

        //принимается эхо от сервера
        char buf[1024];
        ssize_t n = recv(sock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) { 
            std::cout << "Server closed.\n"; 
            break; 
        }
        buf[n] = '\0';
        std::cout << "Echo: " << buf << std::endl;
    }

    //закрывается сокет
    close(sock);
    return 0;
}
