#include "figuras.h"

Triangulo::Triangulo(InterfazDibujo& i, Punto punto1, Punto punto2, Punto punto3, const Color& colorLinea, Color* colorFondo) : Figura(i) {
    tipo = tipoTriangulo;
    puntosControl = new Punto[3];

    Punto pts[3] = { punto1, punto2, punto3 };

    // Ordenamiento burbuja para 3 elementos por Y (ascendente), luego X
    for (int i = 0; i < 2; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            if (pts[i].y > pts[j].y ||
                (pts[i].y == pts[j].y && pts[i].x > pts[j].x)) {
                Punto temp = pts[i];
                pts[i] = pts[j];
                pts[j] = temp;
            }
        }
    }

    puntosControl = new Punto[3];
    for (int i = 0; i < 3; ++i) {
        puntosControl[i] = pts[i];
    }

    this->colorLinea = colorLinea;
    if (colorFondo != nullptr)
        this->colorFondo = new Color(colorFondo->r, colorFondo->g, colorFondo->b);
    else
        this->colorFondo = nullptr;
}

void Triangulo::dibujar() {

    Linea uno(interfaz, puntosControl[0], puntosControl[1], colorLinea, colorFondo);
    Linea dos(interfaz, puntosControl[1], puntosControl[2], colorLinea, colorFondo);
    Linea tres(interfaz, puntosControl[2], puntosControl[0], colorLinea, colorFondo);


    if (colorFondo != nullptr) dibujarRelleno();

    uno.dibujar();
    dos.dibujar();
    tres.dibujar();

}

void Triangulo::dibujarRelleno() {

    // Si el triángulo no tiene altura, no hay nada que rellenar
    if (puntosControl[0].y == puntosControl[2].y) return;

    // Pendientes inversas (dx/dy) para cada lado
    // Pendiente inversa para el lado 0->1
    double invSlope01;
    double dy = puntosControl[1].y - puntosControl[0].y;
    if (dy != 0)
        invSlope01 = (puntosControl[1].x - puntosControl[0].x) / dy;
    else
        invSlope01 = 0.0;

    // Pendiente inversa para el lado 0->2
    double invSlope02;
    dy = puntosControl[2].y - puntosControl[0].y;
    if (dy != 0)
        invSlope02 = (puntosControl[2].x - puntosControl[0].x) / dy;
    else
        invSlope02 = 0.0;

    // Pendiente inversa para el lado 1->2
    double invSlope12;
    dy = puntosControl[2].y - puntosControl[1].y;
    if (dy != 0)
        invSlope12 = (puntosControl[2].x - puntosControl[1].x) / dy;
    else
        invSlope12 = 0.0;

    double x_left = puntosControl[0].x;
    double x_right = puntosControl[0].x;

    // Recorremos cada scanline desde la fila superior hasta la inferior
    for (int y = (int)puntosControl[0].y; y <= (int)puntosControl[2].y; ++y) {
        if (y <= puntosControl[1].y) {
            // Región superior: bordes (0->1) y (0->2)
            x_left = puntosControl[0].x + invSlope01 * (y - puntosControl[0].y);
            x_right = puntosControl[0].x + invSlope02 * (y - puntosControl[0].y);
        }
        else {
            // Región inferior: bordes (1->2) y (0->2)
            x_left = puntosControl[1].x + invSlope12 * (y - puntosControl[1].y);
            x_right = puntosControl[0].x + invSlope02 * (y - puntosControl[0].y);
        }

        // Asegurar orden izquierda-derecha
        if (x_left > x_right) std::swap(x_left, x_right);

        int ix_left = (int)std::round(x_left);
        int ix_right = (int)std::round(x_right);

        // Dibujar la línea horizontal (relleno)
        for (int x = ix_left; x <= ix_right; ++x) {
            interfaz.putPixel(x, y, *colorFondo);
        }
    }
}

bool Triangulo::colisiona(Punto esqSupIzq, Punto esqInfDer) {

    Punto centroRectangulo((esqSupIzq.x + esqInfDer.x) / 2.0, (esqSupIzq.y + esqInfDer.y) / 2.0);

    Linea lado1(interfaz, puntosControl[0], puntosControl[1], colorLinea, nullptr);
    Linea lado2(interfaz, puntosControl[1], puntosControl[2], colorLinea, nullptr);
    Linea lado3(interfaz, puntosControl[2], puntosControl[0], colorLinea, nullptr);

    if (lado1.colisiona(esqSupIzq, esqInfDer) ||
        lado2.colisiona(esqSupIzq, esqInfDer) ||
        lado3.colisiona(esqSupIzq, esqInfDer)) {
        return true;
    }

    if (colorFondo != nullptr) {
        // Verificamos si el centro está dentro del triángulo.
        // Usamos el método de orientación (producto cruz) para determinar si el punto
        // está del mismo lado de cada arista.

        double d1 = orientacion(centroRectangulo, puntosControl[0], puntosControl[1]);
        double d2 = orientacion(centroRectangulo, puntosControl[1], puntosControl[2]);
        double d3 = orientacion(centroRectangulo, puntosControl[2], puntosControl[0]);

        // Si todos los signos son iguales está dentro.
        return (d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0);
    }


    return false;
}

int Triangulo::getNumeroPuntosControl() {
    return 3;
}
Punto Triangulo::getControlPoint(int i) {
    return puntosControl[i];
}

Color Triangulo::getColorLinea() {
    return colorLinea;
}
Color* Triangulo::getColorFondo() {
    return colorFondo;
}

InterfazDibujo& Triangulo::getInterface() {
    return interfaz;
}