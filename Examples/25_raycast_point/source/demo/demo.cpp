/******************************************************************************

    N'gine Lib for C++
    Ejemplo Raycast a un punto de un sprite (Programa)

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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Elementos del fondo
    bg_data = nullptr;
    bg = nullptr;

    // Elementos del sprite
    spr_data = nullptr;
    spr = nullptr;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elementos del fondo
    delete bg; bg = nullptr;
    delete bg_data; bg_data = nullptr;

    // Elementos del sprite
    delete spr; spr = nullptr;
    delete spr_data; spr_data = nullptr;

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

    // Carga de archivos
    if (!Load()) return false;

    // Crea los elementos del programa
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
uint32_t Demo::Run() {

    // Control del loop
    uint32_t loop = FLAG_ZERO;

    // Mientras loop valga cero, repite
    while (!loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();

        // Logica del programa
        Update();
        // Render de los elementos graficos
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Salida del programa
        if (
            ngn->system->quit                                           // Si se pulsa la [X] de la ventana o ALT+F4
            ||
            ngn->input->key_ESC->down                                   // la tecla ESC
            ||
            ngn->input->controller[0].button[XBOX_BUTTON_XBOX].down     // o el boton [X] del gamepad de XBOX
        ) loop |= FLAG_EXIT;

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga de los archivos necesarios ***/
bool Demo::Load() {

    // Fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (!bg_data) return false;

    // Sprite
    spr_data = ngn->load->Sprite("data/cake.spr");
    if (!spr_data) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Sprite
    spr = new NGN_Sprite(spr_data, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

    // Parametros iniciales
    zoom_level = {1.0f, 1.0f};

}



/*** Logica del programa ***/
void Demo::Update() {

    // Modifica las propiedades del sprite
    SpriteControl();

    // Raycast test
    bool r = ngn->collisions->RaycastPoint(spr, ngn->input->mouse.x, ngn->input->mouse.y);
    if (r) {
        spr->frame = 1;
    } else {
        spr->frame = 0;
    }

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(bg);

    // Sprite
    ngn->render->Sprite(spr);

}



/*** Modifica las propiedades del sprite ***/
void Demo::SpriteControl() {

    // Mueve el sprite [Cursores]
    Vector2 spd = {0.0f, 0.0f};
    if (ngn->input->key_ARROW_RIGHT->held) spd.x += MOVE_SPEED;
    if (ngn->input->key_ARROW_LEFT->held) spd.x -= MOVE_SPEED;
    if (ngn->input->key_ARROW_DOWN->held) spd.y += MOVE_SPEED;
    if (ngn->input->key_ARROW_UP->held) spd.y -= MOVE_SPEED;
    spr->Translate(spd);
    if (spr->position.x < 0.0f) spr->position.x = 0.0f;
    if (spr->position.x > (float)SCR_WIDTH) spr->position.x = (float)SCR_WIDTH;
    if (spr->position.y < 0.0f) spr->position.y = 0.0f;
    if (spr->position.y > (float)SCR_HEIGHT) spr->position.y = (float)SCR_HEIGHT;


    // Rotacion [Z][C]
    if (ngn->input->key_C->held) spr->Rotate(ANGULAR_SPEED);
    if (ngn->input->key_Z->held) spr->Rotate(-ANGULAR_SPEED);

    // Zoom Global [Q][E]
    if (ngn->input->key_E->held) {
        zoom_level.x += ZOOM_SPEED;
        zoom_level.y += ZOOM_SPEED;
        if (zoom_level.x > MAX_SIZE) zoom_level.x = MAX_SIZE;
        if (zoom_level.y > MAX_SIZE) zoom_level.y = MAX_SIZE;
    }
    if (ngn->input->key_Q->held) {
        zoom_level.x -= ZOOM_SPEED;
        zoom_level.y -= ZOOM_SPEED;
        if (zoom_level.x < MIN_SIZE) zoom_level.x = MIN_SIZE;
        if (zoom_level.y < MIN_SIZE) zoom_level.y = MIN_SIZE;
    }

    // Zoom X [A][D]
    if (ngn->input->key_D->held) {
        zoom_level.x += ZOOM_SPEED;
        if (zoom_level.x > MAX_SIZE) zoom_level.x = MAX_SIZE;
    }
    if (ngn->input->key_A->held) {
        zoom_level.x -= ZOOM_SPEED;
        if (zoom_level.x < MIN_SIZE) zoom_level.x = MIN_SIZE;
    }

    // Zoom Y [W][S]
    if (ngn->input->key_W->held) {
        zoom_level.y += ZOOM_SPEED;
        if (zoom_level.y > MAX_SIZE) zoom_level.y = MAX_SIZE;
    }
    if (ngn->input->key_S->held) {
        zoom_level.y -= ZOOM_SPEED;
        if (zoom_level.y < MIN_SIZE) zoom_level.y = MIN_SIZE;
    }

    // Reinicia el sprite [Espacio]
    if (ngn->input->key_SPACE->down) {
        zoom_level = {1.0f, 1.0f};
        spr->Position(((float)SCR_WIDTH / 2.0f), ((float)SCR_HEIGHT / 2.0f));
        spr->rotation = 0.0f;
    }

    // Aplica la escala
    spr->Scale(zoom_level.x, zoom_level.y);

}
