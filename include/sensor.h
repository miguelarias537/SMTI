#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <stdexcept>
#include <string>

// Estructura de datos recibida por TCP.
struct MuestraSensor {
    int idSensor;
    double valor;
    char tipo[12];
};

// Clase base abstracta.
class Sensor {
protected:
    int idSensor;
    std::string ubicacion;
    double valor;

public:
    Sensor(int id, double val,
           const std::string& lugar = "No especificada")
        : idSensor(id), ubicacion(lugar), valor(val) {
        if (id <= 0) {
            throw std::runtime_error("ID de sensor no valido");
        }
    }

    // Destructor virtual para liberar correctamente objetos derivados.
    virtual ~Sensor() {}

    // Metodo virtual puro: convierte Sensor en clase abstracta.
    virtual void procesarLectura() = 0;

    int getId() const {
        return idSensor;
    }

    double getValor() const {
        return valor;
    }

    std::string getUbicacion() const {
        return ubicacion;
    }

    // Sobrecarga amiga para imprimir los datos comunes de cualquier sensor.
    friend std::ostream& operator<<(std::ostream& os, const Sensor& s) {
        os << "Sensor ID: " << s.idSensor
           << " | Ubicacion: " << s.ubicacion
           << " | Valor: " << s.valor;
        return os;
    }
};

#endif
