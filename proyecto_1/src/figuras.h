#pragma once
#include "Engine2D.h"

using namespace std;

struct Punto {
    double x;
    double y;
    void print() { cout << "(" << x << "," << y << ")" << endl; }
};

class Figura {
    protected:
        InterfazDibujo& interfaz;
        Color colorLinea;
        Color* colorFondo;
        Punto* puntosControl;
        Figura(InterfazDibujo& i) : interfaz(i), colorFondo(nullptr), puntosControl(nullptr) {}
    public:
        virtual void dibujar() {};  
        virtual ~Figura() {
            delete[] puntosControl;
            delete colorFondo;
        }
};

class Linea : public Figura {
    public:

        Linea(InterfazDibujo& i, Punto inicio, Punto final, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
};

class Rectangulo : public Figura {
    public:
        Rectangulo(InterfazDibujo& i,Punto Esq1, Punto Esq2, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        void dibujarColor();
};

class Circulo : public Figura {
    public:
        Circulo(InterfazDibujo& i, Punto centro, Punto radio, const Color& colorLinea, Color*  colorFondo);
        void dibujar() override;
        void put8Pixels(Punto punto);
};

class Elipse : public Figura {
    public:
        Elipse(InterfazDibujo& i, Punto centro, Punto eje1, Punto eje2, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        void put4Pixels(Punto punto);
};

class Triangulo : public Figura {
    public:
        Triangulo(InterfazDibujo& i, Punto punto1, Punto punto2, Punto punto3, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
};

class Bezier : public Figura {
    int nNodos;
    float dt;
    public: 
        Bezier(InterfazDibujo& i, vector<Punto> puntos, float dt, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        Punto casteljau(float t);
};