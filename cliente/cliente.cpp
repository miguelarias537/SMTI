
#include <iostream>
#include <winsock2.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

struct MuestraSensor {
	int idSensor;
	double valor;
	char tipo[12];
};

int main() {
	
	// Inicializar generador aleatorio
	srand((unsigned int)time(NULL));
	
	// Generar mediciones aleatorias
	MuestraSensor temperatura = {
		1,
			20.0 + (rand() % 2001) / 100.0,
			"TEMP"
	};
	
	MuestraSensor humedad = {
		2,
			40.0 + (rand() % 4001) / 100.0,
			"HUMEDAD"
	};
	
	MuestraSensor vibracion = {
		3,
			1.0 + (rand() % 901) / 100.0,
			"VIBRACION"
	};
	
	cout << "==================================" << endl;
	cout << "       CLIENTE SIMULADOR SMTI      " << endl;
	cout << "==================================" << endl;
	
	cout << "\nMEDICIONES GENERADAS:" << endl;
	
	cout << "Temperatura: "
		<< temperatura.valor << " C" << endl;
	
	cout << "Humedad: "
		<< humedad.valor << " %" << endl;
	
	cout << "Vibracion: "
		<< vibracion.valor << " mm/s" << endl;
	
	// Inicializar Winsock
	WSADATA datos;
	
	if (WSAStartup(MAKEWORD(2, 2), &datos) != 0) {
		cout << "\nERROR: No se pudo iniciar Winsock." << endl;
		return 1;
	}
	
	// Crear socket TCP
	SOCKET cliente = socket(
							AF_INET,
							SOCK_STREAM,
							IPPROTO_TCP
							);
	
	if (cliente == INVALID_SOCKET) {
		cout << "\nERROR: No se pudo crear el socket." << endl;
		WSACleanup();
		return 1;
	}
	
	// Configurar direccion del servidor
	sockaddr_in servidor = {};
	
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(8080);
	servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	cout << "\nConectando con el servidor SMTI..." << endl;
	
	// Conectar con el servidor
	if (connect(
				cliente,
				(sockaddr*)&servidor,
				sizeof(servidor)
				) == SOCKET_ERROR) {
		
		cout << "ERROR: No se pudo conectar al servidor." << endl;
		
		closesocket(cliente);
		WSACleanup();
		
		return 1;
	}
	
	cout << "Conexion establecida correctamente." << endl;
	
	// Enviar temperatura
	send(cliente,
		 (char*)&temperatura,
		 sizeof(temperatura),
		 0);
	
	// Enviar humedad
	send(cliente,
		 (char*)&humedad,
		 sizeof(humedad),
		 0);
	
	// Enviar vibracion
	send(cliente,
		 (char*)&vibracion,
		 sizeof(vibracion),
		 0);
	
	cout << "\nLas tres mediciones fueron enviadas." << endl;
	
	// Finalizar envio
	shutdown(cliente, SD_SEND);
	
	closesocket(cliente);
	WSACleanup();
	
	cout << "\nCliente finalizado correctamente." << endl;
	
	return 0;
}
