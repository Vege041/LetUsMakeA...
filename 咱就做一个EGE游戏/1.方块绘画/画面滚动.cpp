#include<bits/stdc++.h>
#include<thread>
using namespace std;
const double Pi=acos(-1);

#include "graphics.h"

int MAX_X=1024;
int MAX_Y=640;

#define colormap_MAX_X 4007
#define colormap_MAX_Y 4007
color_t colormap[colormap_MAX_X][colormap_MAX_Y];
inline void colormap_init()
{
    for(int i=0;i<colormap_MAX_X;i++)
    {
        for(int j=0;j<colormap_MAX_Y;j++)
        {
            colormap[i][j]=(rand()%0xff<<16)+(rand()%0xff<<8)+(rand()%0xff);
            // printf("%x\n",colormap[i][j]);
        }
    }
    puts("done : colormap_init");
}

inline void colormap_show(int x=0,int y=0)
{
    if(x<0)x=0;
    if(y<0)y=0;
    if(x+MAX_X>colormap_MAX_X)x=colormap_MAX_X-MAX_X;
    if(y+MAX_Y>colormap_MAX_Y)y=colormap_MAX_Y-MAX_Y;

    for(int i=0;i<MAX_X;i++)
    {
        for(int j=0;j<MAX_Y;j++)
        {
            putpixel(i,j,colormap[x+i][y+j]);
        }
    }
}

int show_x=0,show_y=0;
inline void show_map()
{
    while(is_run())
    {
        delay_fps(60000);
        colormap_show(show_x,show_y);
    }
}

int main()
{
    initgraph(MAX_X, MAX_Y,INIT_RENDERMANUAL | INIT_NOFORCEEXIT);

    colormap_init();
    // thread getfps_thd([&](){while(1)printf("%f\t%f\n",getfps(),getfps());});
    Sleep(123);
    thread show_map_thd(show_map);
    for(int i=0;i<10000;i++)
    {
        show_x=i;
        printf("%d\n",show_x);
    }

    getch();
    cleardevice();
    getch();
    closegraph();
    return 0;
}
