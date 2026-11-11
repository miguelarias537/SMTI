
#include "ServidorSMTI.h"
#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

ServidorSMTI::ServidorSMTI(EstadoSMTI* estadoCompartido) {
	servidor = INVALID_SOCKET;
	cliente = INVALID_SOCKET;
	estado = estadoCompartido;
	detenerSolicitado = 0;
}

void ServidorSMTI::detener() {
	InterlockedExchange(&detenerSolicitado, 1);
}

void ServidorSMTI::iniciar() {
	WSADATA datos;
	
	if (WSAStartup(MAKEWORD(2, 2), &datos) != 0)
		throw runtime_error("Error al iniciar Winsock");
	
	servidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (servidor == INVALID_SOCKET) {
		WSACleanup();
		throw runtime_error("Error al crear socket");
	}
	
	sockaddr_in direccion = {};
	direccion.sin_family = AF_INET;
	direccion.sin_port = htons(8080);
	direccion.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(servidor, (sockaddr*)&direccion,
			 sizeof(direccion)) == SOCKET_ERROR) {
		cerrar();
		throw runtime_error("Error al enlazar el puerto");
	}
	
	if (listen(servidor, 1) == SOCKET_ERROR) {
		cerrar();
		throw runtime_error("Error al escuchar conexiones");
	}
	
	cout << "SERVIDOR SMTI INICIADO" << endl;
	cout << "Puerto TCP: 8080" << endl;
	cout << "Esperando conexion..." << endl;
	
	while (!debeDetenerse()) {
		fd_set conjunto;
		FD_ZERO(&conjunto);
		FD_SET(servidor, &conjunto);
		
		timeval espera;
		espera.tv_sec = 0;
		espera.tv_usec = 500000;
		
		int resultado = select(
							   0, &conjunto, nullptr, nullptr, &espera
							   );
		
		if (resultado == 0)
			continue;
		
		if (resultado == SOCKET_ERROR) {
            cerrar();
            throw runtime_error("Error al esperar cliente");
        }
		
		cliente = accept(servidor, nullptr, nullptr);
		
		if (cliente == INVALID_SOCKET) {
			if (debeDetenerse())
				break;
			
            cerrar();
            throw runtime_error("Error al aceptar cliente");
        }

        estado->establecerConexion(true);

        cout << "Cliente conectado correctamente." << endl;

        try {
            recibirDatos();
        }
        catch (...) {
            estado->establecerConexion(false);
            cerrar();
            throw;
        }

        if (cliente != INVALID_SOCKET) {
			closesocket(cliente);
			cliente = INVALID_SOCKET;
		}
		
		estado->establecerConexion(false);
	}
	
	cerrar();
}

void ServidorSMTI::recibirDatos() {
	MuestraSensor muestra;
	char* destino = reinterpret_cast<char*>(&muestra);
	int recibidos = 0;
	
	DWORD tiempoEspera = 500;
	
	setsockopt(
			   cliente,
			   SOL_SOCKET,
			   SO_RCVTIMEO,
			   (const char*)&tiempoEspera,
			   sizeof(tiempoEspera)
			   );
	
	while (!debeDetenerse()) {
		int n = recv(
					 cliente,
					 destino + recibidos,
					 sizeof(MuestraSensor) - recibidos,
					 0
					 );
		
		if (n == 0) {
			cout << "Cliente desconectado." << endl;
			break;
		}
		
		if (n == SOCKET_ERROR) {
			int error = WSAGetLastError();
			
			if (error == WSAETIMEDOUT ||
				error == WSAEWOULDBLOCK)
				continue;
			
			throw runtime_error("Error al recibir datos");
		}
		
		recibidos += n;
		
		if (recibidos == sizeof(MuestraSensor)) {
			muestra.tipo[sizeof(muestra.tipo) - 1] = '\0';
			
			if (muestra.idSensor <= 0)
				throw runtime_error("ID de sensor invalido");
			
			if (strcmp(muestra.tipo, "TEMP") == 0)
				sensores.agregar(
								 new SensorTemperatura(muestra.idSensor, muestra.valor, "Planta - Temperatura")
								 );
			else if (strcmp(muestra.tipo, "HUMEDAD") == 0)
				sensores.agregar(
								 new SensorHumedad(muestra.idSensor, muestra.valor, "Planta - Humedad")
								 );
			else if (strcmp(muestra.tipo, "VIBRACION") == 0)
				sensores.agregar(
								 new SensorVibracion(muestra.idSensor, muestra.valor, "Planta - Vibracion")
								 );
			else
				throw runtime_error("Tipo de sensor desconocido");
			
			estado->actualizar(
							   muestra.tipo,
							   muestra.idSensor,
							   muestra.valor,
							   sensores.getCantidad()
							   );
			
			cout << "Medicion recibida: "
				<< muestra.tipo << " = "
				<< muestra.valor << endl;
			
			recibidos = 0;
		}
	}
	
	cout << "Procesamiento finalizado. Sensores: "
						  << sensores.getCantidad() << endl;
	
	sensores.procesarTodos();
}

void ServidorSMTI::cerrar() {
	if (estado != nullptr)
		estado->establecerConexion(false);
	
	if (cliente != INVALID_SOCKET) {
		closesocket(cliente);
		cliente = INVALID_SOCKET;
	}
	
	if (servidor != INVALID_SOCKET) {
		closesocket(servidor);
		servidor = INVALID_SOCKET;
	}
	
	WSACleanup();
}
