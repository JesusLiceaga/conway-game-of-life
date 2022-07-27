#include "conway.cpp"

COLORREF** buffer;

//------------------------------------ Main -----------------------------------------------
int main(){
	int v = 100;
	extern const int alto;
	extern const int ancho;
	
	buffer = createImage(alto, ancho);
	initBuffer(buffer);
	
	initCells();
	if(selectCells(&v, buffer) == 1)
		play(&v, buffer);
	
	freeImage(buffer, alto);
	return 0;
}
