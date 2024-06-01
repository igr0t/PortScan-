// * Criado Por Igr0t * //

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

// Função para verificar se uma porta está aberta
bool isPortOpen(const std::string& ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erro ao criar socket\n";
        return false;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    int result = connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr));
    close(sockfd);

    return result == 0;
}

// Função para resolver o nome do host a partir de um endereço IP
std::string resolveHostname(const std::string& ip) {
    struct sockaddr_in sa;
    socklen_t len;
    char hbuf[NI_MAXHOST];

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    len = sizeof(sa);

    if (getnameinfo((struct sockaddr*)&sa, len, hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD)) {
        return ip;  // Se falhar na resolução, retorna o próprio IP
    }
    else {
        return std::string(hbuf);
    }
}

// Função principal de varredura
void portScan(const std::string& ip, const std::vector<int>& ports) {
    std::string hostname = resolveHostname(ip);
    std::cout << "Iniciando varredura em: " << hostname << " (" << ip << ")\n";

    for (int port : ports) {
        if (isPortOpen(ip, port)) {
            std::cout << "Porta " << port << " está aberta\n";
            // Aqui você poderia adicionar a identificação do serviço, mas isso é mais complexo
            // e geralmente envolve enviar e receber dados específicos do protocolo
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <IP>\n";
        return 1;
    }

    std::string ip = argv[1];
    std::vector<int> ports = { 21, 22, 23, 25, 80, 110, 143, 443, 3306 }; // Lista de portas comuns para varredura

    portScan(ip, ports);

    return 0;
}
