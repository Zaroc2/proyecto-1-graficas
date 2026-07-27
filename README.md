# Motor Gráfico 2D - Proyecto de Computación Gráfica

Aplicación en **C++** para dibujar, seleccionar y manipular figuras geométricas 2D. Desarrollada como proyecto para la materia *Introducción a la Computación Gráfica*, utilizando **OpenGL** y un motor propio.

---

## 📋 Descripción General

Este proyecto implementa un motor gráfico 2D desde cero, construido sobre **OpenGL 3.3** con **GLFW** para manejo de ventanas y eventos, e **ImGui** para la interfaz de usuario. Permite dibujar figuras geométricas básicas (líneas, rectángulos, círculos, elipses, triángulos) y curvas de Bézier, con soporte para selección de figuras mediante detección de colisiones.

El motor utiliza un **buffer de píxeles** para renderizar cada figura y una **textura de pantalla** que se actualiza en cada frame, siguiendo un enfoque de rasterización software.

---

## 🚀 Funcionalidades

- **Dibujo de figuras geométricas**:
  - Líneas (algoritmo de Bresenham)
  - Rectángulos
  - Círculos (algoritmo de punto medio)
  - Elipses
  - Triángulos (rasterización por barrido)
  - Curvas de Bézier
  - Interfaz de usuario con **ImGui**

- **Selección de figuras**: Detección de colisiones entre el puntero del mouse y las figuras dibujadas.

- **Interfaz de dibujo**: API virtual `InterfazDibujo` que permite desacoplar la lógica de renderizado.

---

## 🛠️ Tecnologías Utilizadas

| Tecnología | Descripción |
|------------|-------------|
| **C++20** | Lenguaje de programación principal |
| **OpenGL 3.3** | API de gráficos para renderizado |
| **GLFW** | Manejo de ventanas y eventos |
| **GLAD** | Cargador de extensiones OpenGL |
| **ImGui** | Interfaz de usuario inmediata |
| **GLM** | Matemáticas para gráficos |
| **CMake** | Sistema de construcción |

---

## ▶️ Ejecución

### Requisitos previos

- Tener instalado **CMake** (versión 3.12 o superior)
- Tener un compilador compatible con **C++20** (GCC, Clang o MSVC)
- Tener **Git** para clonar el repositorio

### Dependencias externas

El proyecto utiliza las siguientes bibliotecas, que deben estar disponibles en el sistema:

- **GLFW** (desarrollo)
- **GLAD** (incluido en el proyecto)
- **ImGui** (incluido en el proyecto)
- **GLM** (desarrollo)
- **OpenGL** (sistema)

### Pasos para compilar y ejecutar

1. **Clona el repositorio**:
   ```bash
   git clone https://github.com/Zaroc2/proyecto-1-graficas.git
   cd proyecto-1-graficas
   ```

2. **Crea un directorio de compilación**:
   ```bash
   mkdir build && cd build
   ```

3. **Configura el proyecto con CMake**:
   ```bash
   cmake ..
   ```
   *(Si estás en Windows con Visual Studio, usa `cmake .. -G "Visual Studio 17 2022"`)*

4. **Compila el proyecto**:
   ```bash
   cmake --build .
   ```

5. **Ejecuta la aplicación**:
   ```bash
   ./proyecto1
   ```
   *(En Windows, `proyecto1.exe`)*
   
---

## 📂 Estructura del Proyecto

```plaintext
proyecto-1-graficas/
├── external/                          # Bibliotecas externas (GLAD, ImGui, etc.)
│   └── ...                            # Archivos de dependencias
├── proyecto_1/                        # Código fuente principal
│   ├── src/                           # Archivos fuente
│   │   ├── engine2D.cpp               # Motor gráfico 2D (implementación)
│   │   ├── engine2D.h                 # Motor gráfico 2D (cabecera)
│   │   ├── figuras.cpp                # Implementación de figuras geométricas
│   │   ├── figuras.h                  # Definición de figuras geométricas
│   │   ├── Linea.cpp                  # Línea (algoritmo de Bresenham)
│   │   ├── Rectangulo.cpp             # Rectángulo
│   │   ├── Circulo.cpp                # Círculo (algoritmo de punto medio)
│   │   ├── Elipse.cpp                 # Elipse
│   │   ├── Triangulo.cpp              # Triángulo (rasterización)
│   │   ├── Bezier.cpp                 # Curva de Bézier
│   │   ├── FiguraSeleccionada.cpp     # Lógica de selección de figuras
│   │   └── proyecto#1.cpp             # Punto de entrada (main)
│   └── CMakeLists.txt                 # Configuración de CMake para el proyecto
├── CMakeLists.txt                     # Configuración principal de CMake
├── CMakePresets.json                  # Presets de CMake
├── .gitignore                         # Archivos ignorados por Git
└── README.md                          # Este archivo
```

> **Nota**: La carpeta `external/` contiene las bibliotecas de terceros necesarias para el proyecto. El motor `engine2D` proporciona la infraestructura de renderizado, mientras que las clases en `figuras.h` definen la jerarquía de figuras geométricas.

## 📄 Estado del Proyecto

> ✅ **Estado actual**: Proyecto funcional con motor gráfico 2D y soporte para dibujo de figuras básicas y curvas de Bézier.  
> Me gustaría terminar de implementar ciertas funcionalidades, pero quizá nunca lo haga
> Desarrollado como proyecto académico para la materia *Introducción a la Computación Gráfica*.  
> Última actividad: junio 2026 (Añadido éste readme).
