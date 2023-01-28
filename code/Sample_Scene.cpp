/*
 * Sample Scene
 * Alejandro Menéndez Álvarez
 */

#include "Sample_Scene.hpp"
#include "Menu_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Translation>
#include <vector>


using namespace basics;
using namespace std;

namespace example
{

    Sample_Scene::Sample_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;

    }

    //inicializa la escena y sus variables publicas
    bool Sample_Scene::initialize ()
    {

        state     = LOADING;
        suspended = false;
        x         = 460;
        y         = 255;
        timer=2;
        speed=2;
        lose=false;
        end=false;
        inX=false;
        inY=false;
        paused=false;
        puntuacion=0;
        direction=0;

        //inicializa los fantasmas
        ghost.Initialize(270,460);
        ghost1.Initialize(865,255);

        return true;

    }

    void Sample_Scene::suspend ()
    {
        suspended = true;
    }

    void Sample_Scene::resume ()
    {
        suspended = false;
    }

    //maneja los eventos táctiles del dispositivo
    void Sample_Scene::handle (Event & event)
    {
        if (state == RUNNING&&!paused&&!end&&!lose)
        {
            switch (event.id)
            {
                case ID(touch-started):
                {
                    direction=0;
                    //toma la posicion de donde se ha tocado
                    touch_location =
                            {
                                    *event[ID(x)].as< var::Float > (),
                                    *event[ID(y)].as< var::Float > ()
                            };
                    //gestiona la x e y que manejan la posicion de pacman, encajandolo en los carriles y dependiendo de que botón de movimiento toque
                    if(touch_location.coordinates.x()>1000&&touch_location.coordinates.x()<1100&&
                       touch_location.coordinates.y()>250&&touch_location.coordinates.y()<350){
                            direction=1;

                    }
                    else if(touch_location.coordinates.x()>1000&&touch_location.coordinates.x()<1100&&
                            touch_location.coordinates.y()>50&&touch_location.coordinates.y()<150){
                            direction=2;

                    }
                    else if(touch_location.coordinates.x()>900&&touch_location.coordinates.x()<1000&&
                            touch_location.coordinates.y()>150&&touch_location.coordinates.y()<250){
                            direction=3;

                    }
                    else if(touch_location.coordinates.x()>1100&&touch_location.coordinates.x()<1200&&
                            touch_location.coordinates.y()>150&&touch_location.coordinates.y()<250){
                            direction=4;

                    }


                }
                //case ID(touch-ended):



            }
            //pone el pause
            switch(event.id)
            {
                case ID(touch-started):
                {
                    touch_location =
                            {
                                    *event[ID(x)].as<var::Float>(),
                                    *event[ID(y)].as<var::Float>()
                            };
                    if(touch_location.coordinates.x() > 50 &&
                       touch_location.coordinates.x() < 150 &&
                       touch_location.coordinates.y() > canvas_height - 200 &&
                       touch_location.coordinates.y() < canvas_height-100)
                        paused=true;
                    break;
                }
            }
        }
        //quita el pause
        else if(paused&&!end&&!lose){
            switch(event.id)
            {
                case ID(touch-started):
                {
                    touch_location =
                            {
                                    *event[ID(x)].as<var::Float>(),
                                    *event[ID(y)].as<var::Float>()
                            };
                    if(touch_location.coordinates.x() > 50 &&
                       touch_location.coordinates.x() < 150 &&
                       touch_location.coordinates.y() > canvas_height - 200 &&
                       touch_location.coordinates.y() < canvas_height-100)
                        paused=false;
                    break;
                }
            }

        }
        //permite pulsar al perder o ganar para volver a la pantalla de inicio
        else if((end||lose)&&timer<=0){
            director.run_scene (shared_ptr< Scene >(new Menu_Scene));

        }


    }

    //update que activa el codigo de su interior de manera recursiva (mientras no se haya pausado)
    void Sample_Scene::update (float time)
    {
        if(!paused){
            switch (state)
            {
                case LOADING: load ();     break;
                case RUNNING: run  (time); break;
            }


            if(direction==1&&((x>265&&x<285)||(x>455&&x<470)||(x>665&&x<675)||(x>865&&x<875))&&y<655)
                y+=speed;
            if(direction==2&&((x>265&&x<285)||(x>455&&x<470)||(x>665&&x<675)||(x>865&&x<875))&&y>45)
                y-=speed;
            if(direction==3&&((y>40&&y<60)||(y>250&&y<270)||(y>440&&y<460)||(y>645&&y<660))&&x>270)
                x-=speed;
            if(direction==4&&((y>40&&y<60)||(y>250&&y<270)||(y>440&&y<460)||(y>645&&y<660))&&x<870)
                x+=speed;

            //indicadores de disponibilidad para saber cuando estás en un carril
            if((x>265&&x<285)||(x>455&&x<470)||(x>665&&x<675)||(x>865&&x<875))
                inY=true;
            else
                inY=false;

            if((y>40&&y<60)||(y>250&&y<270)||(y>440&&y<460)||(y>645&&y<660))
                inX=true;
            else
                inX=false;

            //update de los 2 fantasmas
            ghost.Update(x,y);
            ghost1.Update(x,y);

            //chequea cada coco para ver si se ha ganado
            if(!end){
                puntuacion=0;
                for(int i=0;i<16;++i){
                    if(!cocos[i].visible){
                        puntuacion++;
                    }
                    cocos[i].Update(x,y);
                }

            }
        }

        //chequea "death" en el fantasma para ver si se ha perdido
        if(ghost.death(x,y)||ghost1.death(x,y)){
            lose=true;
        }
        //chequea la puntuacion para ver si se ha ganado
        if(puntuacion>=16){
            end=true;

        }
        //se haya ganado o perdido se acciona la pestaña de fin, que pausa e inicia un contador para no darle inmediatamente
        if(lose||end){
            paused=true;
            timer-=time;
        }
    }

    //renderiza las texturas en escena
    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended && state == RUNNING)
        {
            //se crea y actualiza el canvas
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear        ();
            }
            //mapa fondo
            canvas->set_color    (0, 0, 200);
            canvas->fill_rectangle({270, 50},{580,580});

            //carriles
            canvas->set_color    (0, 0, 0);
            canvas->fill_rectangle({440, 50},{20,580});
            canvas->fill_rectangle({650, 50},{20,580});
            canvas->fill_rectangle({270, 240},{580,20});
            canvas->fill_rectangle({270, 430},{580,20});

            //cruceta
            canvas->set_color    (1, 1, 1);
            canvas->fill_rectangle ({ 1000, 250}, { 100, 100 });
            canvas->fill_rectangle ({ 900, 150}, { 100, 100 });
            canvas->fill_rectangle ({ 1100, 150}, { 100, 100 });
            canvas->fill_rectangle ({ 1000, 50}, { 100, 100 });

            //flechas
            canvas->fill_rectangle ({ 1050, 300}, { 100, 100 }, up.get ());
            canvas->fill_rectangle ({ 950, 200}, { 100, 100 },left.get());
            canvas->fill_rectangle ({ 1150, 200}, { 100, 100 },right.get());
            canvas->fill_rectangle ({ 1050, 100}, { 100, 100 },down.get());

            //pacman
            canvas->fill_rectangle ({ x-10, y-10}, { 20, 20 }, pacman.get ());

            //fantasmas
            ghost.render(canvas, fantasma.get());
            ghost1.render(canvas, fantasma.get());

            //cada coco mientras sea visible
            if(!end){
                for(int i=0;i<16;++i){
                    cocos[i].render(canvas,coco.get());
                }
            }


            //marcadores de disponibilidad
            if(inY){
                canvas->set_color    (0, 0, 200);
                canvas->fill_rectangle ({ 50, 50}, { 100, 100 });
            }
            if(inX){
                canvas->set_color    (200, 0, 0);
                canvas->fill_rectangle ({ 50, 200}, { 100, 100 });
            }

            //botón de pause
            canvas->set_color    (0, 100, 0);
            canvas->fill_rectangle ({ 50, canvas_height-200}, { 100, 100 });
            canvas->fill_rectangle ({ 100, canvas_height-150}, { 100, 100 },pause.get());

            //renderiza la pantalla de derrota
            if(lose){
                canvas->fill_rectangle({canvas_width/2,canvas_height/2},{1280,720},lost.get());
            }

            //renderiza la pantalla de victoria
            if(end){
                canvas->fill_rectangle({canvas_width/2,canvas_height/2},{1280,720},final.get());
            }



        }
    }

    //carga las texturas desde las fotos en assets
    void Sample_Scene::load ()
    {
        //crea el contexto
        if (!suspended)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            //crea las texturas tomandolas dede assets
            if (context)
            {

                up = Texture_2D::create (ID(test), context, "up.png");
                down = Texture_2D::create (ID(test), context, "down.png");
                left = Texture_2D::create (ID(test), context, "left.png");
                right = Texture_2D::create (ID(test), context, "right.png");
                pacman = Texture_2D::create (ID(test), context, "pacman.png");
                coco = Texture_2D::create (ID(test), context, "coco.png");
                pause = Texture_2D::create (ID(test), context, "pause.png");
                final = Texture_2D::create (ID(test), context, "WIN.png");
                fantasma = Texture_2D::create (ID(test), context, "ghost.png");
                lost = Texture_2D::create (ID(test), context, "LOSE.png");

                //las añade al contexto
                if (up)
                {
                    context->add (up);
                    context->add (pacman);
                    context->add (coco);
                    context->add (pause);
                    context->add (final);
                    context->add (down);
                    context->add (left);
                    context->add (right);
                    context->add (fantasma);
                    context->add (lost);


                    state = RUNNING;
                }
            }
        }
    }

    void Sample_Scene::run (float )
    {
    }
    

}
