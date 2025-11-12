/**
 * @file EntryPoint.h
 * @brief Punto de entrada principal del motor
 * 
 * Este header define la función main() para la aplicación.
 * El usuario solo necesita crear una clase que herede de Application
 * y definir la función CreateApplication().
 * 
 * Uso en tu main.cpp:
 *   #include <core/EntryPoint.h>
 *   
 *   Engine::Application* CreateApplication() {
 *       return new MyGameApp();
 *   }
 */

#pragma once

#include "core/Application.h"
#include "core/Logger.h"
#include <cstdio>  // Para fprintf en caso de error temprano

// Función que debe implementar el usuario
extern Engine::Application* CreateApplication();

/**
 * @brief Entry point principal
 * Crea la aplicación y ejecuta el game loop
 */
int main(int argc, char** argv) {
    (void)argc;  // Evitar warning de parámetro no usado
    (void)argv;
    
    // Debug temprano
    fprintf(stderr, "[DEBUG] main() - INICIO\n");
    fflush(stderr);
    
    // Crear aplicación (implementada por usuario)
    // NOTA: El logger se inicializa en el constructor de Application
    fprintf(stderr, "[DEBUG] main() - Llamando a CreateApplication()...\n");
    fflush(stderr);
    
    auto* app = CreateApplication();
    
    fprintf(stderr, "[DEBUG] main() - CreateApplication() completado\n");
    fflush(stderr);
    
    if (!app) {
        // No podemos hacer LOG aquí porque el logger aún no está inicializado
        fprintf(stderr, "ERROR: No se pudo crear la aplicación!\n");
        return -1;
    }
    
    // Ahora sí podemos hacer LOG (después de que Application inicializó el logger)
    LOG_CORE_INFO("╔════════════════════════════════════════╗");
    LOG_CORE_INFO("║   Motor Gráfico 3D - Versión 1.0.0    ║");
    LOG_CORE_INFO("║         C++20 Modular Engine           ║");
    LOG_CORE_INFO("╚════════════════════════════════════════╝");
    
    // Ejecutar game loop
    app->Run();
    
    // Limpiar
    delete app;
    
    return 0;
}
