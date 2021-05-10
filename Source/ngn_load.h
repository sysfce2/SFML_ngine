/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-wip5 ***
    Funciones de carga de archivos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


    N'gine se distribuye bajo la licencia CREATIVE COMMONS
    "Attribution-NonCommercial 4.0 International"
    https://creativecommons.org/licenses/by-nc/4.0/

    You are free to:

        - Share
        copy and redistribute the material in any medium or format.
        - Adapt
        remix, transform, and build upon the material.

        The licensor cannot revoke these freedoms as long as you follow
        the license terms.

    Under the following terms:

        - Attribution
        You must give appropriate credit, provide a link to the license,
        and indicate if changes were made. You may do so in any reasonable
        manner, but not in any way that suggests the licensor endorses you
        or your use.

        - NonCommercial
        You may not use the material for commercial purposes.

        - No additional restrictions
        You may not apply legal terms or technological measures that
        legally restrict others from doing anything the license permits.

******************************************************************************/



#ifndef NGN_LOAD_H_INCLUDED
#define NGN_LOAD_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
// Includes de SDL TTF
#include <SDL_ttf.h>
// Includes de la libreria
#include "ngn_filesystem.h"


/*** Defines ***/
static const std::string MAGIC_STRING_TBG = "NGN TILEDBG";
static const std::string MAGIC_STRING_SPR = "NGN SPRITE";
static const std::string MAGIC_STRING_CMAP = "NGN COLLISION MAP";




/*** Definicion de la clase ***/
class NGN_Load {

   // Public
    public:

        // Contructor
        NGN_Load();

        // Destructor
        ~NGN_Load();

        // Carga de texturas
        NGN_TextureData* Texture(std::string filepath);

        // Carga un fondo tileado
        NGN_TiledBgData* TiledBg(std::string filepath);

        // Carga un sprite
        NGN_SpriteData* Sprite(std::string filepath);

        // Carga un mapa de colisiones
        NGN_CollisionMapData* CollisionMap(std::string filepath);

        // Carga un archivo de audio en RAM
        NGN_AudioClipData* AudioClip(std::string filepath);

        // Carga y convierte una fuente TTF al formato de la libreria
        NGN_TextFont* TrueTypeFont(
            std::string filepath,                   // Archivo a cargar
            uint32_t height,                        // Altura de la fuente (en pixeles)
            bool antialias = true,                  // Antialias?
            uint32_t font_color = 0xFFFFFF,         // Color base
            uint32_t outline = 0,                   // Borde? (en pixeles)
            uint32_t outline_color = 0x000000       // Color del borde
        );


        // Carga una imagen PNG como RAW
        NGN_RawImage* PngAsRaw(std::string filepath);

        // Carga un fotograma de un sprite como RAW
        NGN_RawImage* SpriteAsRaw(std::string filepath, uint32_t frame = 0);

        // Carga los fotogramas de un sprite como un vector de RAWs
        bool SpriteAsRawVector(
            std::string filepath,                       // Archivo a cargar
            std::vector<NGN_RawImage*> &raw_frames,     // Vector de destino con los frames
            uint32_t first_frame = 0,                   // Frame inicial (0 por defecto)
            uint32_t last_frame = NGN_DEFAULT_VALUE     // Frame final (ultimo por defecto)
        );

        // Metodo para cargar un archivo desde de el origen predeterminado, en caso de estar encriptado, se desencriptara.
        int32_t LoadFile(std::string filepath, std::vector<uint8_t> &data);

        // Establece el disco como el origen de datos
        void SetDisk();

        // Establece un archivo empaquetado como el origen de datos
        bool SetPackage(std::string pkg_file, std::string key = "");


    // Private
    private:

        // Objeto para el acceso al sistema de archivos
        NGN_FileSystem* file_system;

        // Guarda el origen de los datos (Disco o empaquetado)
        bool use_package;


        // Carga los datos de un sprite
        bool LoadSpriteData(std::string filepath, std::vector<uint8_t> &img_pixels, NGN_SpriteData* spr);


};



#endif // NGN_LOAD_H_INCLUDED
