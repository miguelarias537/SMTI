
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <exception>

#include "../include/ServidorSMTI.h"

const char CLASE_VENTANA[] = "SMTI_VENTANA";

HBRUSH fondo;
HBRUSH fondoTarjeta;
HFONT fuentes[30];
int cantidadFuentes = 0;

EstadoSMTI estadoGlobal;
ServidorSMTI* servidorGlobal = nullptr;
HANDLE hiloServidor = NULL;

HWND textoTemperatura;
HWND textoHumedad;
HWND textoVibracion;
HWND textoIdTemperatura;
HWND textoIdHumedad;
HWND textoIdVibracion;
HWND textoCantidad;
HWND textoConexion;

HWND CrearTexto(HWND ventana, const char* texto,
				int x, int y, int ancho, int alto,
				int tamano, bool negrita = false) {
	
	HWND control = CreateWindowA(
								 "STATIC", texto,
								 WS_CHILD | WS_VISIBLE | SS_LEFT,
								 x, y, ancho, alto,
								 ventana, NULL, NULL, NULL);
	
	HFONT letra = CreateFontA(
							  tamano, 0, 0, 0,
							  negrita ? FW_BOLD : FW_NORMAL,
							  FALSE, FALSE, FALSE,
							  DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
							  DEFAULT_PITCH, "Segoe UI");
	
	if (cantidadFuentes < 30)
		fuentes[cantidadFuentes++] = letra;
	
	SendMessage(control, WM_SETFONT,
				(WPARAM)letra, TRUE);
	
	return control;
}
				
				DWORD WINAPI EjecutarServidor(LPVOID parametro) {
    ServidorSMTI* servidor = static_cast<ServidorSMTI*>(parametro);

    try {
        servidor->iniciar();
    }
    catch (const std::exception& e) {
        OutputDebugStringA(e.what());
        MessageBoxA(NULL, e.what(),
                    "Error del servidor SMTI",
                    MB_OK | MB_ICONERROR);
    }
    catch (...) {
        OutputDebugStringA(
            "Error desconocido durante la ejecucion del servidor SMTI.");
    }

    return 0;
}

