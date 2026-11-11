
#ifndef ESTADOSMTI_H
#define ESTADOSMTI_H

#include <windows.h>
#include <cstring>

struct DatosPanel {
	double temperatura;
	double humedad;
	double vibracion;
	
	int idTemperatura;
	int idHumedad;
	int idVibracion;
	
	int cantidadSensores;
	bool conectado;
};

struct RegistroMedicion {
	int idSensor;
	double valor;
	char tipo[12];
};

class EstadoSMTI {
private:
	CRITICAL_SECTION bloqueo;
	DatosPanel datos;
	
	RegistroMedicion historial[100];
	int cantidadHistorial;
	
public:
	EstadoSMTI() {
		InitializeCriticalSection(&bloqueo);
		
		datos.temperatura = 0;
		datos.humedad = 0;
		datos.vibracion = 0;
		
		datos.idTemperatura = 0;
		datos.idHumedad = 0;
		datos.idVibracion = 0;
		
		datos.cantidadSensores = 0;
		datos.conectado = false;
		
		cantidadHistorial = 0;
	}
	
	void actualizar(const char* tipo, int id,
					double valor, int cantidad) {
		
		EnterCriticalSection(&bloqueo);
		
		if (strcmp(tipo, "TEMP") == 0) {
			datos.temperatura = valor;
			datos.idTemperatura = id;
		}
		else if (strcmp(tipo, "HUMEDAD") == 0) {
			datos.humedad = valor;
			datos.idHumedad = id;
		}
		else if (strcmp(tipo, "VIBRACION") == 0) {
			datos.vibracion = valor;
			datos.idVibracion = id;
		}
		
		datos.cantidadSensores = cantidad;
		
		// Guardar la medicion en el historial.
		if (cantidadHistorial == 100) {
			for (int i = 1; i < 100; i++) {
				historial[i - 1] = historial[i];
			}
			cantidadHistorial = 99;
		}
		
		RegistroMedicion& registro =
			historial[cantidadHistorial];
		
		registro.idSensor = id;
		registro.valor = valor;
		
		strncpy(registro.tipo, tipo,
				sizeof(registro.tipo) - 1);
		
		registro.tipo[sizeof(registro.tipo) - 1] = '\0';
		
		cantidadHistorial++;
		
		LeaveCriticalSection(&bloqueo);
	}
					
					void establecerConexion(bool estado) {
						EnterCriticalSection(&bloqueo);
						datos.conectado = estado;
						LeaveCriticalSection(&bloqueo);
					}
					
					DatosPanel obtenerDatos() {
						EnterCriticalSection(&bloqueo);
						DatosPanel copia = datos;
						LeaveCriticalSection(&bloqueo);
						return copia;
					}
					
					int obtenerHistorial(RegistroMedicion destino[],
										 int maximo) {
						EnterCriticalSection(&bloqueo);
						
						int total = cantidadHistorial;
						int inicio = total - maximo;
						
						if (inicio < 0)
							inicio = 0;
						
						int copiados = 0;
						
						for (int i = inicio; i < total; i++) {
							destino[copiados] = historial[i];
							copiados++;
						}
						
						LeaveCriticalSection(&bloqueo);
						return copiados;
					}
										 
										 ~EstadoSMTI() {
											 DeleteCriticalSection(&bloqueo);
										 }
										 
										 EstadoSMTI(const EstadoSMTI&) = delete;
										 EstadoSMTI& operator=(const EstadoSMTI&) = delete;
};

#endif
