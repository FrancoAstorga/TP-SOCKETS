#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>


#define PORT 8080

char *generarNombreUsuario(int longitud)
{
    if (longitud < 5 || longitud > 15)
    {
        return NULL; // Validación de longitud
    }

    const char *vocales = "aeiou";
    const char *consonantes = "bcdfghjklmnpqrstvwxyz";
    char *nombre = (char *)malloc(longitud + 1);// Asigna memoria para el nombre de usuario, incluyendo el caracter nulo
    int empiezaConVocal = rand() % 2; // Elegir aleatoriamente entre vocal o consonante

    for (int i = 0; i < longitud; i++)
    {
        if (i % 2 == empiezaConVocal)
        {
            nombre[i] = vocales[rand() % strlen(vocales)];
        }
        else
        {
            nombre[i] = consonantes[rand() % strlen(consonantes)];
        }
    }
    nombre[longitud] = '\0'; // Terminar la cadena
    return nombre;
}

char *generarContrasena(int longitud)
{
    if (longitud < 8 || longitud >= 50)
    {
        return NULL; // Validación de longitud
    }

    const char *caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char *contrasena = (char *)malloc(longitud + 1);

    for (int i = 0; i < longitud; i++)
    {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0'; // Terminar la cadena
    return contrasena;
}

int main()
{
    WSADATA wsa;
    SOCKET servidor, cliente;
    struct sockaddr_in servidorAddr, clienteAddr;
    int clienteAddrLen = sizeof(clienteAddr);
    char buffer[1024];
    int longitud;
    int running=1;
    char *ptr;

    system("cls");

    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Fallo en Winsock. Codigo de error: %d", WSAGetLastError());
        return 1;
    }

    if ((servidor = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("No se pudo crear el socket. Codigo de error: %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = INADDR_ANY;
    servidorAddr.sin_port = htons(PORT);

    if (bind(servidor, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) == SOCKET_ERROR)
    {
        printf("Error al enlazar socket. Codigo de error: %d", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

while (running){

    listen(servidor, 2);
    printf("Esperando conexiones entrantes...\n");
    cliente = accept(servidor, (struct sockaddr *)&clienteAddr, &clienteAddrLen);
    printf("Cliente conectado.\n");

    while (strncmp(buffer, "SALIDA", 6) != 0)
    {  


        // Limpia el buffer estableciendo todos los bytes a 0
        memset(buffer, 0, sizeof(buffer)); 

        // Recibir longitud y comando de cliente
        recv(cliente, buffer, sizeof(buffer), 0);

        //generador de semilla del rand, la cual toma los segundos del time
        srand(time(NULL));


        if (strncmp(buffer, "USUARIO", 7) == 0)
        {
            ptr = strstr(buffer, "USUARIO");

            if (ptr != NULL)
            {
                ptr += strlen("USUARIO"); // Saltar la palabra "USUARIO"
                longitud = atoi(ptr);     // Convertir el número a entero
                free(ptr);
            }

            char *nombre = generarNombreUsuario(longitud);
            if (nombre)
            {
                send(cliente, nombre, strlen(nombre), 0);
                free(nombre);
            }
            else
            {
                
                char *mensajeError = "Longitud de nombre de usuario invalida.";
                send(cliente, mensajeError, strlen(mensajeError), 0);
            }
        }
        else if (strncmp(buffer, "CONTRASENA", 10) == 0)
        {
            ptr = strstr(buffer, "CONTRASENA");

            if (ptr != NULL)
            {
                ptr += strlen("CONTRASENA"); // Saltar la palabra "CONTRASENA"
                longitud = atoi(ptr);     // Convertir el número a entero
                free(ptr);
            }

            char *contrasena = generarContrasena(longitud);
            if (contrasena)
            {
                send(cliente, contrasena, strlen(contrasena), 0);
                free(contrasena);
            }
            else
            {
                char *mensajeError = "Longitud de contrasena invalida.";
                send(cliente, mensajeError, strlen(mensajeError), 0);
            }
        }else{
                char *mensajeError = "Opcion incorrecta";
                send(cliente, mensajeError, strlen(mensajeError), 0);
        }


    }
    
    memset(buffer, 0, sizeof(buffer)); 

    if (cliente == INVALID_SOCKET)
    {
        printf("Fallo en la aceptacion del cliente. Codigo de error: %d", WSAGetLastError());
    }


}


    return 0;
}
