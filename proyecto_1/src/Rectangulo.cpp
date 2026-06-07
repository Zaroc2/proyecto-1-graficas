#include "figuras.h"

Rectangulo::Rectangulo(InterfazDibujo& i, Punto Esq1, Punto Esq2, const Color& colorLinea, Color* colorFondo) : Figura(i) {

    tipo = tipoRectangulo;

    Punto EsqSupIzq, EsqInfDer;

    if (Esq1.x < Esq2.x) {
        if (Esq1.y < Esq2.y) {
            EsqSupIzq = Esq1;
            EsqInfDer = Esq2;
        }
        else {
            EsqSupIzq = Punto(Esq1.x, Esq2.y);
            EsqInfDer = Punto(Esq2.x, Esq1.y);
        }
    }
    else {
        if (Esq1.y < Esq2.y) {
            EsqSupIzq = Punto(Esq2.x, Esq1.y);
            EsqInfDer = Punto(Esq1.x, Esq2.y);
        }
        else {
            EsqSupIzq = Esq2;
            EsqInfDer = Esq1;
        }
    }

    puntosControl = new Punto[4];
    puntosControl[0] = EsqSupIzq;
    puntosControl[1] = Punto(EsqInfDer.x, EsqSupIzq.y);
    puntosControl[2] = Punto(EsqSupIzq.x, EsqInfDer.y);
    puntosControl[3] = EsqInfDer;
    this->colorLinea = colorLinea;
    if (colorFondo != nullptr)
        this->colorFondo = new Color(colorFondo->r, colorFondo->g, colorFondo->b);
    else
        this->colorFondo = nullptr;

}

void Rectangulo::dibujar() {

    if (colorFondo != nullptr) {
        dibujarColor();
        return;
    }

    Linea sup(interfaz, puntosControl[0], puntosControl[1], colorLinea, colorFondo);
    Linea izq(interfaz, puntosControl[0], puntosControl[2], colorLinea, colorFondo);
    Linea der(interfaz, puntosControl[1], puntosControl[3], colorLinea, colorFondo);
    Linea inf(interfaz, puntosControl[2], puntosControl[3], colorLinea, colorFondo);
    sup.dibujar();
    izq.dibujar();
    der.dibujar();
    inf.dibujar();

}

void Rectangulo::dibujarColor() {

    for (int i = puntosControl[0].x; i <= puntosControl[1].x; i++) {
        for (int j = puntosControl[0].y; j <= puntosControl[2].y; j++) {
            interfaz.putPixel(i, j, *colorFondo);
        }
    }

    Linea sup(interfaz, puntosControl[0], puntosControl[1], colorLinea, colorFondo);
    Linea izq(interfaz, puntosControl[0], puntosControl[2], colorLinea, colorFondo);
    Linea der(interfaz, puntosControl[1], puntosControl[3], colorLinea, colorFondo);
    Linea inf(interfaz, puntosControl[2], puntosControl[3], colorLinea, colorFondo);
    sup.dibujar();
    izq.dibujar();
    der.dibujar();
    inf.dibujar();

}

bool Rectangulo::colisiona(Punto esqSupIzq, Punto esqInfDer) {

    if (colorFondo != nullptr) {

        return (puntosControl[3].x >= esqSupIzq.x && puntosControl[0].x <= esqInfDer.x &&
            puntosControl[3].y >= esqSupIzq.y && puntosControl[1].y <= esqInfDer.y);

    }
    else {

        Linea sup(interfaz, puntosControl[0], puntosControl[1], colorLinea, colorFondo);
        Linea izq(interfaz, puntosControl[0], puntosControl[2], colorLinea, colorFondo);
        Linea der(interfaz, puntosControl[1], puntosControl[3], colorLinea, colorFondo);
        Linea inf(interfaz, puntosControl[2], puntosControl[3], colorLinea, colorFondo);

        if (
            sup.colisiona(esqSupIzq, esqInfDer) ||
            izq.colisiona(esqSupIzq, esqInfDer) ||
            der.colisiona(esqSupIzq, esqInfDer) ||
            inf.colisiona(esqSupIzq, esqInfDer)
            )
            return true;
    }
    return false;

}

int Rectangulo::getNumeroPuntosControl() {
    return 4;
}
Punto Rectangulo::getControlPoint(int i) {
    return puntosControl[i];
}

Color Rectangulo::getColorLinea() {
    return colorLinea;
}
Color* Rectangulo::getColorFondo() {
    return colorFondo;
}

InterfazDibujo& Rectangulo::getInterface() {
    return interfaz;
}