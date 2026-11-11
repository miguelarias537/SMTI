
#ifndef SERVIDORSMTI_H
#define SERVIDORSMTI_H

#include <winsock2.h>
#include <windows.h>

#include "ListaGen.h"
#include "EstadoSMTI.h"
#include "SensorTemperatura.h"
#include "SensorHumedad.h"
#include "SensorVibracion.h"

class ServidorSMTI {
private:
	SOCKET servidor;
	SOCKET cliente;
	
	ListaGen<Sensor*> sensores;
	
	EstadoSMTI* estado;
	volatile LONG detenerSolicitado;
	
public:
	ServidorSMTI(EstadoSMTI* estadoCompartido);
	
	void iniciar();
	void recibirDatos();
	void cerrar();
	void detener();
	
	bool debeDetenerse() const {
		return InterlockedCompareExchange(
										  const_cast<volatile LONG*>(&detenerSolicitado),
										  0, 0
										  ) != 0;
	}
};

#endif

