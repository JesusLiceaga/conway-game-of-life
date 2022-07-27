#include "conway.hpp"

//Genera el espacio necesario para una imagen
COLORREF** createImage(int nr, int nc){
	int i;
	COLORREF** imagen;
	
	imagen = (COLORREF**) malloc(nr * sizeof(COLORREF*));
	for (i = 0; i < nr; i++)
		imagen[i] = (COLORREF*) malloc(nc * sizeof(COLORREF));
	
	return imagen;
}

//Libera la memoria usada para una imagen
void freeImage(COLORREF** imagen, int nr){	
	int i;
	
	for (i=0; i < nr; i++)
		free(imagen[i]);	
	free(imagen);
	
	return;
}

/*Cambia el estado de la célula (x, y). Si estaba viva pasa a estar muerta y viceversa.
También, aumenta o disminuye en 1 la cantidad de vecinos que tienen las células colindantes.
Regresa por la izquierda el color del nuevo estado de la célula (blanca muerta, negra viva). */
COLORREF modifyCell(int x, int y, Celula matriz[y_cells + 2][x_cells + 2]){
	COLORREF color;
	
	if(matriz[y][x].viva == 0){ //0 es que estaba muerta
		color = negro;
		matriz[y][x].viva = 1;
		matriz[y - 1][x - 1].vecinas++;
		matriz[y][x - 1].vecinas++;
		matriz[y - 1][x].vecinas++;
		matriz[y + 1][x - 1].vecinas++;
		matriz[y - 1][x + 1].vecinas++;
		matriz[y][x + 1].vecinas++;
		matriz[y + 1][x].vecinas++;
		matriz[y + 1][x + 1].vecinas++; 
	}
	else{ //1 es que está viva
		color = blanco;
		matriz[y][x].viva = 0;
		matriz[y - 1][x - 1].vecinas--;
		matriz[y][x - 1].vecinas--;
		matriz[y - 1][x].vecinas--;
		matriz[y + 1][x - 1].vecinas--;
		matriz[y - 1][x + 1].vecinas--;
		matriz[y][x + 1].vecinas--;
		matriz[y + 1][x].vecinas--;
		matriz[y + 1][x + 1].vecinas--; 
	}
	
	return color;
}

/* Con base en el estado actual de la célula y dependiendo de la cantidad de vecinos
vivos que tenga (siguiendo las reglas del juego de Conway), decide si permanece en su 
estado actual (regresa 0) o si debe de cambiar (regresa 1) */
int changeStateCell(int x, int y, Celula matriz[y_cells + 2][x_cells + 2]){
	if(matriz[y][x].viva == 0){
		//Si está muerta y tiene exactamente 3 vecinos vivos revive
		if(matriz[y][x].vecinas == 3) 
			return 1;
		else
			return 0;
	}
	else{
		//Si está viva y tiene 2 o 3 vecions vivos permanece viva
		if(matriz[y][x].vecinas == 2 || matriz[y][x].vecinas == 3)
			return 0;
		else
			return 1;
	}
}

//"Imprime" la célula (x, y) en el buffer con el color que se le pase
void cellToBuffer(int x, int y, COLORREF color, COLORREF** buffer){
	int i, j;
	
	for(i = 11 * y + 1; i < 11 * y + 11; i++){
		for(j = 11 * x; j < 11 * x + 10; j++)
			buffer[i][j] = color;
	}
	
	return;
}

