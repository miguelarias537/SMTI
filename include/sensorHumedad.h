
#ifndef SENSORHUMEDAD_H
#define SENSORHUMEDAD_H

#include "Sensor.h"


class SensorHumedad : public Sensor {
public:
	SensorHumedad(int id, double val, const std::string& ubicacion = "No especificada");
	

	void procesarLectura() override;
};

#endif
