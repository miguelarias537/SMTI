#ifndef LISTAGEN_H
#define LISTAGEN_H

#include <iostream>
using namespace std;

template <typename T>
class Nodo {
public:
	T dato;
	Nodo<T>* siguiente;
	
	Nodo(T valor) {
		dato = valor;
		siguiente = nullptr;
	}
};

template <typename T>
class ListaGen {
private:
	Nodo<T>* primero;
	int cantidad;
	
public:
	ListaGen() {
		primero = nullptr;
		cantidad = 0;
	}
	
	void agregar(T dato) {
		Nodo<T>* nuevo = new Nodo<T>(dato);
		
		if (primero == nullptr) {
			primero = nuevo;
		} else {
			Nodo<T>* actual = primero;
			
			while (actual->siguiente != nullptr) {
				actual = actual->siguiente;
			}
			
			actual->siguiente = nuevo;
		}
		
		cantidad++;
	}
	
	void procesarTodos() {
		Nodo<T>* actual = primero;
		
		while (actual != nullptr) {
			actual->dato->procesarLectura();
			actual = actual->siguiente;
		}
	}
	
	// Busca un elemento por el ID del objeto apuntado.
	// Para este proyecto T se utiliza como Sensor*.
	T buscarPorId(int id) const {
		Nodo<T>* actual = primero;

		while (actual != nullptr) {
			if (actual->dato != nullptr &&
				actual->dato->getId() == id) {
				return actual->dato;
			}
			actual = actual->siguiente;
		}

		return nullptr;
	}

	int getCantidad() const {
		return cantidad;
	}
	
	~ListaGen() {
		Nodo<T>* actual = primero;
		
		while (actual != nullptr) {
			Nodo<T>* temporal = actual;
			actual = actual->siguiente;
			
			delete temporal->dato;
			delete temporal;
		}
		
		primero = nullptr;
		cantidad = 0;
	}
	
	ListaGen(const ListaGen&) = delete;
	ListaGen& operator=(const ListaGen&) = delete;
};

#endif