//Establece el estado inicial del buffer
void initBuffer(COLORREF** buffer){
	int nr, nc, i, j;
	COLORREF** imagen;
	
	//Pintamos todo de blanco
	for(i = 0; i < alto; i++){
		for(j = 0; j < ancho; j++)
			buffer[i][j] = blanco;
	}
	
	//Pintamos el botón de play, el control de velocidad y el stop
	playPause(1, buffer);
	speed(601, 477, buffer); //Inicia con 3

	DimensionesBmp("stop.bmp", &nr, &nc);
	imagen = createImage(nr, nc);
	LeeBmpColor(imagen, "stop.bmp");
	for(i = 450; i < 450 + nr; i++){
		for(j = 320; j < 320 + nc; j++)
			buffer[i][j] = imagen[i - 450][j - 320];
	}
	freeImage(imagen, nr);
	
	//Pintamos la cuadrícula
	for(j = 1; j < x_cells + 1; j++){
		for(i = 0; i < alto - 100 + 1; i++)
			buffer[i][11 * j - 1] = gris;
	}
	for(i = 0; i < y_cells + 1; i++){
		for(j = 0; j < ancho; j++)
			buffer[11 * i][j] = gris;
	}

	ventana.PlotCBitmap(buffer, alto, ancho, 0, 0, 1.0);
	return;
}


//Realiza cambios en la apariencia del menú
void playPause(int n, COLORREF** buffer){
	int nr, nc, i, j, x, y, resultado;
	COLORREF** imagen;
	
	DimensionesBmp("play.bmp", &nr, &nc);
	imagen = createImage(nr, nc);
	
	//Cambia el botón de play a pause o al revés
	if(n == 0)
		LeeBmpColor(imagen, "pause.bmp");
	else
		LeeBmpColor(imagen, "play.bmp");
	
	for(i = 450; i < 450 + nr; i++){
		for(j = 190; j < 190 + nc; j++)
			buffer[i][j] = imagen[i - 450][j - 190];
	}
	freeImage(imagen, nr);
	ventana.PlotCBitmap(buffer, alto, ancho, 0, 0, 1.0);
}

//Determina la velocidad de juego
int speed(int x, int y, COLORREF** buffer){
	int nr, nc, i, j, v;
	COLORREF** imagen;
	
	DimensionesBmp("menu_1.bmp", &nr, &nc);
	imagen = createImage(nr, nc);
	
	if(x > 532 && x < 559 && y > 476 && y < 503){
		LeeBmpColor(imagen, "menu_1.bmp");
		v = 200;
	}
	else if(x > 566 && x < 593 && y > 476 && y < 503){
		LeeBmpColor(imagen, "menu_2.bmp");
		v = 150;
	}
	else if(x > 600 && x < 627 && y > 476 && y < 503){
		LeeBmpColor(imagen, "menu_3.bmp");
		v = 100;
	}
	else if(x > 634 && x < 661 && y > 476 && y < 503){
		LeeBmpColor(imagen, "menu_4.bmp");
		v = 60;
	}
	else if(x > 668 && x < 695 && y > 476 && y < 503){
		LeeBmpColor(imagen, "menu_5.bmp");
		v = 30;
	}
	
	//Imprime el menú correspondiente
	for(i = 475; i < 475 + nr; i++){
		for(j = 533; j < 533 + nc; j++)
			buffer[i][j] = imagen[i - 475][j - 533];
	}
	freeImage(imagen, nr);
	ventana.PlotCBitmap(buffer, alto, ancho, 0, 0, 1.0);
	
	return v;
}

//Inicializa todas las células muertas
void initCells(){
	int i, j;
	
	for(i = 0; i < y_cells + 2; i++){
		for(j = 0; j < x_cells + 2; j++){
			cells[i][j].viva = 0;
			cells[i][j].vecinas = 0;
			copia_cells[i][j].viva = 0;
			copia_cells[i][j].vecinas = 0;
		}
	}
	
	return;
}

