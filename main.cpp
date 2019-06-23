#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace sf;
using namespace std;
/*
odleglosc pomiedzy punktami A i B
r = sqrt((BX - AX)*(BX - AX) + (BY - AY)*(BY - AY));
*/

int main()
{
    srand(time(NULL));
    double oknoX = 1200; //szerokosc okna
    double oknoY = 800; //wysokosc okna
    RenderWindow okno(VideoMode(oknoX,oknoY),"Dyski");
    Time t = milliseconds(1);
    int n = 25; //ilosc dyskow
    double L = 1; //lepkosc
    const double Pi = 3.14159265358979323846;
    const double G = 25;//6.67 * pow(10,-11);
    const double M = 50;
    double dt = 0.001;
    double r[n];
    Clock c;
    Time elapsed = c.restart();

    CircleShape Dyski[n];
    RectangleShape ciecz(Vector2f(250,350));
    ciecz.setFillColor(Color(191,138,24));
    ciecz.setPosition (50,50);
    Vector2f dyskP[n];
    Vector2f nowaP[n];
    FloatRect dyskiBox[n];
    FloatRect cieczBox = ciecz.getGlobalBounds();

    Vector2f V[n]; // predkosc dyskow
    double masa[n]; // masa dyskow
    double dyskR[n]; // rozmiar dyskow
    Vector2f k[n];
    Vector2f W[n];
    Vector2f F[n];
    double F2[n];

    for(int i=0;i<n;i++)
    {
        masa[i] = (rand()%71)+30; // masa dyskow
        dyskR[i] = (rand()%11)+15; // rozmiar dyskow od 15 do 25
        V[i].x = rand()%401+(-200); // predkosc wzdluz x od -300 do 300
        V[i].y = rand()%401+(-200); // predkosc wzdluz y od -300 do 300
    }

    for(int i=0;i<n;i++)
    {
        Dyski[i].setFillColor(Color((rand()%255)+1,(rand()%255)+1,(rand()%255)+1));
        Dyski[i].setRadius(dyskR[i]);
        Dyski[i].setPosition(rand()%300+800,rand()%100+100);
    }

    while(okno.isOpen())
    {
        Vector2i myszP = Mouse::getPosition(okno);
        Event zdarzenie;
        while(okno.pollEvent(zdarzenie))
        {
            if(zdarzenie.type == Event::Closed)
                 okno.close();
        }
        if(Keyboard::isKeyPressed(Keyboard::Escape))
            okno.close();

        elapsed += c.restart();
        while(elapsed >=t)
        {
            for(int i=0;i<n;i++)
            {
                dyskiBox[i] = Dyski[i].getGlobalBounds();
                dyskP[i] = Dyski[i].getPosition();

                // grawitacja
                W[i].x = ( myszP.x - dyskP[i].x ) / abs( myszP.x - dyskP[i].x );
                W[i].y = ( myszP.y - dyskP[i].y ) / abs( myszP.y - dyskP[i].y );

                F[i].x = ( G * ( masa[i] * M ) / ( myszP.x - dyskP[i].x ) * ( myszP.x - dyskP[i].x ) ) * W[i].x;
                F[i].y = ( G * ( masa[i] * M ) / ( myszP.y - dyskP[i].y ) * ( myszP.y - dyskP[i].y ) ) * W[i].y;

                k[i].x = -6*Pi*L*dyskR[i]*V[i].x;
                k[i].y = -6*Pi*L*dyskR[i]*V[i].y;

                if(dyskiBox[i].intersects(cieczBox))
                {
                    F[i].x = F[i].x + k[i].x;
                    F[i].y = F[i].x + k[i].y;
                }

                V[i].x = V[i].x + ( F[i].x / masa[i] ) * dt;
                V[i].y = V[i].y + ( F[i].y / masa[i] ) * dt;

                nowaP[i].x = dyskP[i].x + ( V[i].x * dt );
                nowaP[i].y = dyskP[i].y + ( V[i].y * dt );

                if ((nowaP[i].x - dyskR[i]) < -dyskR[i]) //lewa krawedz okna
                {
                    V[i].x *= -1;
                    nowaP[i].x = -dyskR[i] + dyskR[i];
                }
                else if ((nowaP[i].x + dyskR[i]) >= (oknoX-dyskR[i])) // prawa krawedz okna
                {
                    V[i].x *= -1;
                    nowaP[i].x = (oknoX-dyskR[i]) - dyskR[i];
                }
                else if ((nowaP[i].y - dyskR[i]) < -dyskR[i]) //gorna krawedz okna
                {
                    V[i].y *= -1;
                    nowaP[i].y = -dyskR[i] + dyskR[i];
                }
                else if ((nowaP[i].y + dyskR[i]) >= (oknoY-dyskR[i])) //dolna krawedz okna
                {
                    V[i].y *= -1;
                    nowaP[i].y = (oknoY-dyskR[i]) - dyskR[i];
                }

                Dyski[i].setPosition(nowaP[i]);
            }
            elapsed -= t;
        }

        okno.clear(Color(200,200,200));
        okno.draw(ciecz);
        for(int i=0;i<n;i++)
            okno.draw(Dyski[i]);
        okno.display();
    }
}

