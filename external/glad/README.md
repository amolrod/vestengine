# Generación de GLAD

## ¿Qué es GLAD?
GLAD es un loader de OpenGL que genera el código necesario para cargar las funciones de OpenGL en runtime.

## Cómo generar GLAD para este proyecto:

### Opción 1: Online (Recomendado para empezar)
1. Ve a https://glad.dav1d.de/
2. Configura:
   - **Language:** C/C++
   - **Specification:** OpenGL
   - **API gl:** Version 4.6
   - **Profile:** Core
   - **Extensions:** (deja las default o añade las que necesites)
   - **Options:** Marca "Generate a loader"
3. Click en "GENERATE"
4. Descarga el ZIP
5. Extrae:
   - `include/glad/glad.h` → `external/glad/include/glad/`
   - `include/KHR/khrplatform.h` → `external/glad/include/KHR/`
   - `src/glad.c` → `external/glad/src/`

### Opción 2: Usando glad Python package
```bash
pip install glad
glad --profile core --api gl=4.6 --out-path external/glad --generator c
```

### Opción 3: Para desarrollo rápido (temporal)
He incluido archivos placeholder básicos. Para producción, DEBES generar GLAD correctamente.

## Verificación
Después de generar, deberías tener:
```
external/glad/
├── include/
│   ├── glad/
│   │   └── glad.h
│   └── KHR/
│       └── khrplatform.h
└── src/
    └── glad.c
```

## Próximos pasos
Una vez generado GLAD correctamente, ejecuta:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
