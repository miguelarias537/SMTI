#include "SensorHumedad.h"
#include <iostream>

using namespace std;

SensorHumedad::SensorHumedad(
    int id, double val, const string& ubicacion)
    : Sensor(id, val, ubicacion) {
}

void SensorHumedad::procesarLectura() {
    cout << "\n--- SENSOR DE HUMEDAD ---" << endl;
    cout << *this << " %" << endl;

    if (valor > 70) {
        cout << "Estado: ALERTA - Humedad elevada" << endl;
    } else {
        cout << "Estado: NORMAL" << endl;
    }
}
