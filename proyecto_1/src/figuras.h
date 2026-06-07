#pragma once
#include "Engine2D.h"

using namespace std;

struct Punto {
    double x;
    double y;
    void print() { cout << "(" << x << "," << y << ")" << endl; }
};

int orientacion(Punto P, Punto Q, Punto R);
int segmentosIntersectan(Punto A, Punto B, Punto C, Punto D);

enum Tipo {tipoLinea, tipoRectangulo, tipoCirculo, tipoElipse, tipoTriangulo, tipoBezier};

class Figura {
    protected:
        InterfazDibujo& interfaz;
        Color colorLinea;
        Color* colorFondo;
        Punto* puntosControl;
        Figura(InterfazDibujo& i) : interfaz(i), colorFondo(nullptr), puntosControl(nullptr) {}
    public:
        Tipo tipo;
        virtual void dibujar() {};  
        virtual bool colisiona(Punto esqSupIzq, Punto esqSupDer) { return false; };
        virtual ~Figura() {
            delete[] puntosControl;
            delete colorFondo;
        }
        virtual int getNumeroPuntosControl() = 0;
        virtual Punto getControlPoint(int i) = 0;
        virtual Color getColorLinea() = 0;
        virtual Color* getColorFondo() = 0;
        virtual InterfazDibujo& getInterface() = 0;
};


class Linea : public Figura {
    public:

        Linea(InterfazDibujo& i, Punto inicio, Punto final, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        bool colisiona(Punto esqSupIzq, Punto esqInfDer) override;
        int getNumeroPuntosControl() override;
        Punto getControlPoint(int i) override;
        Color getColorLinea() override;
        Color* getColorFondo() override;
        InterfazDibujo& getInterface() override;
};

class Rectangulo : public Figura {
    public:
        Rectangulo(InterfazDibujo& i,Punto Esq1, Punto Esq2, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        bool colisiona(Punto esqSupIzq, Punto esqInfDer) override;
        void dibujarColor();
        int getNumeroPuntosControl() override;
        Punto getControlPoint(int i) override;
        Color getColorLinea() override;
        Color* getColorFondo() override;
        InterfazDibujo& getInterface() override;

};

class Circulo : public Figura {
    public:
        Circulo(InterfazDibujo& i, Punto centro, Punto radio, const Color& colorLinea, Color*  colorFondo);
        void dibujar() override;
        bool colisiona(Punto esqSupIzq, Punto esqInfDer) override;
        void put8Pixels(Punto punto);
        int getNumeroPuntosControl() override;
        Punto getControlPoint(int i) override;
        Color getColorLinea() override;
        Color* getColorFondo() override;
        InterfazDibujo& getInterface() override;

};

class Elipse : public Figura {
    public:
        Elipse(InterfazDibujo& i, Punto centro, Punto eje1, Punto eje2, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        bool colisiona(Punto esqSupIzq, Punto esqInfDer) override;
        void put4Pixels(Punto punto);
        int getNumeroPuntosControl() override;
        Punto getControlPoint(int i) override;
        Color getColorLinea() override;
        Color* getColorFondo() override;
        InterfazDibujo& getInterface() override;

};

class Triangulo : public Figura {
    public:
        Triangulo(InterfazDibujo& i, Punto punto1, Punto punto2, Punto punto3, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        void dibujarRelleno();
        bool colisiona(Punto esqSupIzq, Punto esqInfDer) override;
        int getNumeroPuntosControl() override;
        Punto getControlPoint(int i) override;
        Color getColorLinea() override;
        Color* getColorFondo() override;
        InterfazDibujo& getInterface() override;

};

class Bezier : public Figura {
    int nNodos;
    float dt;
    public: 
        Bezier(InterfazDibujo& i, vector<Punto> puntos, float dt, const Color& colorLinea, Color* colorFondo);
        void dibujar() override;
        Punto casteljau(float t);
        bool colisiona(Punto esqSupIzq, Punto esqInfDer) override;
        int getNumeroPuntosControl() override;
        Punto getControlPoint(int i) override;
        Color getColorLinea() override;
        Color* getColorFondo() override;
        InterfazDibujo& getInterface() override;

};

class FiguraSeleccionada {
    unique_ptr<Figura> figuraReferencia;
    vector<unique_ptr<Circulo>> puntosControl;

    public:
        FiguraSeleccionada(unique_ptr<Figura> f);
        void dibujar();
        void colisiona();

};