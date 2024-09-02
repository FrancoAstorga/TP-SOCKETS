#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#define SERVER "127.0.0.1"  // Dirección IP del servidor (localhost)
#define PORT 8080           // Puerto en el que el servidor está escuchando

int main() {
    WSADATA wsa;
    SOCKET cliente;
    struct sockaddr_in servidorAddr;
    char mensaje[1024], respuesta[1024];
    int longitud,opcion,tamRespuesta;

    system("cls");
    printf("Inicializando Winsock...\n");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Fallo en Winsock. Codigo de error: %d\n", WSAGetLastError());
        return 1;
    }

    if ((cliente = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket. Codigo de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = inet_addr(SERVER);
    servidorAddr.sin_port = htons(PORT);

    // Conectarse al servidor
    if (connect(cliente, (struct sockaddr*)&servidorAddr, sizeof(servidorAddr)) < 0) {
        printf("Conexion fallida. Codigo de error: %d\n", WSAGetLastError());
        closesocket(cliente);
        WSACleanup();
        return 1;
    }

    printf("Conectado al servidor %s en el puerto %d.\n", SERVER, PORT);

    // Menu 
    while (1) {

        printf("\nElige una opcion:\n1. Generar nombre de usuario\n2. Generar contrasenia\n3. Salir\n> ");

        scanf("%d", &opcion);
        getchar();  // Limpiar el buffer de entrada

        if (opcion == 3) {
  

            if (send(cliente, "SALIDA", strlen("SALIDA"), 0) < 0) {
                printf("Error al enviar el mensaje.\n");
                continue;
            }

            break;
        }

        if (opcion == 1 || opcion == 2){
            printf("Indica la longitud deseada: ");
            scanf("%d", &longitud);
            getchar();  // Limpiar el buffer de entrada
        }

        //Formateamos el mensaje y lo guardamos en el buffer
        if (opcion == 1) {
            sprintf(mensaje, "USUARIO %d", longitud);
        } else if (opcion == 2) {
            sprintf(mensaje, "CONTRASENA %d", longitud);
        }else{
            sprintf(mensaje,"OPCION INVALIDA");
        }

        // Enviar solicitud al servidor
        if (send(cliente, mensaje, strlen(mensaje), 0) < 0) {
            printf("Error al enviar el mensaje.\n");
            continue;
        }

        // Recibir respuesta del servidor
        tamRespuesta = recv(cliente, respuesta, sizeof(respuesta), 0);

        if (tamRespuesta == SOCKET_ERROR) {
            printf("Error al recibir la respuesta.\n");
        } else {
            respuesta[tamRespuesta] = '\0'; //Limpiamos basura
            printf("\nRespuesta del servidor: %s\n", respuesta);
        }
        
    }

    closesocket(cliente);
    WSACleanup();
    return 0;
}