//Permite seleccionar la configuración inicial
int selectCells(int* v, COLORREF** buffer){
	int resultado, x, y, x0, y0, i;
	COLORREF color;
	
	while(1){
		resultado = MouseHit(&x, &y);

		if (resultado == 1){
			if(x > 199 && x < 301 && y > 459 && y < 519){ //Si le dio click al botón de play, jugamos
				playPause(0, buffer);
				return 1;
			}
			else if(x > 329 && x < 431 && y > 459 && y < 519){ //Si le da al stop, terminamos el programa
				return 0;
			}
			else if(y > 476 && y < 503 && ((x > 532 && x < 559) || (x > 566 & x < 593) || (x > 600 && x < 627) 
			|| (x > 634 && x < 661) || (x > 668 && x < 695))){ //Vemos si cambia la velocidad
				*v = speed(x, y, buffer);
			}
			
			//Vemos en qué "cuadrante" le dio click el usuario
			x0 = floor((float) x / 11);
			y0 = floor((float) y / 11);
			
			//Vemos si se hizo click en un punto de la cuadrícula
			if (y0 < y_cells && x0 < x_cells){
				/*Cambiamos el estado de la célula clickeada
				los +1 son una "traslación" de la matriz, esto se explica un poco más en la función play */
				color = modifyCell(x0 + 1, y0 + 1, cells);
				color = modifyCell(x0 + 1, y0 + 1, copia_cells);
				
				//Imprimimos la célula con el color correspondiente
				cellToBuffer(x0, y0, color, buffer);
				ventana.PlotCBitmap(buffer, alto, ancho, 0, 0, 1.0);
			}
		}
		
		Sleep(100);
	}
}

/* Esta función va cambiando lo que se muestra en pantalla (es el juego como tal).
Nótese que, a pesar de que nuestra matriz es de 42 x 82, solamente trabajamos con una de 40 x 80,
esto es porque las restantes forman una "envoltura" de células que siempre van a estar muertas,
con el objetivo de que el programa funcione bien en los bordes.
Por otra parte, se trabaja con 2 matrices, cells y copia_cells, esto para que cambiar de estado
a una célula no modifique la cantidad de vecinos vivos que las demás tienen hasta que se hayan
cambiado todas las necesarias.*/
void play(int* v, COLORREF** buffer){
	int i, j, x, y, n, estado, resultado;
	
	while(1){
		//Vemos si cambiamos cada célula
		for(i = 1; i < y_cells + 1; i++){
			for(j = 1; j < x_cells + 1; j++){
				estado = changeStateCell(j, i, cells);
				
				//Cambiamos la copia
				if(estado == 1)
					cellToBuffer(j - 1, i - 1, modifyCell(j, i, copia_cells), buffer); 
			}
		}
		
		//Hacemos la original igual a la copia
		for(i = 0; i < y_cells + 2; i++){
			for(j = 0; j < x_cells + 2; j++)
				cells[i][j] = copia_cells[i][j]; 
		}
		
		//Imprimimos la nueva configuración
		ventana.PlotCBitmap(buffer, alto, ancho, 0, 0, 1.0);
		
		//Vemos si el usuario dio click
		resultado = MouseHit(&x, &y);
		if(resultado == 1){
			if(x > 329 && x < 431 && y > 459 && y < 519){ //Si da click en stop termiamos el programa
				return;
			}
			else if(x > 199 && x < 301 && y > 459 && y < 519){ //Si da click en el botón de play, esperamos
				playPause(1, buffer);
				while(1){
					resultado = MouseHit(&x, &y);
					if (resultado == 1){
						if(x > 199 && x < 301 && y > 459 && y < 519){ //Si vuelve a darle en play, continuamos
							playPause(0, buffer);
							break;
						}
						else if(x > 329 && x < 431 && y > 459 && y < 519){ //Si le da en stop, termina
							return;	
						}
						else if(y > 476 && y < 503 && ((x > 532 && x < 559) || (x > 566 & x < 593) || (x > 600 && x < 627) 
						|| (x > 634 && x < 661) || (x > 668 && x < 695))){ //Vemos si cambia la velocidad
							*v = speed(x, y, buffer);
						}
					}
					Sleep(100);
				}
			}
			else if(y > 476 && y < 503 && ((x > 532 && x < 559) || (x > 566 & x < 593) || (x > 600 && x < 627) 
			|| (x > 634 && x < 661) || (x > 668 && x < 695))){ //Vemos si cambia la velocidad
				*v = speed(x, y, buffer);
			}
		}
		
		Sleep(*v);
	}
}
