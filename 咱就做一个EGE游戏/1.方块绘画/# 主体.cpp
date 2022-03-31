#include<bits/stdc++.h>
#include<thread>
using namespace std;
const double Pi=acos(-1);

#include "graphics.h"
#define EMPTY (-1)

inline void End_of_program()
{
    puts("Got to the end.");
    getch();
    cleardevice();
    getch();
    closegraph();
    exit(0);
}

int MAX_X=680;
int MAX_Y=410;

#define pxmap_MAX_X 4007
#define pxmap_MAX_Y 4007
vector< vector<color_t> >pxmap_back(pxmap_MAX_X,vector<color_t>(pxmap_MAX_Y));
vector< vector<color_t> >pxmap_frnt(pxmap_MAX_X,vector<color_t>(pxmap_MAX_Y));

inline void pxmap_init()
{
    for(int i=0;i<pxmap_MAX_X;i++)
    {
        for(int j=0;j<pxmap_MAX_Y;j++)
        {
            pxmap_back[i][j]=0;
            pxmap_frnt[i][j]=EMPTY;
        }
    }
    puts("done : pxmap_init");
}
inline void pxmap_init_random()
{
    for(int i=0;i<pxmap_MAX_X;i++)
    {
        for(int j=0;j<pxmap_MAX_Y;j++)
        {
            pxmap_back[i][j]=(rand()%0xff<<16)+(rand()%0xff<<8)+(rand()%0xff);
            pxmap_frnt[i][j]=EMPTY;
            // printf("%x\n",pxmap_back[i][j]);
        }
    }
    puts("done : pxmap_init_random");
}

int*color_player_1=NULL;
inline void pxmap_show(int x=0,int y=0)
{
    if(x<0)x=0;
    if(y<0)y=0;
    if(x+MAX_X>pxmap_MAX_X)x=pxmap_MAX_X-MAX_X;
    if(y+MAX_Y>pxmap_MAX_Y)y=pxmap_MAX_Y-MAX_Y;

    for(int i=0;i<MAX_X;i++)
    {
        for(int j=i<41?14:0;j<MAX_Y;j++)
        {
            if(pxmap_frnt[x+i][y+j]!=EMPTY)
            {
                if(getpixel(i,j)!=pxmap_frnt[x+i][y+j])
                    putpixel(i,j,pxmap_frnt[x+i][y+j]);
            }
            else
            {
                if(getpixel(i,j)!=pxmap_back[x+i][y+j])
                    putpixel(i,j,pxmap_back[x+i][y+j]);
            }

        }
    }

    for(int i=0;i<41;i++)
        for(int j=0;j<14;j++)
            putpixel(i,j,*color_player_1);
    // puts("done : pxmap_show");
}

int show_x=1000,show_y=1000;
inline void pxshow_map()
{
    delay_fps(30);
    while(is_run())
    {
        pxmap_show(show_x,show_y);
        // xyprintf(10,10,"%3.2f - fps\n",getfps());
    }
}


struct Player
{
    inline void show()
    {
        for(int i=0;i<px;i++)
        {
            pxmap_frnt[this->x+this->shape[i*2]][this->y+this->shape[i*2+1]]=
            0xffffff
            ^pxmap_back[this->x+this->shape[i*2]][this->y+this->shape[i*2+1]]
            &this->color[i];
        }
        // puts("done : player - show");
    }

    inline void show_print()
    {
        for(int i=0;i<px;i++)
        {
            pxmap_back[this->x+this->shape[i*2]][this->y+this->shape[i*2+1]]=
            this->dcolor;
            // this->color[i];
        }
        // puts("done : player - show");
    }

    inline void erase()
    {
        for(int i=0;i<px;i++)
        {
            pxmap_frnt[this->x+this->shape[i*2]][this->y+this->shape[i*2+1]]=EMPTY;
        }
        // puts("done : player - show");
    }

    inline void erase_print()
    {
        for(int i=0;i<px;i++)
        {
            pxmap_back[this->x+this->shape[i*2]][this->y+this->shape[i*2+1]]=0;
        }
        // puts("done : player - show");
    }

