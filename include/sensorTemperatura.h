
#ifndef SENSORTEMPERATURA_H
#define SENSORTEMPERATURA_H

#include "Sensor.h"

class SensorTemperatura : public Sensor {
public:
	SensorTemperatura(int id, double val, const std::string& ubicacion = "No especificada");
	
	void procesarLectura() override;
};

#endif

