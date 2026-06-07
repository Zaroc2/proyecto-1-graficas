#include "figuras.h"

Circulo::Circulo(InterfazDibujo& i, Punto centro, Punto radio, const Color& colorLinea, Color* colorFondo) : Figura(i) {
    tipo = tipoCirculo;
    puntosControl = new Punto[2];
    puntosControl[0] = centro;
    puntosControl[1] = radio;
    this->colorLinea = colorLinea;
    if (colorFondo != nullptr)
        this->colorFondo = new Color(colorFondo->r, colorFondo->g, colorFondo->b);
    else
        this->colorFondo = nullptr;
}

void Circulo::put8Pixels(Punto punto) {

    if (colorFondo != nullptr) {

        for (int i = puntosControl[0].x - punto.x; i <= puntosControl[0].x + punto.x; i++) {
            interfaz.putPixel(i, puntosControl[0].y + punto.y, *colorFondo);
            interfaz.putPixel(i, puntosControl[0].y - punto.y, *colorFondo);
        }
        for (int i = puntosControl[0].x - punto.y; i <= puntosControl[0].x + punto.y; i++) {
            interfaz.putPixel(i, puntosControl[0].y + punto.x, *colorFondo);
            interfaz.putPixel(i, puntosControl[0].y - punto.x, *colorFondo);
        }

    }


    interfaz.putPixel(puntosControl[0].x + punto.x, puntosControl[0].y + punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.x, puntosControl[0].y + punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x + punto.x, puntosControl[0].y - punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.x, puntosControl[0].y - punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x + punto.y, puntosControl[0].y + punto.x, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.y, puntosControl[0].y + punto.x, colorLinea);
    interfaz.putPixel(puntosControl[0].x + punto.y, puntosControl[0].y - punto.x, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.y, puntosControl[0].y - punto.x, colorLinea);
}

void Circulo::dibujar() {

    int x = 0;

    //Aplicamos Pitagoras para encontrar la distancia del centro al radio (pues el radio estara donde sea que este el mouse)
    int cateto1 = abs(puntosControl[0].x - puntosControl[1].x);
    int cateto2 = abs(puntosControl[0].y - puntosControl[1].y);

    int y = (int)glm::sqrt(cateto1 * cateto1 + cateto2 * cateto2);


    int d = 1 - y;

    // Si no hay relleno, usar el método original simple
    if (colorFondo == nullptr) {
        put8Pixels(Punto(x, y));
        while (y > x) {
            if (d < 0) {
                d += 2 * x + 3;
            }
            else {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
            put8Pixels(Punto(x, y));
        }
        return;
    }

    // Si hay relleno: PRIMERA pasada -> pintar sólo relleno (scanlines) y guardar puntos de borde
    std::vector<Punto> bordes;
    auto fillFor = [&](const Punto& p) {
        // span horizontal en y +/- p.y
        for (int i = puntosControl[0].x - (int)p.x; i <= puntosControl[0].x + (int)p.x; ++i) {
            interfaz.putPixel(i, puntosControl[0].y + (int)p.y, *colorFondo);
            interfaz.putPixel(i, puntosControl[0].y - (int)p.y, *colorFondo);
        }
        // span horizontal en y +/- p.x (los simétricos)
        for (int i = puntosControl[0].x - (int)p.y; i <= puntosControl[0].x + (int)p.y; ++i) {
            interfaz.putPixel(i, puntosControl[0].y + (int)p.x, *colorFondo);
            interfaz.putPixel(i, puntosControl[0].y - (int)p.x, *colorFondo);
        }
        };

    // primera muestra
    fillFor(Punto{ (double)x, (double)y });
    bordes.push_back(Punto{ (double)x, (double)y });

    // primera región
    while (y > x) {
        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        fillFor(Punto{ (double)x, (double)y });
        bordes.push_back(Punto{ (double)x, (double)y });
    }

    // SEGUNDA pasada: dibujar sólo el contorno usando los puntos guardados
    for (const auto& p : bordes) {
        interfaz.putPixel(puntosControl[0].x + (int)p.x, puntosControl[0].y + (int)p.y, colorLinea);
        interfaz.putPixel(puntosControl[0].x - (int)p.x, puntosControl[0].y + (int)p.y, colorLinea);
        interfaz.putPixel(puntosControl[0].x + (int)p.x, puntosControl[0].y - (int)p.y, colorLinea);
        interfaz.putPixel(puntosControl[0].x - (int)p.x, puntosControl[0].y - (int)p.y, colorLinea);
        interfaz.putPixel(puntosControl[0].x + (int)p.y, puntosControl[0].y + (int)p.x, colorLinea);
        interfaz.putPixel(puntosControl[0].x - (int)p.y, puntosControl[0].y + (int)p.x, colorLinea);
        interfaz.putPixel(puntosControl[0].x + (int)p.y, puntosControl[0].y - (int)p.x, colorLinea);
        interfaz.putPixel(puntosControl[0].x - (int)p.y, puntosControl[0].y - (int)p.x, colorLinea);
    }
}

bool Circulo::colisiona(Punto esqSupIzq, Punto esqInfDer) {

    int radio = glm::sqrt((puntosControl[0].x - puntosControl[1].x) * (puntosControl[0].x - puntosControl[1].x) + (puntosControl[0].y - puntosControl[1].y) * (puntosControl[0].y - puntosControl[1].y));
    int distanciaAColision = glm::sqrt((puntosControl[0].x - esqSupIzq.x - 5) * (puntosControl[0].x - esqSupIzq.x - 5) + (puntosControl[0].y - esqSupIzq.y - 5) * (puntosControl[0].y - esqSupIzq.y - 5));

    if (colorFondo != nullptr) {
        if (distanciaAColision <= radio + 5)
            return true;
    }
    else {
        if (radio - 6 < distanciaAColision && distanciaAColision < radio + 6)
            return true;
    }

    return false;
}

int Circulo::getNumeroPuntosControl() {
    return 2;
}
Punto Circulo::getControlPoint(int i) {
    return puntosControl[i];
}

Color Circulo::getColorLinea() {
    return colorLinea;
}
Color* Circulo::getColorFondo() {
    return colorFondo;
}

InterfazDibujo& Circulo::getInterface() {
    return interfaz;
}