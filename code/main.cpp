/*
 * MAIN
 *Alejandro Menéndez Álvarez
 */

#include <basics/Director>
#include <basics/enable>
#include <basics/Graphics_Resource_Cache>
#include <basics/Texture_2D>
#include <basics/Window>
#include <basics/opengles/Context>
#include <basics/opengles/Canvas_ES2>
#include <basics/opengles/OpenGL_ES2>

//#include "Sample_Scene.hpp"
#include "Intro_Scene.hpp"
#include "Menu_Scene.hpp"



using namespace basics;
using namespace example;
using namespace std;

int main ()
{
    // Es necesario habilitar un backend gráfico antes de nada:

    enable< basics::OpenGL_ES2 > ();

    // Se crea una Game_Scene y se inicia mediante el Director:

    director.run_scene (shared_ptr< Scene >(new Intro_Scene));

    return 0;
}

// El linker tiende a eliminar código que cree que no se usa y, cuando se termina usando, lo echa en
// falta provocando errores. Dejar estas referencias aquí por el momento para evitar esos problemas:

void keep_links ()
{
    const bool &c = Window::can_be_instantiated;
    Window::Accessor window;
    Graphics_Resource_Cache cache;
    opengles::Context::create(window, &cache);
    Canvas::Factory f = opengles::Canvas_ES2::create;
}
