#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int port = 7777;

    // 1. создается TCP сокет
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return 1;
    }

    // не надо ждать закрытия порта
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. заполняется адресную структуру
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // слушаются все интерфейсы
    addr.sin_port = htons(port);

    // 3. привязывается сокет к адресу
    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_sock);
        return 1;
    }

    // 4. ставится в режим прослушивания
    if (listen(server_sock, 5) < 0) {
        perror("listen");
        close(server_sock);
        return 1;
    }

    std::cout << "Echo TCP server is listening on port " << port << "..." << std::endl;
    std::cout << "Test it with: nc 127.0.0.1 " << port << std::endl;

    // 5. бесконечный цикл обработки соединений
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        // ожидание клиента
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // 6. читаются данные от клиента и отправляются обратно
        char buf[1024];
        ssize_t n;
        while ((n = recv(client_sock, buf, sizeof(buf), 0)) > 0) {
            send(client_sock, buf, n, 0); 
        }

        std::cout << "Client disconnected." << std::endl;
        close(client_sock); // закрывается рабочий сокет
    }

    close(server_sock); // Основной сокет
    return 0;
}
