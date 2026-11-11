
#ifndef SENSORVIBRACION_H
#define SENSORVIBRACION_H

#include "Sensor.h"

// Clase derivada de Sensor
class SensorVibracion : public Sensor {
public:
	SensorVibracion(int id, double val, const std::string& ubicacion = "No especificada");
	
	void procesarLectura() override;
};

#endif
