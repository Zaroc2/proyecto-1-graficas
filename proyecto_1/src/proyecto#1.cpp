#include "Engine2D.h"
#include "figuras.h"
#include <iostream>

enum Estado {Idle,DibujandoLinea,DibujandoRectangulo,DibujandoCirculo,DibujandoElipse,DibujandoTriangulo,DibujandoCurva};

class proyecto1 : public Engine2D {
private:
    Color colorFondo;
    Color colorPincel;
    Color colorFigura;

    vector<Punto> puntos;
    vector<unique_ptr<Figura>> Figuras;
	unique_ptr<Figura> FiguraTemporal;

    int estado;
    float dt;
    bool rellenarFigura;

public:
    proyecto1(): Engine2D(1024, 600, "Proyecto #1 - Gestion y Despliegue de Primitivas"), estado(Idle) {}
    void setup() override {

        colorFondo = Color(0.1f, 0.1f, 0.15f);
        colorPincel = Color(1.0f, 1.0f, 0.0f);
        colorFigura = Color(0.0f, 0.0f, 0.0f);
        clear(colorFondo);

        FiguraTemporal = nullptr;
        estado = Idle;
        dt = 0.1;
        rellenarFigura = false;


        std::cout << "Motor inicializado exitosamente." << std::endl;
    }
    // Eventos
    void onkeyDown(int key) override {
        /*
        if (key == GLFW_KEY_SPACE) {
            clear(colorFondo);
        }
        */

        switch (key) {

            case GLFW_KEY_0:
                estado = Idle;
                if (puntos.size() != 0) {
                    puntos.clear();
                }
                break;
            case GLFW_KEY_1:
                if (puntos.size() > 2) {
                    cout << "No se puede cambiar a linea" << endl;
                    return;
                }
                estado = DibujandoLinea;
                break;
            case GLFW_KEY_2:
                if (puntos.size() > 2) {
                    cout << "No se puede cambiar a rectangulo" << endl;
                    return;
                }
                estado = DibujandoRectangulo;
                break;
            case GLFW_KEY_3:
                if (puntos.size() > 2) {
                    cout << "No se puede cambiar a circulo" << endl;
                    return;
                }
                estado = DibujandoCirculo;
                break;
            case GLFW_KEY_4:
                if (puntos.size() > 3) {
                    cout << "No se puede cambiar a elipse" << endl;
                    return;
                }
                estado = DibujandoElipse;
                break;
            case GLFW_KEY_5:
                if (puntos.size() > 3) {
                    cout << "No se puede cambiar a triangulo" << endl;
                    return;
                }
                estado = DibujandoTriangulo;
                break;
            case GLFW_KEY_6:
                estado = DibujandoCurva;
                break;
            case GLFW_KEY_C:
                if (rellenarFigura) {
                    rellenarFigura = false;
                }
                else {
                    rellenarFigura = true;
                }
                break;
            case GLFW_KEY_RIGHT:
                    if(!(dt > 1.0f)) {
                        dt += 0.01f;
					}
					break;
            case GLFW_KEY_LEFT:
                if(!(dt < 0.02f)) {
                    dt -= 0.01f;
                }
                break;

        }
    }
    void onMouseButtonDown(int button, double x, double y) override {

        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        switch (estado) {
            case DibujandoLinea:
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    addPunto();
                    if (puntos.size() == 2) {
                        addFigura();
                    }
                }
                break;
            case DibujandoRectangulo:
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    addPunto();
                    if (puntos.size() == 2) {
                        addFigura();
                    }
                }
                break;
            case DibujandoCirculo:
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
					addPunto();
                    if (puntos.size() == 2) {
                        addFigura();
                    }
                }
                break;
            case DibujandoElipse:
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    addPunto();
                    if (puntos.size() == 3) {
                        addFigura();
                    }
                }
                break;
            case DibujandoTriangulo:
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    addPunto();
                    if (puntos.size() == 3) {
                        addFigura();
                    }
                }
                break;
            case DibujandoCurva:
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    addPunto();
                }
                else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                    addPunto();
                    addFigura();
                }
                break;


        }

    }
    void onMouseButtonUp(int button, double x, double y) override {
        /*
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            dibujando = false;
        }
        */
    }
    // Evento de movimiento continuo
    void onMouseMove(double x, double y) override {

        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        switch (estado) {
            case DibujandoLinea:
                if (puntos.size() == 1) {
                    Punto aux;
                    glfwGetCursorPos(glfwGetCurrentContext(), &aux.x, &aux.y);
					FiguraTemporal = make_unique<Linea>(*this, puntos.at(0), aux, colorPincel, &colorFigura);

                }
                break;
            case DibujandoRectangulo:
                if (puntos.size() == 1) {
                    Punto aux;
                    glfwGetCursorPos(glfwGetCurrentContext(), &aux.x, &aux.y);
                    
                    if (rellenarFigura) {
						FiguraTemporal = make_unique<Rectangulo>(*this, puntos.at(0), aux, colorPincel, &colorFigura);
                    }
                    else {
						FiguraTemporal = make_unique<Rectangulo>(*this, puntos.at(0), aux, colorPincel, nullptr);
                    }

                }
                break;
            case DibujandoCirculo:
                if (puntos.size() == 1) {
                    Punto aux;
                    glfwGetCursorPos(glfwGetCurrentContext(), &aux.x, &aux.y);
                    if (rellenarFigura) {
						FiguraTemporal = make_unique<Circulo>(*this, puntos.at(0), aux, colorPincel, &colorFigura);
                    }
                    else {
						FiguraTemporal = make_unique<Circulo>(*this, puntos.at(0), aux, colorPincel, nullptr);
                    }

                }
                break;
            case DibujandoElipse:
                
                if (puntos.size() == 1) {
                    double auxY;
                    glfwGetCursorPos(glfwGetCurrentContext(), nullptr, &auxY);
                    if (rellenarFigura) {
						FiguraTemporal = make_unique<Elipse>(*this, puntos.at(0), Punto(puntos.at(0).x, auxY), Punto(puntos.at(0).x, auxY), colorPincel, &colorFigura);
                    }
                    else {
						FiguraTemporal = make_unique<Elipse>(*this, puntos.at(0), Punto(puntos.at(0).x, auxY), Punto(puntos.at(0).x, auxY), colorPincel, nullptr);
                    }
                }
                else if (puntos.size() == 2) {
                    double auxX;
                    glfwGetCursorPos(glfwGetCurrentContext(), &auxX, nullptr);
                    if (rellenarFigura) {
						FiguraTemporal = make_unique<Elipse>(*this, puntos.at(0), puntos.at(1), Punto(auxX, puntos.at(0).y), colorPincel, &colorFigura);
                    }
                    else {
						FiguraTemporal = make_unique<Elipse>(*this, puntos.at(0), puntos.at(1), Punto(auxX, puntos.at(0).y), colorPincel, nullptr);
                    }

                }
                break;
            case DibujandoTriangulo:
                if (puntos.size() == 1) {
                    Punto aux;
                    glfwGetCursorPos(glfwGetCurrentContext(), &aux.x, &aux.y);
                    if (rellenarFigura) {
						FiguraTemporal = make_unique<Linea>(*this, puntos.at(0), aux, colorPincel, &colorFigura);
                    }
                    else {
						FiguraTemporal = make_unique<Linea>(*this, puntos.at(0), aux, colorPincel, nullptr);
                    }
                }
                else if (puntos.size() == 2) {
                    Punto aux;
                    glfwGetCursorPos(glfwGetCurrentContext(), &aux.x, &aux.y);
                    if (rellenarFigura) {
						FiguraTemporal = make_unique<Triangulo>(*this, puntos.at(0), puntos.at(1), aux,colorPincel, &colorFigura);
                    }
                    else {
						FiguraTemporal = make_unique<Triangulo>(*this, puntos.at(0), puntos.at(1), aux, colorPincel, nullptr);
                    }
                }
                break;
            case DibujandoCurva:
                if (puntos.size() >= 1) {
                    vector<Punto> auxVector = puntos;
                    Punto auxPoint;
                    glfwGetCursorPos(glfwGetCurrentContext(), &auxPoint.x, &auxPoint.y);
                    auxVector.push_back(auxPoint);
					FiguraTemporal = make_unique<Bezier>(*this, auxVector, dt, colorPincel, &colorFigura);
                }
        }
    }
    void update(float deltaTime) override {
        // Aquí irían cosas que cambian automáticamente con el tiempo

        if (FiguraTemporal != nullptr) {
            clear(colorFondo);    
            for (int i = 0; i < Figuras.size(); i++) {
                Figuras.at(i)->dibujar();
            }

            FiguraTemporal->dibujar();
            FiguraTemporal = nullptr;
        }
        
    }
    void drawUI() override {

        ImGui::SetNextWindowPos(ImVec2(12, 12), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_Always);
        ImGui::Begin("##panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        // Título
        ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "Herramientas");
        ImGui::Separator();

        auto modoButton = [&](const char* label, Estado nuevoEstado) {
            bool activo = (estado == nuevoEstado);
            if (activo) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
            }
            if (ImGui::Button(label)) {
                estado = nuevoEstado;
                puntos.clear();          // Limpiar puntos pendientes al cambiar de modo
            }
            if (activo) {
                ImGui::PopStyleColor(2);
            }
            };

        modoButton("Linea", DibujandoLinea);
        ImGui::SameLine();
        modoButton("Rectangulo", DibujandoRectangulo);
        ImGui::SameLine();
        modoButton("Circulo", DibujandoCirculo);
        ImGui::SameLine();
        modoButton("Elipse", DibujandoElipse);
        ImGui::SameLine();
        modoButton("Triangulo", DibujandoTriangulo);

        // Selectores sin pestañas
        float pencilCol[3] = { colorPincel.r, colorPincel.g, colorPincel.b };
        if (ImGui::ColorEdit3("Color Borde", pencilCol)) {
            colorPincel.r = pencilCol[0];
            colorPincel.g = pencilCol[1];
            colorPincel.b = pencilCol[2];
        }

        float fillCol[3] = { colorFigura.r, colorFigura.g, colorFigura.b };
        if (ImGui::ColorEdit3("Color Relleno", fillCol)) {
            colorFigura.r = fillCol[0];
            colorFigura.g = fillCol[1];
            colorFigura.b = fillCol[2];
        }

        ImGui::Checkbox("Rellenar figuras", &rellenarFigura);

        float bgCol[3] = { colorFondo.r, colorFondo.g, colorFondo.b };
        if (ImGui::ColorEdit3("Color Fondo", bgCol)) {
            colorFondo.r = bgCol[0];
            colorFondo.g = bgCol[1];
            colorFondo.b = bgCol[2];
            clear(colorFondo);
        }

        ImGui::Separator();
        ImGui::SliderFloat("Paso Bezier (dt)", &dt, 0.01f, 0.2f);

        ImGui::Separator();
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        ImGui::End();
    }

    void addFigura(){

        switch (estado) {
            case DibujandoLinea:
                Figuras.push_back(make_unique<Linea>(*this, puntos.at(0), puntos.at(1), colorPincel,&colorFigura));
                break;
            case DibujandoRectangulo:
                if (rellenarFigura) {
                    Figuras.push_back(make_unique<Rectangulo>(*this, puntos.at(0), puntos.at(1), colorPincel, &colorFigura));
                }
                else {
                    Figuras.push_back(make_unique<Rectangulo>(*this, puntos.at(0), puntos.at(1), colorPincel, nullptr));
                }
                break;
            case DibujandoCirculo:
                if (rellenarFigura) {
                    Figuras.push_back(make_unique<Circulo>(*this, puntos.at(0), puntos.at(1), colorPincel, &colorFigura));
                }
                else {
                    Figuras.push_back(make_unique<Circulo>(*this, puntos.at(0), puntos.at(1), colorPincel, nullptr));
                }
                break;
            case DibujandoElipse:
                if (rellenarFigura) {
                    Figuras.push_back(make_unique<Elipse>(*this, puntos.at(0), puntos.at(1), puntos.at(2), colorPincel, &colorFigura));
                }
                else {
                    Figuras.push_back(make_unique<Elipse>(*this, puntos.at(0), puntos.at(1), puntos.at(2), colorPincel, nullptr));
                }
                break;
            case DibujandoTriangulo:
                if (rellenarFigura) {
                    Figuras.push_back(make_unique<Triangulo>(*this, puntos.at(0), puntos.at(1), puntos.at(2), colorPincel, &colorFigura));
                }
                else {
                    Figuras.push_back(make_unique<Triangulo>(*this, puntos.at(0), puntos.at(1), puntos.at(2), colorPincel, nullptr));
                }
                break;
            case DibujandoCurva:
                Figuras.push_back(make_unique<Bezier>(*this, puntos, dt, colorPincel, &colorFigura));
                break;
        }

        puntos.clear();
    }

    void addPunto() {
        Punto aux;
        glfwGetCursorPos(glfwGetCurrentContext(), &aux.x, &aux.y);
        puntos.push_back(aux);
	}
};

int main() {

    proyecto1 app;
    app.run();

    return 0;
}