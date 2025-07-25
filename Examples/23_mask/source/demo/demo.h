/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso de mascaras (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.19.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.5) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.22.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20230410)
    (c) 2005 - 2023 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "Simple mask";              // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                             // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;              // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                // Modo de pantalla en Linux
static const bool SHOW_MOUSE = false;                               // Estado del puntero del raton
static const bool BILINEAR_FILTER = false;                          // Filtrado bi-linear
static const bool VSYNC = true;                                     // Sincronismo vertical
static const bool FPS_COUNTER = false;                              // Contador de frames por segundo (solo en modo debug)



/*** Declaracion de la clase ***/
class Demo {

    public:

        // Constructor
        Demo();

        // Destructor
        ~Demo();

        // Procesos iniciales
        bool Awake();

        // Al iniciar el programa
        bool Start();

        // Nucleo de ejecucion
        uint8_t Run();


    private:

        // FLAGS de estado
        const uint8_t FLAG_ZERO = 0;
        const uint8_t FLAG_EXIT = 1;

        // Carga de archivos necesarios
        bool Load();

        // Crea los elementos
        void Create();

        // Logica del programa
        void Update();

        // Render de los elementos graficos
        void Render();

        // Textura del fondo
        NGN_TextureData* bg_data;                   // Datos de la textura
        NGN_Texture* bg;                            // Textura

        // Pelota de baloncesto
        NGN_RawImage* ball_raw;                     // Imagen en RAW
        NGN_TextureData* ball_data;                 // Datos de la textura
        NGN_Texture* ball;                          // Textura

        // Pajaro
        NGN_TextureData* bird_data;                 // Datos de la textura
        NGN_Texture* bird;                          // Textura
        std::vector<NGN_RawImage*> bird_frames;     // Vector con todos los frames del sprite en RAW
        uint8_t bird_fr;                            // Fotograma actual a mostrar
        uint8_t bird_timer;                         // Temporizador para el cambio de frames
        const uint8_t anim_delay = 5;               // Retardo entre frames

        // Textura con la mascara aplicada
        NGN_RawImage* mask_raw;                     // Imagen RAW con el resultado
        NGN_TextureData* mask_result_data;          // Datos de la textura
        NGN_Texture* mask_result;                   // Textura
        uint8_t mask_mode;                          // Modo de la mascara

};


#endif // DEMO_H_INCLUDED
