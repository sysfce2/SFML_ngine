/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Camara virtual 2D

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.6.2-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.8) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
    http://lodev.org/lodepng/


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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"

/*** Defines ***/
static const std::string WINDOW_TITLE = "N'GINE Sprite Animation & Render to Texture.";
static const uint32_t SCR_WIDTH = 1280;
static const uint32_t SCR_HEIGHT = 720;



/*** Constructor de la clase ***/
Demo::Demo() {

    // Punteros a NULL
    man_gfx = NULL;
    man[0].sprite = NULL;
    man[1].sprite = NULL;
    bg_gfx = NULL;
    bg = NULL;
    render_texture = NULL;


}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Destruye los sprites
    delete man[0].sprite; man[0].sprite = NULL;
    delete man[1].sprite; man[1].sprite = NULL;
    delete man_gfx; man_gfx = NULL;

    // Destruye el fondo
    delete bg; bg = NULL;
    delete bg_gfx; bg_gfx = NULL;

    // Destruye el render
    delete render_texture; render_texture = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Selecciona el modo grafico
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT)) return false;

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(false);

    // Debug?
    ngn->system->fps_counter = false;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Caga de archivos
    if (!Load()) return false;

    // Crea la escena
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
void Demo::Run() {

    // Control del loop
    bool loop = true;

    // Loop principal
    while (loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();             // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Render de la escena
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        loop &= !ngn->system->quit;             // Si se pulsa la [X] de la ventana
        loop &= !ngn->input->key_ESC->down;     // O se pulsa la tecla [ESC] sal del bucle de ejecucion

    }

}



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Carga el fondo
    bg_gfx = ngn->load->TiledBg("data/running_man_bg.tbg");
    if (!bg_gfx) return false;

    // Carga los sprites
    man_gfx = ngn->load->Sprite("data/man_running.spr");
    if (!man_gfx) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::Create() {

    // Crea la textura del render
    render_texture = new NGN_Texture(SCR_WIDTH, SCR_HEIGHT, 0, 0);
    scale = 1.0f;
    rotation = 0.0f;

    // Crea el fondo
    bg = new NGN_TiledBg(bg_gfx, 0, 0);

    // Crea el sprite
    man[0].sprite = new NGN_Sprite(man_gfx);
    man[0].position = {640.0f, 532.0f};
    man[1].sprite = new NGN_Sprite(man_gfx);
    man[1].position = {640.0f, 180.0f};

    // Crea las animaciones
    man[0].sprite->AddAnimation("idle", 0, 0, 0, 0);      // Idle
    man[0].sprite->AddAnimation("fwd", 1, 8, 1, 4);       // Adelante
    man[0].sprite->AddAnimation("bck", 8, 1, 8, 8);       // Atras
    man[0].sprite->SetAnimation("idle");                  // Selecciona la animacion por defecto
    man[1].sprite->AddAnimation("idle", 0, 0, 0, 0);      // Idle
    man[1].sprite->AddAnimation("fwd", 1, 8, 1, 4);       // Adelante
    man[1].sprite->AddAnimation("bck", 1, 8, 1, 8);       // Atras
    man[1].sprite->SetAnimation("idle");                  // Selecciona la animacion por defecto

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Movimieto del personaje
    if (ngn->input->key_ARROW_RIGHT->held && (man[0].position.x < 1120.0f)) {
        man[0].position.x += man[0].speed[0];
        man[0].sprite->SetAnimation("fwd");
        man[1].position.x += man[1].speed[0];
        man[1].sprite->SetAnimation("fwd");
        man[1].sprite->flip_h = false;
    } else if (ngn->input->key_ARROW_LEFT->held && (man[0].position.x > 192.0f)) {
        man[0].position.x += man[0].speed[1];
        man[0].sprite->SetAnimation("bck");
        man[1].position.x += man[1].speed[1];
        man[1].sprite->SetAnimation("bck");
        man[1].sprite->flip_h = true;
    } else {
        man[0].sprite->SetAnimation("idle");
        man[1].sprite->SetAnimation("idle");
    }

    // Aplicalo
    man[0].sprite->Position(man[0].position);
    man[0].sprite->PlayAnimation();
    man[1].sprite->Position(man[1].position);
    man[1].sprite->PlayAnimation();

    // Zoom de camara
    if (ngn->input->key_ARROW_UP->held) {
        scale += 0.01f;
        if (scale > 1.5f) scale = 1.5f;
    }
    if (ngn->input->key_ARROW_DOWN->held) {
        scale -= 0.01f;
        if (scale < 1.0f) scale = 1.0f;
    }

    // Rotacion de la camara
    if (ngn->input->key_Q->held) {
        rotation -= 0.3f;
        if (rotation < -15.0f) rotation = -15.0f;
    }
    if (ngn->input->key_W->held) {
        rotation += 0.3f;
        if (rotation > 15.0f) rotation = 15.0f;
    }

    // Calcula el zoom escalando la textura
    render_texture->Scale(scale);
    float x = (SCR_WIDTH / 2) - (((float)(SCR_WIDTH / 2)) * scale);
    float y = (SCR_HEIGHT / 2) - (((float)(SCR_HEIGHT / 2)) * scale);
    render_texture->Position(x, y);
    render_texture->rotation = rotation;

}



/*** Render de los graficos ***/
void Demo::Render() {

    // Renderiza a una textura
    ngn->render->RenderToTexture(render_texture);
    ngn->render->TiledBg(bg);
    ngn->render->Sprite(man[0].sprite);
    ngn->render->Sprite(man[1].sprite);

    // Renderiza la textura en la pantalla
    ngn->render->RenderToScreen();
    ngn->render->Texture(render_texture);

}