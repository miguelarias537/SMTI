#include "SensorVibracion.h"
#include <iostream>

using namespace std;

SensorVibracion::SensorVibracion(
    int id, double val, const string& ubicacion)
    : Sensor(id, val, ubicacion) {
}

void SensorVibracion::procesarLectura() {
    cout << "\n--- SENSOR DE VIBRACION ---" << endl;
    cout << *this << " mm/s" << endl;

    if (valor > 5.0) {
        cout << "Estado: ALERTA - Vibracion elevada" << endl;
    } else {
        cout << "Estado: NORMAL" << endl;
    }
}
