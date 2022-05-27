#define _CRT_SECURE_NO_WARNINGS
#define BUFFER_SIZE 256
#define tamIP 30

//funciones auxiliares
int cambio(char* cad, int n) {
	int i, c;
	c = getchar();

	if (c == EOF) {
		cad[0] = '\0';
		return 0;
	}
	if (c == '\n')
		i = 0;
	else {
		cad[0] = c;
		i = 1;
	}
	for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++)
		cad[i] = c;
	cad[i] = '\0';
	if (c != '\n' && c != EOF)
		while ((c = getchar()) != '\n' && c != EOF);
	return 1;
};
int comparar(const char* a, const char* b)
{
	if (strncmp(a, b, strlen(b)) == 0) return 1;
	return 0;
}

//lectura de archivo de IPs y comprobacion de respuesta
void lecturaIPs() {
	FILE* input;
	char nombreArchivo[100] = "";
	char ip[tamIP] = "";
	//seleccionar archivos con IPs
	printf("Introduzca la direccion del archivo con las IP: ");
	cambio(nombreArchivo, 100);

	//apertura del archivo
	if ((input = fopen(nombreArchivo, "r")) == NULL) {
		printf("No se pudo abrir el archivo %s. \n", nombreArchivo);
		return(-1);
	}
	printf("Contenido del archivo %s. \n", nombreArchivo);
	
	//mostrar direcciones
	while ((fgets(ip, tamIP, input) != NULL)) {
		printf("%s", ip);
	}
	printf("\n");
	fclose(input);


	//secuencia de lanzamiento de pings
	if ((input = fopen(nombreArchivo, "r")) == NULL) {
		printf("No se pudo abrir el archivo %s. \n", nombreArchivo);
		return(-1);
	}
	char comandoPing[30] = "";
	while ((fgets(ip, tamIP, input)) != NULL) {
		strcat(strcpy(comandoPing, "ping "), ip);
		system(comandoPing);
	}
	printf("\n");
	fclose(input);
}

//solicita el nombre del adaptador de red y muestra su informacion
void mostrarRED() {
	FILE* input;
	char adaptador[BUFFER_SIZE] = "";
	printf("Introduce el nombre del adaptador del que quieras conocer la configuracion de red. \n");
	cambio(adaptador, BUFFER_SIZE);

	//redireccionar ipconfig stdout al archivo nuevo des de la consola en el mismo comando
	system("ipconfig > informacionAdaptador.txt");
	if ((input = fopen("informacionAdaptador.txt", "r")) == NULL) {
		printf("No se pudo abrir el archivo %s. \n", adaptador);
		return(-1);
	}

	//volcar resultados
	char ch[BUFFER_SIZE] = "";
	int encontrado = 0;
	int numeroDeLineasVacias = 0;
	char IPv4[16] = "";
	char subred[16] = "";
	char puertaEnlace[16] = "";
	char DNS[16] = "";

	while ((fgets(ch, BUFFER_SIZE, input) != NULL) && (numeroDeLineasVacias != 2)) {
		//comprobacion de si la lectura es la misma que el adaptador
		if ((encontrado == 1) || (comparar(ch, adaptador) == 1)) {
			encontrado = 1;

			if (comparar(ch, "\n")) {
				numeroDeLineasVacias++;
			}
			if ((comparar(ch, "   IPv4 Address") == 1) || (comparar(ch, "   Direcci¢n IPv4") == 1)) {
				sscanf(ch, "%*[^:]:%[^\n]", &IPv4);
			}
			if ((comparar(ch, "   Subnet Mask") == 1) || (comparar(ch, "   M scara de subred") == 1)) {
				sscanf(ch, "%*[^:]:%[^\n]", &subred);
			}
			if ((comparar(ch, "   Default Gateway") == 1) || (comparar(ch, "   Puerta de enlace predeterminada") == 1)) {
				sscanf(ch, "%*[^:]:%[^\n]", &puertaEnlace);
			}
			if ((comparar(ch, "   DNS Subfix") == 1) || (comparar(ch, "   Sufijo DNS espec¡fico para la conexi¢n") == 1)) {
				sscanf(ch, "%*[^:]:%[^\n]", &DNS);
			}
		}
	}

	//volcar informacion en pantalla si existe
	if (IPv4[0] != '\0') {
		printf("IP -> %s \n", IPv4);
	}
	if (subred[0] != '\0') {
		printf("Mascara de subred -> %s \n", subred);
	}
	if (puertaEnlace[0] != '\0') {
		printf("Puerta de enlace -> %s \n", puertaEnlace);
	}
	if (DNS[0] != '\0') {
		printf("DNS -> %s \n", DNS);
	}

	//eliminar archivos generados en el proceso
	if (remove("informacionAdaptador.txt") == 0) {
		printf("Archivo informacionAdaptador.txt borrado. \n");
	}
	else {
		printf("El archivo informacionAdaptador.txt no se ha podido borrar o no existe. \n");
	}
}

