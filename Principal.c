#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "libreria_AA2.h"


//funcion menu principal
int menu() {
	int opcion;
	// Imprimir opciones del menu
	printf("1. Comprobacion de IPs. \n");
	printf("2. Mostrar la configuracion de red. \n");
	printf("3. Comparacion de velocidad de DNS. \n");
	printf("4. Salir \n");
	// Pedir opcion
	printf("\n ----------------------------------------------------------------- \n");
	printf("Escoja una opcion: \n");
	scanf_s("%d", &opcion);
	return opcion;
}

//programa principal
int main() {
	int opcion;

	do {
		// iniciar el menu
		opcion = menu();

		switch (opcion) {
		case 1:
			lecturaIPs();
			break;
		case 2:
			mostrarRED();
			break;
		case 3:
			velocidadDNS();
			break;
		case 4:
			exit(0);
		}

	} while (opcion >= 1 || opcion <= 4);
}

