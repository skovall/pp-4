#include <iostream>
#include <string>
#include <atomic>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

std::atomic<int> request_counter{0};

std::string handle_request(const std::string& request) {
    if (request.find("GET / ") != std::string::npos || 
        request.find("GET / HTTP") != std::string::npos) {
        request_counter++;
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from CI/CD #" 
               + std::to_string(request_counter.load());
    }
    if (request.find("GET /health") != std::string::npos) {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nOK";
    }
    if (request.find("GET /metrics") != std::string::npos) {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nhttp_requests_total " 
               + std::to_string(request_counter.load());
    }
    return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    
    std::cout << "Server listening on port 8080" << std::endl;
    
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, 1024);
        std::string response = handle_request(buffer);
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
        memset(buffer, 0, sizeof(buffer));
    }
    return 0;
}
