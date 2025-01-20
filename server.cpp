#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define PORT 9000

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Escuchar por conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    cout << ">> Servei iniciat. Escoltant al port " << PORT << "..." << endl;

    while (true) {
        // Aceptar una conexión entrante
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        cout << ">> Connexió acceptada des del client " << inet_ntoa(address.sin_addr) << endl;

        // Obtener la hora y la fecha actual
        time_t t = time(0);  // obtener la hora actual
        struct tm *now = localtime(&t);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", now);

        // Enviar la hora y fecha al cliente
        send(new_socket, buffer, strlen(buffer), 0);
        cout << ">> Hora i data enviades: " << buffer << endl;

        close(new_socket);
    }

    return 0;
}
