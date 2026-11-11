#include "SensorTemperatura.h"
#include <iostream>

using namespace std;

SensorTemperatura::SensorTemperatura(
    int id, double val, const string& ubicacion)
    : Sensor(id, val, ubicacion) {
}

void SensorTemperatura::procesarLectura() {
    cout << "\n--- SENSOR DE TEMPERATURA ---" << endl;
    cout << *this << " grados Celsius" << endl;

    if (valor > 40) {
        cout << "Estado: ALERTA - Temperatura elevada" << endl;
    } else {
        cout << "Estado: NORMAL" << endl;
    }
}