    inline void be_square(color_t clr=0x00ffff)
    {
        shape.clear();
        color.clear();
        for(int i=0;i>=-10;i--)
        {
            for(int j=-5;j<=5;j++)
            {
                shape.push_back(j);
                shape.push_back(i);
                color.push_back(clr);
            }
        }
        px=color.size();
    }

    inline void move(int dx,int dy)
    {
        // printf("move %d - %d\n",dx,dy);
        this->erase();
        x+=dx;
        y+=dy;
        this->show();
    }

    int x=show_x+100;
    int y=show_y+100;
    int dcolor=0x00ffff;
    int px;
    vector<int>shape;
    vector<color_t>color;
}player;


inline void player_ctrl()
{
    set<UINT>keydown;
    key_msg m_msg;

    bool print_state=false;
    bool erase_state=false;
    color_player_1=&player.dcolor;

    thread getting_key
    ([&](){
        while(is_run())
        {
            m_msg=getkey();
            switch(m_msg.msg)
            {
                case key_msg_down:
                    keydown.insert(m_msg.key);
                    if(m_msg.key=='E')
                        print_state=!print_state;
                    if(m_msg.key=='Q')
                        erase_state=!erase_state;
                    break;
                case key_msg_up:
                    keydown.erase(m_msg.key);
                    break;
            }
            // printf("%d - %d\n",keydown.size(),m_msg.msg);
        }
    });


    while(is_run())
    {
        if(keydown.size()>0)
        {
            int dx=keystate('D')-keystate('A');
            int dy=keystate('S')-keystate('W');
            player.move(dx,dy);

            show_x+=keystate('L')-keystate('J');
            show_y+=keystate('K')-keystate('I');

            if(keystate('R'))
            {
                player.dcolor=(player.dcolor&0x00ffff)|min(((player.dcolor&0xff0000)+0x010000),0xff0000);
                // printf("%8x\n",player.dcolor);
            }
            if(keystate('T'))
            {
                player.dcolor=(player.dcolor&0xff00ff)|min(((player.dcolor&0x00ff00)+0x000100),0x00ff00);
                // printf("%8x\n",player.dcolor);
            }
            if(keystate('Y'))
            {
                player.dcolor=(player.dcolor&0xffff00)|min(((player.dcolor&0x0000ff)+0x000001),0x0000ff);
                // printf("%8x\n",player.dcolor);
            }
            if(keystate('F'))
            {
                player.dcolor=(player.dcolor&0x00ffff)|max(((player.dcolor&0xff0000)-0x010000),0x010000);
                // printf("%8x\n",player.dcolor);
            }
            if(keystate('G'))
            {
                player.dcolor=(player.dcolor&0xff00ff)|max(((player.dcolor&0x00ff00)-0x000100),0x000100);
                // printf("%8x\n",player.dcolor);
            }
            if(keystate('H'))
            {
                player.dcolor=(player.dcolor&0xffff00)|max(((player.dcolor&0x0000ff)-0x000001),0x000001);
                // printf("%8x\n",player.dcolor);
            }
            if(keystate(VK_SPACE))
                cleardevice();
                // End_of_program();
            if(print_state)
                player.show_print();
            if(erase_state)
                player.erase_print();
        }
        delay_ms(10);
    }
}
// thread player_ctrl_thd(player_ctrl);

int main()
{
    initgraph(MAX_X, MAX_Y, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);

    pxmap_init_random();
    // pxmap_init();

    // thread getfps_thd([&](){while(1){printf("%3.2f - fps\n",getfps());Sleep(750);}});
    thread pxshow_map_thd(pxshow_map);
    thread player_ctrl_thd(player_ctrl);
    player.be_square();
    player.show();
    // for(int i=0;i<pxmap_MAX_X;i++)
    // {
    //     show_x=i;
    //     // printf("%d\n",show_x);
    //     // getch();
    //     delay_ms(10);
    // }

    player_ctrl_thd.join();

    End_of_program();
    return 0;
}
