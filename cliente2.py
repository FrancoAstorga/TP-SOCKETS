import socket
import os 



SERVER = "127.0.0.1"  # Dirección IP del servidor (localhost)
PORT = 8080           # Puerto en el que el servidor está escuchando

def main():
    os.system("cls")
    # Crear un socket TCP/IP
    cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Conectarse al servidor
        cliente.connect((SERVER, PORT))
        print(f"Conectado al servidor {SERVER} en el puerto {PORT}.")

        while True:
            print("\nElige una opcion:\n1. Generar nombre de usuario\n2. Generar contraseña\n3. Salir")
            opcion = input("> ")

            if opcion == '3':
                cliente.sendall(b"SALIDA")
                break

            if opcion == '1' or opcion == '2':
                longitud = input("Indica la longitud deseada: ")

            if opcion == '1':
                mensaje = f"USUARIO {longitud}"
            elif opcion == '2':
                mensaje = f"CONTRASENA {longitud}"
            else:
                print("Opción no válida.")
                input("Presiona Enter para continuar...")
                continue

            # Enviar solicitud al servidor en bytes
            cliente.sendall(mensaje.encode())

            # Recibir respuesta del servidor hasta 1024 bytes
            respuesta = cliente.recv(1024)
            if not respuesta:
                print("Error al recibir la respuesta.")
            else:
                # Bytes a string
                print(f"Respuesta del servidor: {respuesta.decode()}")


    except Exception as e:
        print(f"Se produjo un error: {e}")
    finally:
        cliente.close()
        print("Conexión cerrada.")

if __name__ == "__main__":
    main()
