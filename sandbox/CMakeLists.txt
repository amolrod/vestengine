# =============================================================================
# Sandbox - Aplicación de Prueba del Motor
# =============================================================================

# Archivos del sandbox
add_executable(Sandbox
    main.cpp
)

# Incluir headers del motor
target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Linkear con el motor
target_link_libraries(Sandbox PRIVATE
    MotorGrafico
)

# Aplicar warnings estrictos
set_project_warnings(Sandbox)

# Configurar como aplicación de consola o GUI según plataforma
if(WIN32)
    # En Windows, evitar consola en Release
    set_target_properties(Sandbox PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()

# Copiar shaders al directorio de build automáticamente
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Sandbox>/shaders
    COMMENT "Copiando shaders al directorio de ejecución..."
)

message(STATUS "Sandbox application configured")
