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

