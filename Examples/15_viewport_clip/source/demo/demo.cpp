/******************************************************************************

    Ejemplo de uso del recorte de area del viewport
    Archivo de Demo (Programa)

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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <vector>
#include <ctime>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = nullptr;
    ball_data = nullptr;

    // Inicializa los punteros de objetos
    bg = nullptr;

    // Inicializa los vectores
    ball.clear();

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    for (uint32_t i = 0; i < ball.capacity(); i ++) {
        delete ball[i].gfx; ball[i].gfx = nullptr;
    }
    ball.clear();
    delete bg; bg = nullptr;

    // Borra los datos
    delete ball_data; ball_data = nullptr;
    delete bg_data; bg_data = nullptr;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Habilita el archivo de registro
    ngn->log->OpenLogFile("debug_log.txt");

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // Visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    #if defined (OS_WINDOWS)
        ngn->graphics->SetMode(SCR_MODE_WINDOWS);
    #elif defined (OS_LINUX)
        ngn->graphics->SetMode(SCR_MODE_LINUX);
    #endif
    ngn->graphics->Update();

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        if(!ngn->load->SetPackage("data.pkg", "0123456789ABCDEF")) return false;
    #endif

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message("Versions check:\n" + ngn->system->GetVersion());
		ngn->log->Message("N'gine started successfully!");
	#endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Cambia la semilla del random
    srand(time(nullptr));

    // Carga los archivos
    if (!Load()) return false;

    // Crea la textura con los datos cargados
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea los objetos mobiles
    CreateObjects();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
int8_t Demo::Run() {

    // Control del loop
    int8_t loop = -1;

    while (loop < 0) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        if (ngn->system->quit) {    // Si se pulsa la [X] de la ventana
            loop = 0;
        } else if (ngn->input->key_ESC->down || ngn->input->controller[0].button[10].down) {    // Si se pulsa la tecla [ESC] O se pulsa el boton XBOX
            loop = 1;
        }

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga todos los archivos necesarios ***/
bool Demo::Load() {

    // Carga la imagen para el fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == nullptr) return false;

    // Carga la imagen para los objetos
    ball_data = ngn->load->Texture("data/basketball.png");
    if (ball_data == nullptr) return false;

    // Carga correcta
    return true;

}



/*** Crea los objetos ***/
void Demo::CreateObjects() {

    // Variables
    uint32_t min_x, max_x, min_y, max_y, _size, x, y;

    // Crea una lista con el numero de elementos necesarios
    ball.clear();
    ball.reserve(OBJECT_NUMBER);

    // Inicializa la lista
    for (uint32_t i = 0; i < ball.capacity(); i ++) {
        // Crea la textura
        ball[i].gfx = new NGN_Texture(ball_data);
        // Tamaño aleatorio
        _size = ((rand() % 96) + 32);
        ball[i].gfx->Size(_size, _size);
        // Posicion aleatoria (esquina superior izquierda)
        min_x = min_y = 0;
        max_x = (ngn->graphics->native_w - _size);
        max_y = (ngn->graphics->native_h - _size);
        x = (rand() % max_x);
        y = (rand() % max_y);
        ball[i].gfx->Position(x, y);
        // Limites del movimiento para este objeto
        ball[i].top = min_y;
        ball[i].left = min_x;
        ball[i].bottom = max_y;
        ball[i].right = max_x;
        // Velocidades aleatorias
        ball[i].speed.x = (((float)(rand() % 100) / 10.0f) + 1.0f);
        if ((rand() % 100) > 50) ball[i].speed.x = -ball[i].speed.x;
        ball[i].speed.y = (((float)(rand() % 100) / 10.0f) + 1.0f);
        if ((rand() % 100) > 50) ball[i].speed.y = -ball[i].speed.y;
    }

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve los objetos
    Move();
    // Recorte de la pantalla
    Clip();
    // Render de la escena
    Render();

}



/*** Mueve los objetos ***/
void Demo::Move() {

    for (uint32_t i = 0; i < ball.capacity(); i ++) {

        // Mueve los objetos
        ball[i].gfx->Translate(ball[i].speed.x, ball[i].speed.y);

        // Limites
        if (ball[i].gfx->position.x < ball[i].left) {
            ball[i].gfx->position.x = ball[i].left;
            ball[i].speed.x = -ball[i].speed.x;
        }
        if (ball[i].gfx->position.x > ball[i].right) {
            ball[i].gfx->position.x = ball[i].right;
            ball[i].speed.x = -ball[i].speed.x;
        }
        if (ball[i].gfx->position.y < ball[i].top) {
            ball[i].gfx->position.y = ball[i].top;
            ball[i].speed.y = -ball[i].speed.y;
        }
        if (ball[i].gfx->position.y > ball[i].bottom) {
            ball[i].gfx->position.y = ball[i].bottom;
            ball[i].speed.y = -ball[i].speed.y;
        }

        // Rotalos
        ball[i].gfx->Rotate(ball[i].speed.x);

    }

}



/*** Recorte de la pantalla con el mouse ***/
void Demo::Clip() {

    int x, y, w, h;
    int _x = std::abs(((int32_t)SCR_WIDTH / 2) - ngn->input->mouse.x);
    int _y = std::abs(((int32_t)SCR_HEIGHT / 2) - ngn->input->mouse.y);
    x = ((SCR_WIDTH / 2) - _x);
    y = ((SCR_HEIGHT / 2) - _y);
    w = (_x * 2);
    h = (_y * 2);
    ngn->graphics->SetViewportClip(x, y, w, h);

}



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);

    // Renderiza los objetos mobiles
    for (uint32_t i = 0; i < ball.capacity(); i ++) {
        ngn->render->Texture(ball[i].gfx);
    }

}
