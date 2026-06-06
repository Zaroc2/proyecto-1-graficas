#include "figuras.h"

int orientacion(Punto P, Punto Q, Punto R) {
    double val = (Q.x - P.x) * (R.y - P.y) - (Q.y - P.y) * (R.x - P.x);
    if (val == 0) return 0;
    return (val > 0) ? 1 : -1;
}

int segmentosIntersectan(Punto A, Punto B, Punto C, Punto D) {
    int o1 = orientacion(A, B, C);
    int o2 = orientacion(A, B, D);
    int o3 = orientacion(C, D, A);
    int o4 = orientacion(C, D, B);

    // Si tienen direcciones contrarias, estan intersectando
    if (o1 != o2 && o3 != o4) return true;
    return false;
}

Linea::Linea(InterfazDibujo& i, Punto inicio, Punto final, const Color& colorLinea, Color *colorFondo) : Figura(i){
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
    if (segmentosIntersectan(puntosControl[0],puntosControl[1],esqSupIzq, Punto(esqInfDer.x, esqSupIzq.y)) ||
        segmentosIntersectan(puntosControl[0], puntosControl[1], Punto(esqInfDer.x, esqSupIzq.y), esqInfDer) ||
        segmentosIntersectan(puntosControl[0], puntosControl[1], esqInfDer, Punto(esqSupIzq.x, esqInfDer.y)) ||
        segmentosIntersectan(puntosControl[0], puntosControl[1], Punto(esqSupIzq.x, esqInfDer.y), esqSupIzq)
        )
        return true;
    


    return false;
}



Rectangulo::Rectangulo(InterfazDibujo& i, Punto Esq1, Punto Esq2, const Color& colorLinea, Color* colorFondo) : Figura(i) {

	Punto EsqSupIzq, EsqInfDer;

    if(Esq1.x < Esq2.x) {
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

Circulo::Circulo(InterfazDibujo& i, Punto centro, Punto radio, const Color& colorLinea, Color* colorFondo) : Figura(i) {
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

    if(colorFondo != nullptr) {

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
    interfaz.putPixel(puntosControl[0].x + punto.x, puntosControl[0].y  - punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.x, puntosControl[0].y  - punto.y, colorLinea);
    interfaz.putPixel(puntosControl[0].x + punto.y, puntosControl[0].y + punto.x, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.y, puntosControl[0].y + punto.x, colorLinea);
    interfaz.putPixel(puntosControl[0].x + punto.y, puntosControl[0].y  - punto.x, colorLinea);
    interfaz.putPixel(puntosControl[0].x - punto.y, puntosControl[0].y  - punto.x, colorLinea);
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

Elipse::Elipse(InterfazDibujo& i, Punto centro, Punto eje1, Punto eje2, const Color& colorLinea, Color* colorFondo) : Figura(i) {
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

Triangulo::Triangulo(InterfazDibujo& i, Punto punto1, Punto punto2, Punto punto3,const Color& colorLinea, Color* colorFondo) : Figura(i) {
    puntosControl = new Punto[3];
    
    Punto pts[3] = { punto1, punto2, punto3};

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

Bezier::Bezier(InterfazDibujo& i, vector<Punto> puntos,float dt, const Color& colorLinea, Color* colorFondo) : Figura(i) {

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