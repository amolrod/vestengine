/**
 * @file Texture.h
 * @brief Sistema de texturas 2D con stb_image
 * 
 * Soporta carga de PNG, JPG, BMP, TGA, PSD, HDR
 * Configuración de wrapping, filtering, mipmaps
 */

#pragma once

#include <cstdint>
#include <string>

namespace Engine {

/**
 * @enum TextureFormat
 * @brief Formatos internos de textura
 */
enum class TextureFormat {
    None = 0,
    RGB8,      // 3 canales, 8 bits por canal
    RGBA8,     // 4 canales, 8 bits por canal
    RED,       // 1 canal (escala de grises)
    RG         // 2 canales
};

/**
 * @enum TextureWrap
 * @brief Modos de wrapping (repetición)
 */
enum class TextureWrap {
    Repeat = 0,          // GL_REPEAT (default)
    MirroredRepeat,      // GL_MIRRORED_REPEAT
    ClampToEdge,         // GL_CLAMP_TO_EDGE
    ClampToBorder        // GL_CLAMP_TO_BORDER
};

/**
 * @enum TextureFilter
 * @brief Modos de filtrado
 */
enum class TextureFilter {
    Nearest = 0,         // GL_NEAREST (pixelated)
    Linear,              // GL_LINEAR (smooth)
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear   // GL_LINEAR_MIPMAP_LINEAR (trilinear)
};

/**
 * @struct TextureSpecification
 * @brief Especificación de configuración de textura
 */
struct TextureSpecification {
    uint32_t width = 1;
    uint32_t height = 1;
    TextureFormat format = TextureFormat::RGBA8;
    TextureWrap wrapS = TextureWrap::Repeat;      // Horizontal
    TextureWrap wrapT = TextureWrap::Repeat;      // Vertical
    TextureFilter minFilter = TextureFilter::Linear;
    TextureFilter magFilter = TextureFilter::Linear;
    bool generateMipmaps = true;
    bool flipVertically = true;  // stb_image carga al revés por defecto
};

/**
 * @class Texture2D
 * @brief Textura 2D con carga desde archivo
 * 
 * Patrón: RAII, Resource Handle
 * Responsabilidad: Gestionar textura GPU y su configuración
 * 
 * Uso:
 *   auto texture = Texture2D::Create("assets/textures/brick.png");
 *   texture->Bind(0);  // Texture unit 0
 *   // ... render ...
 */
class Texture2D {
public:
    /**
     * @brief Destructor - libera recursos GPU
     */
    virtual ~Texture2D();
    
    /**
     * @brief Activa esta textura en un texture unit
     * @param slot Texture unit (0-31 típicamente)
     */
    virtual void Bind(uint32_t slot = 0) const;
    
    /**
     * @brief Desactiva esta textura
     */
    virtual void Unbind() const;
    
    /**
     * @brief Obtiene el ancho en píxeles
     */
    virtual uint32_t GetWidth() const { return m_width; }
    
    /**
     * @brief Obtiene el alto en píxeles
     */
    virtual uint32_t GetHeight() const { return m_height; }
    
    /**
     * @brief Obtiene el ID de textura OpenGL
     */
    virtual uint32_t GetRendererID() const { return m_rendererID; }
    
    /**
     * @brief Obtiene el path del archivo
     */
    virtual const std::string& GetPath() const { return m_path; }
    
    /**
     * @brief Verifica si la textura se cargó correctamente
     */
    virtual bool IsLoaded() const { return m_isLoaded; }
    
    /**
     * @brief Factory method - Carga desde archivo
     * @param path Ruta al archivo de imagen
     * @param spec Especificación de configuración (opcional)
     */
    static Texture2D* Create(const std::string& path, 
                             const TextureSpecification& spec = TextureSpecification());
    
    /**
     * @brief Factory method - Crea textura desde datos raw
     * @param width Ancho en píxeles
     * @param height Alto en píxeles
     * @param data Puntero a datos RGBA
     * @param spec Especificación de configuración (opcional)
     */
    static Texture2D* Create(uint32_t width, uint32_t height, 
                             void* data, 
                             const TextureSpecification& spec = TextureSpecification());
    
    /**
     * @brief Constructor público para std::make_shared
     */
    Texture2D(const std::string& path, const TextureSpecification& spec);
    
    /**
     * @brief Constructor desde datos raw
     */
    Texture2D(uint32_t width, uint32_t height, void* data, 
              const TextureSpecification& spec);

private:
    /**
     * @brief Configura los parámetros OpenGL de la textura
     */
    void SetTextureParameters(const TextureSpecification& spec);

private:
    uint32_t m_rendererID = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::string m_path;
    bool m_isLoaded = false;
    TextureFormat m_format = TextureFormat::RGBA8;
};

} // namespace Engine
