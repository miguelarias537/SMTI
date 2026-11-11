#include <iostream>
#include <exception>
#include "ServidorSMTI.h"

using namespace std;

int main() {
    EstadoSMTI estado;

    try {
        ServidorSMTI servidor(&estado);
        servidor.iniciar();
    }
    catch (const exception& e) {
        cerr << "\nERROR SMTI: " << e.what() << endl;
        return 1;
    }

    cout << "\nServidor finalizado correctamente." << endl;
    return 0;
}
