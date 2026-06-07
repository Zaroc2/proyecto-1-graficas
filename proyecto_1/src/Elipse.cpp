#include "figuras.h"

Elipse::Elipse(InterfazDibujo& i, Punto centro, Punto eje1, Punto eje2, const Color& colorLinea, Color* colorFondo) : Figura(i) {
    tipo = tipoElipse;
    //eje1 es el eje en la dimension y, eje2 es el eje en la dimension x
    puntosControl = new Punto[3];
    puntosControl[0] = centro;

    puntosControl[1] = eje2;
    puntosControl[2] = eje1;

    this->colorLinea = colorLinea;
    if (colorFondo != nullptr)
        this->colorFondo = new Color(colorFondo->r, colorFondo->g, colorFondo->b);
    else
        this->colorFondo = nullptr;
}

void Elipse::put4Pixels(Punto punto) {

    if (colorFondo != nullptr) {

        for (int i = puntosControl[0].x - punto.x; i <= puntosControl[0].x + punto.x; i++) {
            interfaz.putPixel(i, puntosControl[0].y + punto.y, *colorFondo);
            interfaz.putPixel(i, puntosControl[0].y - punto.y, *colorFondo);
        }

    }
    interfaz.putPixel(puntosControl[0].x + punto.x, puntosControl[0].y + punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.x, puntosControl[0].y + punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x + punto.x, puntosControl[0].y - punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.x, puntosControl[0].y - punto.y, colorLinea);
}

void Elipse::dibujar() {
    int xc = puntosControl[0].x;
    int yc = puntosControl[0].y;
    int a = abs(puntosControl[1].x - xc);   // semieje X
    int b = abs(puntosControl[2].y - yc);   // semieje Y

    int a2 = a * a;
    int b2 = b * b;
    int a2b2 = a2 * b2;

    int x = 0;
    int y = b;
    int d = 4 * b2 + a2 * (1 - 4 * b);

    // Caso sin relleno: usar el método original (solo borde)
    if (colorFondo == nullptr) {
        // Algoritmo de punto medio original sin relleno
        auto putBorder = [&](int px, int py) {
            interfaz.putPixel(xc + px, yc + py, colorLinea);
            interfaz.putPixel(xc - px, yc + py, colorLinea);
            interfaz.putPixel(xc + px, yc - py, colorLinea);
            interfaz.putPixel(xc - px, yc - py, colorLinea);
            };
        putBorder(x, y);
        while (2 * b2 * (x + 1) < a2 * (2 * y - 1)) {
            if (d < 0) {
                d += 4 * b2 * (2 * x + 3);
            }
            else {
                d += 4 * (b2 * (2 * x + 3) + a2 * (-2 * y + 2));
                y--;
            }
            x++;
            putBorder(x, y);
        }
        putBorder(x, y);
        d = b2 * (4 * x * x + 4 * x + 1) + a2 * (4 * y * y - 8 * y + 4) - 4 * a2b2;
        while (y > 0) {
            putBorder(x, y);
            if (d < 0) {
                d += 4 * (b2 * (2 * x + 2) + a2 * (-2 * y + 3));
                x++;
            }
            else {
                d += 4 * a2 * (-2 * y + 3);
            }
            y--;
        }
        putBorder(x, 0);
        return;
    }

    // --- Con relleno: dos pasadas ---
    std::vector<Punto> bordes;
    auto fillFor = [&](int px, int py) {
        // Rellena las dos líneas horizontales simétricas en ±py
        for (int i = xc - px; i <= xc + px; ++i) {
            interfaz.putPixel(i, yc + py, *colorFondo);
            interfaz.putPixel(i, yc - py, *colorFondo);
        }
        // Guarda el punto (px, py) para dibujar el borde después
        bordes.push_back(Punto{ (double)px, (double)py });
        };

    // Primera pasada: rellenar todas las franjas horizontales
    fillFor(x, y);
    while (2 * b2 * (x + 1) < a2 * (2 * y - 1)) {
        if (d < 0) {
            d += 4 * b2 * (2 * x + 3);
        }
        else {
            d += 4 * (b2 * (2 * x + 3) + a2 * (-2 * y + 2));
            y--;
        }
        x++;
        fillFor(x, y);
    }
    fillFor(x, y);
    d = b2 * (4 * x * x + 4 * x + 1) + a2 * (4 * y * y - 8 * y + 4) - 4 * a2b2;
    while (y > 0) {
        fillFor(x, y);
        if (d < 0) {
            d += 4 * (b2 * (2 * x + 2) + a2 * (-2 * y + 3));
            x++;
        }
        else {
            d += 4 * a2 * (-2 * y + 3);
        }
        y--;
    }
    fillFor(x, 0);

    // Segunda pasada: dibujar el borde usando los puntos guardados
    for (const auto& p : bordes) {
        int px = (int)p.x;
        int py = (int)p.y;
        // Los cuatro puntos simétricos (suficiente para la elipse)
        interfaz.putPixel(xc + px, yc + py, colorLinea);
        interfaz.putPixel(xc - px, yc + py, colorLinea);
        interfaz.putPixel(xc + px, yc - py, colorLinea);
        interfaz.putPixel(xc - px, yc - py, colorLinea);
    }
}

bool Elipse::colisiona(Punto esqSupIzq, Punto esqInfDer) {

    //Vamos a hacer una transformacion lineal para convertir la elipse en un circulo unitario
    //Los semiejes
    double a = std::abs(puntosControl[1].x - puntosControl[0].x);
    double b = std::abs(puntosControl[2].y - puntosControl[0].y);
    if (a < 1e-6) a = 1e-6;
    if (b < 1e-6) b = 1e-6;

    // Transformamos las coordenadas del rectangulo con la misma transformacion lineal
    double txmin = (esqSupIzq.x - puntosControl[0].x) / a;
    double txmax = (esqInfDer.x - puntosControl[0].x) / a;
    double tymin = (esqSupIzq.y - puntosControl[0].y) / b;
    double tymax = (esqInfDer.y - puntosControl[0].y) / b;

    // Punto más cercano al origen
    double closestX = std::max(txmin, std::min(0.0, txmax));
    double closestY = std::max(tymin, std::min(0.0, tymax));
    double dist2 = closestX * closestX + closestY * closestY;
    double dist = std::sqrt(dist2);

    if (colorFondo != nullptr) {
        // Relleno: cualquier contacto
        return dist <= 1.0;
    }
    else {
        // Solo borde: el rectángulo debe atravesar la circunferencia
        double farDist2 = std::max({
    txmin * txmin + tymin * tymin,
    txmin * txmin + tymax * tymax,
    txmax * txmax + tymin * tymin,
    txmax * txmax + tymax * tymax
            });
        double farDist = std::sqrt(farDist2);
        return (dist <= 1.0 && farDist >= 1.0);
    }
}

int Elipse::getNumeroPuntosControl() {
    return 3;
}
Punto Elipse::getControlPoint(int i) {
    return puntosControl[i];
}

Color Elipse::getColorLinea() {
    return colorLinea;
}
Color* Elipse::getColorFondo() {
    return colorFondo;
}

InterfazDibujo& Elipse::getInterface() {
    return interfaz;
}