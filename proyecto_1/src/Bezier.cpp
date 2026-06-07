#include "figuras.h"

Bezier::Bezier(InterfazDibujo& i, vector<Punto> puntos, float dt, const Color& colorLinea, Color* colorFondo) : Figura(i) {
    
    tipo = tipoBezier;
    nNodos = puntos.size();
    puntosControl = new Punto[nNodos];

    for (int i = 0; i < nNodos; i++) {
        puntosControl[i] = puntos.at(i);
    }

    this->dt = dt;
    this->colorLinea = colorLinea;
    this->colorFondo = nullptr;

}

void Bezier::dibujar() {
    Punto anterior = puntosControl[0];


    for (float i = dt; i < 1; i += dt) {

        Punto actual = casteljau(i);

        Linea seg(interfaz, anterior, actual, colorLinea, colorFondo);
        seg.dibujar();

        anterior = actual;
    }

}

Punto Bezier::casteljau(float t) {

    vector<Punto> aux;

    for (int i = 0; i < nNodos; i++) {
        aux.push_back(puntosControl[i]);
    }


    for (int r = 1; r < nNodos; r++) {
        for (int i = 0; i < nNodos - r; i++) {
            aux.at(i).x = (1 - t) * aux.at(i).x + t * aux.at(i + 1).x;
            aux.at(i).y = (1 - t) * aux.at(i).y + t * aux.at(i + 1).y;
        }
    }

    return aux.at(0);
}

bool Bezier::colisiona(Punto esqSupIzq, Punto esqInfDer) {
    // Usar el mismo dt que usas para dibujar (o uno más fino)
    Punto anterior = puntosControl[0];

    for (float t = dt; t <= 1.0; t += dt) {
        Punto actual = casteljau(t);

        if (segmentosIntersectan(anterior, actual, esqSupIzq, Punto(esqInfDer.x, esqSupIzq.y))) return true;
        if (segmentosIntersectan(anterior, actual, Punto(esqInfDer.x, esqSupIzq.y), esqInfDer)) return true;
        if (segmentosIntersectan(anterior, actual, esqInfDer, Punto(esqSupIzq.x, esqInfDer.y))) return true;
        if (segmentosIntersectan(anterior, actual, Punto(esqSupIzq.x, esqInfDer.y), esqSupIzq)) return true;


        anterior = actual;
    }

    return false;
}

int Bezier::getNumeroPuntosControl() {
    return nNodos;
}
Punto Bezier::getControlPoint(int i) {
    return puntosControl[i];
}

Color Bezier::getColorLinea() {
    return colorLinea;
}
Color* Bezier::getColorFondo() {
    return colorFondo;
}

InterfazDibujo& Bezier::getInterface() {
    return interfaz;
}