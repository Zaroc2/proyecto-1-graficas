#include "figuras.h"

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
    int a = abs(puntosControl[1].x - xc);   // semieje X (horizontal)
    int b = abs(puntosControl[2].y - yc);   // semieje Y (vertical)

    int a2 = a * a;
    int b2 = b * b;
    int a2b2 = a2 * b2;

    int x = 0;
    int y = b;

    // int d = b * (4 * b - 4 * a * a) + a * a;
    int d = 4 * b2 + a2 * (1 - 4 * b);

    // while (b * b * 2 * (x+1) < a * a * 2 * y - 1){
    while (2 * b2 * (x + 1) < a2 * (2 * y - 1)) {
        put4Pixels(Punto(x, y));
        if (d < 0) {
			d += 4 * b2 * (2 * x + 3);
        }
        else {
			d += 4 * (b2 * (2 * x + 3) + a2 * (-2 * y + 2));            
            y--;
        }
        x++;
    }
    put4Pixels(Punto(x, y));   // ultimo punto de la region 1

    d = b2 * (4 * x * x + 4 * x + 1) + a2 * (4 * y * y - 8 * y + 4) - 4 * a2b2;

    while (y > 0) {
        put4Pixels(Punto(x, y));
        if (d < 0) {
			d += 4 * (b2 * (2 * x + 2) + a2 * (-2 * y + 3));            
            x++;
        }
        else {
            d += 4 * a2 * (-2 * y + 3);
        }
        y--;
    }
    put4Pixels(Punto(x, 0));   // ultimo punto sobre el eje X
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

    uno.dibujar();
    dos.dibujar();
    tres.dibujar();

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