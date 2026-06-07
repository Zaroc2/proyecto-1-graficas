#include "figuras.h"


FiguraSeleccionada::FiguraSeleccionada(unique_ptr<Figura> f) {

	Color blanco(1.0f, 1.0f, 1.0f);

	figuraReferencia = move(f); //Pasamos la referencia del unique_ptr a figuraReferencia

	int nPtosControl = figuraReferencia->getNumeroPuntosControl();

	for (int i = 0; i < nPtosControl;i++) {
		puntosControl.push_back(make_unique<Circulo>(figuraReferencia->getInterface(), figuraReferencia->getControlPoint(i), Punto(figuraReferencia->getControlPoint(i).x + 5, figuraReferencia->getControlPoint(i).y), Color(0.f, 0.f, 0.f), nullptr));
	}

}