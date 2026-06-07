#include "figuras.h"

Linea::Linea(InterfazDibujo& i, Punto inicio, Punto final, const Color& colorLinea, Color* colorFondo) : Figura(i) {
    tipo = tipoLinea;
    puntosControl = new Punto[2];
    puntosControl[0] = inicio;
    puntosControl[1] = final;
    this->colorLinea = colorLinea;
    colorFondo = nullptr;
}

void Linea::dibujar() {

    if (puntosControl[0].x == puntosControl[1].x && puntosControl[0].y == puntosControl[1].y) {
        interfaz.putPixel((int)puntosControl[0].x, (int)puntosControl[0].y, colorLinea);
        return;
    }

    int dx = abs(((int)puntosControl[1].x - (int)puntosControl[0].x));
    int dy = abs(((int)puntosControl[1].y - (int)puntosControl[0].y));


    int sx = puntosControl[0].x < puntosControl[1].x ? 1 : -1;
    int sy = puntosControl[0].y < puntosControl[1].y ? 1 : -1;
    int err = dx - dy;

    int x0 = (int)puntosControl[0].x;
    int y0 = (int)puntosControl[0].y;
    int xEnd = (int)puntosControl[1].x;
    int yEnd = (int)puntosControl[1].y;

    while (true) {
        interfaz.putPixel(x0, y0, colorLinea);
        if (x0 == xEnd && y0 == yEnd) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }


}

bool Linea::colisiona(Punto esqSupIzq, Punto esqInfDer) {


    //Primero vemos si alguno de los puntos de control esta directamente en una línea del cuadrado
    if (esqSupIzq.x < puntosControl[0].x && puntosControl[0].x < esqInfDer.x &&
        esqSupIzq.y < puntosControl[0].y && puntosControl[0].y < esqInfDer.y    //Si esta en las lineas izq o arriba
        ||
        esqSupIzq.x < puntosControl[1].x && puntosControl[1].x < esqInfDer.x &&
        esqSupIzq.y < puntosControl[1].y && puntosControl[1].y < esqInfDer.y)   //Si esta en las lineas der o abajo
        return true;

    //Si no, entonces tenemos que ver si se intersectan los segmentos
    if (segmentosIntersectan(puntosControl[0], puntosControl[1], esqSupIzq, Punto(esqInfDer.x, esqSupIzq.y)) ||
        segmentosIntersectan(puntosControl[0], puntosControl[1], Punto(esqInfDer.x, esqSupIzq.y), esqInfDer) ||
        segmentosIntersectan(puntosControl[0], puntosControl[1], esqInfDer, Punto(esqSupIzq.x, esqInfDer.y)) ||
        segmentosIntersectan(puntosControl[0], puntosControl[1], Punto(esqSupIzq.x, esqInfDer.y), esqSupIzq)
        )
        return true;



    return false;
}

int Linea::getNumeroPuntosControl() {
    return 2;
}
Punto Linea::getControlPoint(int i) {
    return puntosControl[i];
}

Color Linea::getColorLinea() {
    return colorLinea;
}
Color* Linea::getColorFondo() {
    return colorFondo;
}

InterfazDibujo& Linea::getInterface() {
    return interfaz;
}