LRESULT CALLBACK ProcedimientoVentana(
													  HWND ventana, UINT mensaje,
													  WPARAM wParam, LPARAM lParam) {
					
					switch (mensaje) {
						
					case WM_CREATE:
						
						CrearTexto(ventana, "SMTI 2.0",
								   45, 25, 400, 50, 32, true);
						
						CrearTexto(ventana,
								   "SISTEMA DE MONITOREO Y TELEMETRIA INDUSTRIAL",
								   48, 80, 650, 25, 12);
						
						textoConexion = CrearTexto(
												   ventana, "SERVIDOR: INICIANDO...",
												   760, 40, 290, 30, 15, true);
						
						// Tarjeta de temperatura
						CrearTexto(ventana, "TEMPERATURA",
								   80, 185, 250, 30, 17, true);
						
						textoTemperatura = CrearTexto(
													  ventana, "0.0 C",
													  80, 235, 270, 65, 34, true);
						
						textoIdTemperatura = CrearTexto(
														ventana, "ID DEL SENSOR: --",
														80, 310, 250, 25, 12);
						
						// Tarjeta de humedad
						CrearTexto(ventana, "HUMEDAD",
								   420, 185, 250, 30, 17, true);
						
						textoHumedad = CrearTexto(
												  ventana, "0.0 %",
												  420, 235, 270, 65, 34, true);
						
						textoIdHumedad = CrearTexto(
													ventana, "ID DEL SENSOR: --",
													420, 310, 250, 25, 12);
						
						// Tarjeta de vibracion
						CrearTexto(ventana, "VIBRACION",
								   760, 185, 250, 30, 17, true);
						
						textoVibracion = CrearTexto(
													ventana, "0.0 mm/s",
													760, 235, 290, 65, 34, true);
						
						textoIdVibracion = CrearTexto(
													  ventana, "ID DEL SENSOR: --",
													  760, 310, 250, 25, 12);
						
						// Informacion inferior
						textoCantidad = CrearTexto(
												   ventana, "MEDICIONES RECIBIDAS: 0",
												   65, 450, 450, 30, 17, true);
						
						CreateWindowA(
									  "BUTTON", "Consultar sensores",
									  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
									  65, 535, 230, 45,
									  ventana, (HMENU)1, NULL, NULL);
						
						CreateWindowA(
									  "BUTTON", "Ver historial",
									  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
									  325, 535, 230, 45,
									  ventana, (HMENU)2, NULL, NULL);
						
						CreateWindowA(
									  "BUTTON", "Salir",
									  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
									  585, 535, 180, 45,
									  ventana, (HMENU)3, NULL, NULL);
						
						SetTimer(ventana, 1, 300, NULL);
						
						return 0;
						
					case WM_PAINT: {
						PAINTSTRUCT ps;
						HDC dc = BeginPaint(ventana, &ps);
						
						HBRUSH tarjeta = fondoTarjeta;
						HPEN borde = CreatePen(
											   PS_SOLID, 1, RGB(55, 70, 90));
						
						HGDIOBJ anteriorPincel =
							SelectObject(dc, tarjeta);
						
						HGDIOBJ anteriorLapiz =
							SelectObject(dc, borde);
						
						RoundRect(dc, 45, 155, 355, 375, 18, 18);
						RoundRect(dc, 395, 155, 705, 375, 18, 18);
						RoundRect(dc, 745, 155, 1055, 375, 18, 18);
						
						RoundRect(dc, 45, 440, 1055, 490, 12, 12);
						
						SelectObject(dc, anteriorPincel);
						SelectObject(dc, anteriorLapiz);
						
						DeleteObject(borde);
						EndPaint(ventana, &ps);
						return 0;
					}
					
					case WM_TIMER:
						
						if (wParam == 1) {
							DatosPanel datos = estadoGlobal.obtenerDatos();
							
							char texto[100];
							
							sprintf(texto, "%.1f C", datos.temperatura);
							SetWindowTextA(textoTemperatura, texto);
							
							sprintf(texto, "%.1f %%", datos.humedad);
							SetWindowTextA(textoHumedad, texto);
							
							sprintf(texto, "%.1f mm/s", datos.vibracion);
							SetWindowTextA(textoVibracion, texto);
							
							sprintf(texto, "ID DEL SENSOR: %d",
									datos.idTemperatura);
							SetWindowTextA(textoIdTemperatura, texto);
							
							sprintf(texto, "ID DEL SENSOR: %d",
									datos.idHumedad);
							SetWindowTextA(textoIdHumedad, texto);
							
							sprintf(texto, "ID DEL SENSOR: %d",
									datos.idVibracion);
							SetWindowTextA(textoIdVibracion, texto);
							
							sprintf(texto, "MEDICIONES RECIBIDAS: %d",
									datos.cantidadSensores);
							SetWindowTextA(textoCantidad, texto);
							
							if (datos.conectado)
								SetWindowTextA(textoConexion,
											   "TCP: CONECTADO");
							else
								SetWindowTextA(textoConexion,
											   "TCP: ESPERANDO CLIENTE");
							
							InvalidateRect(textoConexion, NULL, TRUE);
						}
						
						return 0;
						
					case WM_CTLCOLORSTATIC: {
						HDC dc = (HDC)wParam;
						HWND control = (HWND)lParam;
						
						SetBkMode(dc, TRANSPARENT);
						
						if (control == textoConexion) {
							DatosPanel datos = estadoGlobal.obtenerDatos();
							
							if (datos.conectado)
								SetTextColor(dc, RGB(65, 220, 130));
							else
								SetTextColor(dc, RGB(240, 100, 100));
						}
						else if (control == textoTemperatura ||
								 control == textoHumedad ||
								 control == textoVibracion) {
							SetTextColor(dc, RGB(100, 190, 255));
						}
						else if (control == textoCantidad) {
							SetTextColor(dc, RGB(100, 210, 160));
						}
						else {
							SetTextColor(dc, RGB(220, 230, 240));
						}
						
						return (LRESULT)fondo;
					}
					
					case WM_COMMAND:
						
						switch (LOWORD(wParam)) {
							
						case 1: {
							DatosPanel datos = estadoGlobal.obtenerDatos();
							
							char mensaje[400];
							
							sprintf(mensaje,
									"TEMPERATURA: %.1f C\n"
									"ID: %d\n\n"
									"HUMEDAD: %.1f %%\n"
									"ID: %d\n\n"
									"VIBRACION: %.1f mm/s\n"
									"ID: %d\n\n"
									"MEDICIONES: %d",
									datos.temperatura, datos.idTemperatura,
									datos.humedad, datos.idHumedad,
									datos.vibracion, datos.idVibracion,
									datos.cantidadSensores);
							
							MessageBoxA(ventana, mensaje,
										"Consulta de sensores",
										MB_OK | MB_ICONINFORMATION);
							
							return 0;
						}
						
						case 2: {
							RegistroMedicion registros[100];
							
							int total = estadoGlobal.obtenerHistorial(
																	  registros, 100);
							
							if (total == 0) {
								MessageBoxA(ventana,
											"Todavia no hay mediciones registradas.",
											"Historial SMTI",
											MB_OK | MB_ICONINFORMATION);
								return 0;
							}
							
							char mensaje[10000];
							int posicion = 0;
							
							posicion += sprintf(mensaje + posicion,
												"HISTORIAL DE MEDICIONES SMTI\n\n");
							
							for (int i = 0; i < total; i++) {
								posicion += sprintf(mensaje + posicion,
													"%d. Sensor: %s | ID: %d | Valor: %.2f\n",
													i + 1,
													registros[i].tipo,
													registros[i].idSensor,
													registros[i].valor);
							}
							
							MessageBoxA(ventana, mensaje,
										"Historial SMTI",
										MB_OK | MB_ICONINFORMATION);
							
							return 0;
						}
						
						case 3:
							DestroyWindow(ventana);
							return 0;
						}
						
						break;
						
					case WM_DESTROY:
						
						KillTimer(ventana, 1);
						
						if (servidorGlobal != nullptr)
							servidorGlobal->detener();
						
						if (hiloServidor != NULL) {
							WaitForSingleObject(hiloServidor, INFINITE);
							CloseHandle(hiloServidor);
							hiloServidor = NULL;
						}
						
						for (int i = 0; i < cantidadFuentes; i++)
							DeleteObject(fuentes[i]);
						
						DeleteObject(fondo);
						DeleteObject(fondoTarjeta);
						
						PostQuitMessage(0);
						return 0;
					}
					
					return DefWindowProc(
										 ventana, mensaje, wParam, lParam);
				}
													  
													  int WINAPI WinMain(
																		 HINSTANCE instancia,
																		 HINSTANCE anterior,
																		 LPSTR argumentos,
																		 int mostrar) {
														  
														  fondo = CreateSolidBrush(RGB(18, 25, 38));
														  fondoTarjeta = CreateSolidBrush(RGB(29, 40, 57));
														  
														  WNDCLASS ventanaClase = {};
														  
														  ventanaClase.lpfnWndProc = ProcedimientoVentana;
														  ventanaClase.hInstance = instancia;
														  ventanaClase.lpszClassName = CLASE_VENTANA;
														  ventanaClase.hCursor = LoadCursor(NULL, IDC_ARROW);
														  ventanaClase.hbrBackground = fondo;
														  
														  RegisterClass(&ventanaClase);
														  
														  HWND ventana = CreateWindowEx(
																						0,
																						CLASE_VENTANA,
																						"SMTI 2.0 | Monitoreo Industrial",
																						WS_OVERLAPPEDWINDOW,
																						CW_USEDEFAULT, CW_USEDEFAULT,
																						1100, 700,
																						NULL, NULL, instancia, NULL);
														  
														  if (!ventana)
															  return 0;
														  
														  ServidorSMTI servidor(&estadoGlobal);
														  servidorGlobal = &servidor;
														  
														  hiloServidor = CreateThread(
																					  NULL, 0, EjecutarServidor,
																					  &servidor, 0, NULL);
														  
														  if (hiloServidor == NULL) {
															  MessageBoxA(ventana,
																		  "No se pudo iniciar el servidor.",
																		  "Error SMTI",
																		  MB_OK | MB_ICONERROR);
															  
															  DestroyWindow(ventana);
															  return 1;
														  }
														  
														  ShowWindow(ventana, mostrar);
														  UpdateWindow(ventana);
														  
														  MSG mensaje;
														  
														  while (GetMessage(&mensaje, NULL, 0, 0) > 0) {
															  TranslateMessage(&mensaje);
															  DispatchMessage(&mensaje);
														  }
														  
														  return (int)mensaje.wParam;
													  }
																		 