//comprobación de velocidad de DNS
void velocidadDNS() {
	FILE* dns1;
	FILE* dns2;

	//declaracion de variables
	char ip1[25], ip2[25];
	int velocidadMinima1 = 0, velocidadMinima2 = 0, media1 = 0, media2 = 0, numeroDeLinea = 0;
	int velocidadTotal = 0, velocidadTemporal = 0, contador = 0;

	printf("Introduce el primer DNS a comprobar \n");
	cambio(ip1, 25);
	printf("Introduce el segundo DNS a comprobar \n");
	cambio(ip2, 25);

	printf("Los DNS selecionados son: %s y %s \n", ip1, ip2);

	char comandoPing1[30] = "";
	char comandoPing2[30] = "";
	//construccion del ping
	strcat(strcat(strcpy(comandoPing1, "ping "), ip1), " > \"dns1.txt\"");
	strcat(strcat(strcpy(comandoPing2, "ping "), ip2), " > \"dns2.txt\"");
	system(comandoPing1);
	system(comandoPing2);

	//busqueda del mas rapido
	dns1 = fopen("dns1.txt", "r");
	if (dns1 == NULL) {
		printf("No se pudo lanzar el ping a esta ip %s. \n", ip1);
		return(-1);
	}

	char ch[BUFFER_SIZE] = "";
	//informacion del primer dns
	while ((fgets(ch, BUFFER_SIZE, dns1)) != NULL) {
		numeroDeLinea++;
		if ((numeroDeLinea >= 3) && (numeroDeLinea <= 6)) {
			sscanf(ch, "%*[^=]=%*[^=]=%d%*[^\n]", &velocidadTemporal);
			velocidadTotal += velocidadTemporal;
			if ((velocidadTemporal < velocidadMinima1) || (velocidadMinima1 == 0)) {
				velocidadMinima1 = velocidadTemporal;
			}
			contador++;
		}
	}
	fclose(dns1);
	//calculo de media
	media1 = velocidadTotal / contador;

	//reinicio de variables
	numeroDeLinea = 0;
	velocidadTemporal = 0;
	velocidadTotal = 0;
	contador = 0;

	dns2 = fopen("dns2.txt", "r");
	if (dns2 == NULL) {
		printf("Error al lanzar ping a ip %s. \n", ip2);
		return(-1);
	}
	//informacion del segundo dns
	while ((fgets(ch, BUFFER_SIZE, dns2)) != NULL) {
		numeroDeLinea++;
		if ((numeroDeLinea >= 3) && (numeroDeLinea <= 6)) {
			sscanf(ch, "%*[^=]=%*[^=]=%d%*[^\n]", &velocidadTemporal);
			velocidadTotal += velocidadTemporal;
			if ((velocidadTemporal < velocidadMinima2) || (velocidadMinima2 == 0)) {
				velocidadMinima2 = velocidadTemporal;
			}
			contador++;
		}
	}
	fclose(dns2);
	//calculo de media
	media2 = velocidadTotal / contador;

	//dns mas rapido
	if (media1 < media2) {
		printf("El DNS %s es mas rapido que el DNS %s. \n", ip1, ip2);
		printf("La velocidad media de %s es %dms, y la de %s es %dms. \n", ip1, media1, ip2, media2);
	}
	else if (media1 > media2) {
		printf("El DNS %s es mas rapido que el DNS %s. \n", ip2, ip1);
		printf("La velocidad media de %s es %dms, y la de %s es %dms. \n", ip2, media2, ip1, media1);


	}
	else { //calculo del minimo
		if (velocidadMinima1 < velocidadMinima2) {
			printf("Los dos DNS tienen la misma media, pero %s tiene un minimo mas rapido que %s. \n", ip1, ip2);
			printf("La velocidad mas rapida de %s es %dms, y la de %s es %dms. \n", ip1, velocidadMinima1, ip2, velocidadMinima2);

		}
		else if (velocidadMinima1 > velocidadMinima2) {
			printf("Los dos DNS tienen la misma media, pero %s tiene un minimo mas rapido que %s. \n", ip2, ip1);
			printf("La velocidad mas rapida de %s es %dms, y la de %s es %dms. \n", ip2, velocidadMinima2, ip1, velocidadMinima1);

		}
		else {
			printf("Los dos DNS son igual de rapidos. \n");
		}
	}
	printf("Procediendo a borrar archivos de texto generados. \n");

	//borrado de archivos generados
	if (remove("dns1.txt") == 0) {
		printf("Archivo dns1.txt borrado. \n");
	}
	else {
		printf("El archivo dns1.txt no se ha podido borrar o no existe. \n");
	}
	if (remove("dns2.txt") == 0) {
		printf("Archivo dns2.txt borrado. \n");
	}
	else {
		printf("El archivo dns1.txt no se ha podido borrar o no existe. \n");
	}

}