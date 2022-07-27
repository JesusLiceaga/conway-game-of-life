#include "Shell.cpp"

//Este es el struct de cada casilla, dice si está viva y cuántas vecinas vivas tiene
typedef struct{
	int viva, vecinas;
} Celula;

//------------------------------ Variables globales --------------------------------------
//Número de células
//IMPORTANTE: NO se deben de modificar, de lo contrario se tendrá una apariencia extraña
const int x_cells = 80;
const int y_cells = 40;
const int alto = 539;
const int ancho = 885;

Window ventana("Juego de la Vida", ancho, alto);

//Estas son las matrices que usaremos
Celula cells[y_cells + 2][x_cells + 2];
Celula copia_cells[y_cells + 2][x_cells + 2];

//Estos son los colores
COLORREF gris = RGB(128, 128, 128);
COLORREF negro = RGB(0, 0, 0);
COLORREF blanco = RGB(255, 255, 255);

//------------------------------ Declaración de Funciones ----------------------------------
COLORREF** createImage(int nr, int nc);
void freeImage(COLORREF** imagen, int nr); 
COLORREF modifyCell(int x, int y, Celula matriz[y_cells + 2][x_cells + 2]);
int changeStateCell(int x, int y, Celula matriz[y_cells + 2][x_cells + 2]); 
void cellToBuffer(int x, int y, COLORREF color, COLORREF** buffer); 
void initBuffer(COLORREF** buffer); 
void playPause(int n, COLORREF** buffer);
int speed(int x, int y, COLORREF** buffer);
void initCells();
int selectCells(int* v, COLORREF** buffer);
void play(int* v, COLORREF** buffer); 

