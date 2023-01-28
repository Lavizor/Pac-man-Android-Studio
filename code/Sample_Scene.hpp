/*
 * Sample Scene
 * Alejandro Menéndez Álvarez
 */

#include <memory>
#include <basics/Scene>
#include <basics/Canvas>
#include <basics/Texture_2D>
#include <basics/Vector>


using std::shared_ptr;
using namespace basics;

namespace example
{
    using basics::Vector2f;

    class Sample_Scene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;


    private:
        int             puntuacion;
        struct Coco{
            Vector2f position;
            bool visible;

            Coco();

            Coco(int x, int y){
                position={x,y};
                visible=true;
            }

            void Update(int xcoord, int ycoord){

                if(position[0] > xcoord - 10 && position[0] < xcoord + 10 && position[1] > ycoord - 10 && position[1] < ycoord + 10){
                    if(visible){
                        visible=false;
                    }
                }


            };

            void render(Canvas* canvas,Texture_2D* coco) {
                if (visible)
                    canvas->fill_rectangle({position[0]-10,position[1]-10}, {20, 20}, coco);
            };

            ~Coco(){
                //delete();

            }

        };

        //este es el fantasma
        struct Ghost{
            //tiene posicion visibilidad y velocidad
            Vector2f position;
            bool visible;
            float ghostSpeed;

            Ghost(){

            }
            //tiene un constructor vacío porque lo inicializo una vez está creada la escena
            void Initialize(int x, int y){
                position={x,y};
                visible=true;
                ghostSpeed=1.5;
            }
            //para perseguir a pacman compara su coordenada y se mueve en consecuencia
            void Update(int xcoord, int ycoord){

                if(xcoord>position[0]&&checkX()){
                    position[0]+=ghostSpeed;
                }
                if(ycoord>position[1]&&checkY()){
                    position[1]+=ghostSpeed;
                }
                if(xcoord<position[0]&&checkX()){
                    position[0]-=ghostSpeed;
                }
                if(ycoord<position[1]&&checkY()){
                    position[1]-=ghostSpeed;
                }


            };
            //los checks impiden que se salga de los carriles
            bool checkX(){
                if(((position[1]>40&&position[1]<60)||(position[1]>250&&position[1]<270)||(position[1]>440&&position[1]<460)||(position[1]>645&&position[1]<660)))
                    return true;
                else
                    return false;
            }
            bool checkY(){
                if((position[0]>265&&position[0]<285)||(position[0]>455&&position[0]<470)||(position[0]>665&&position[0]<675)||(position[0]>865&&position[0]<875))
                    return true;
                else
                    return false;
            }

            //renderiza su textura
            void render(Canvas* canvas,Texture_2D* fantasma) {
                if (visible)
                    canvas->fill_rectangle({position[0]-10,position[1]-10}, {20, 20}, fantasma);
            };

            //compruba si está tocando a pacman y de ser así "muere", y pierde
            bool death(int x, int y){
                if(position[0]>x-10&&position[0]<x+10&&position[1]>y-10&&position[1]<y+10){
                    return true;
                }
                return false;
            }

            ~Ghost(){
                //delete();

            }

        };


        //crea los cocos
          Coco         cocos[16]={
                  {265,45},
                  {455,45},
                  {665,45},
                  {865,45},
                  {265,255},
                  {455,255},
                  {665,255},
                  {865,255},
                  {265,445},
                  {455,445},
                  {665,445},
                  {865,445},
                  {265,650},
                  {455,650},
                  {665,650},
                  {865,650},
          };
    public:

        enum State
        {
            LOADING,
            RUNNING
        };

        State          state;
        bool           suspended;
        bool            inY;
        bool            inX;
        bool            end;
        bool            paused;
        bool            lose;
        int             speed;
        int             direction;
        float           timer;
        Vector2f       touch_location;
        unsigned       canvas_width;
        unsigned       canvas_height;

        Ghost ghost;
        Ghost ghost1;



        //texturas que se van a usar
        std::shared_ptr<Texture_2D> up,down,left,right;
        std::shared_ptr<Texture_2D> coco;
        std::shared_ptr<Texture_2D> fantasma;
        std::shared_ptr<Texture_2D> pacman;
        std::shared_ptr<Texture_2D> pause;
        std::shared_ptr<Texture_2D> final;
        std::shared_ptr<Texture_2D> lost;
        int          x, y;

    public:

        Sample_Scene();

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override;
        void suspend    () override;
        void resume     () override;

        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Graphics_Context::Accessor & context) override;

    private:

        void load ();
        void run  (float time);

    };

}
