/******************************************************************************

    Ejemplo del uso de multiples colisionadores en un Sprite

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.19.0-stable o superior

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.5) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.22.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20190615)
    (c) 2005 - 2019 by Lode Vandevenne
    http://lodev.org/lodepng/


	N'gine Lib is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal	in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER	LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes de la demo
#include "demo/demo.h"



/*** Main ***/
int main(int argc, char* args[]) {

    // Resultado
    int r = 0x00;

    // Crea el objeto de la libreria
    ngn = nullptr;
    ngn = new NGN();

    // Crea el objecto de la demo
    Demo* demo = nullptr;
    demo = new Demo();

    // Intenta iniciar N'gine para ejecutar el programa
    if (demo->Awake()) {
        // Intenta inicializar el programa
        if (demo->Start()) {
            // Si se ha iniciado correctamente, ejecutalo.
            demo->Run();
        } else {
            // Error al inicializar el juego
            ngn->log->Message("Game start-up failed!");
            r = 0x02;
        }
        #if defined (MODE_DEBUG)
            ngn->log->Message("Execution terminated.");
        #endif
    } else {
        // Error al inicializar la libreria
        std::cout << "N'GINE boot-up failed!" << std::endl;
        r = 0x01;
    }

    // Elimina el objecto demo
    delete demo; demo = nullptr;

    // Elimina el objeto de la libreria
    delete ngn; ngn = nullptr;

    // Devuelve el resultado de la ejecucion
    return r;

}
