#include<bits/stdc++.h>
#include<graphics.h>
// #include<windows.h>

using namespace std;


inline void End_of_program(){
    puts("Got to the end.");
    // getch();
    cleardevice();
    // getch();
    closegraph();
    exit(0);
}

bitset<256>keyDown;
int keyDown_cnt=0;
inline void player_ctrl(){
    key_msg m_msg;

    bool switch_state_E=false;
    bool switch_state_Q=false;

    thread getting_key([&](){
        while(is_run()){
            m_msg=getkey();
            switch(m_msg.msg){
                case key_msg_down:
                    keyDown_cnt+=!keyDown[m_msg.key];
                    keyDown[m_msg.key]=1;
                    if(m_msg.key=='E'){
                        switch_state_E=!switch_state_E;
                    }
                    if(m_msg.key=='Q'){
                        switch_state_Q=!switch_state_Q;
                    }
                    break;
                case key_msg_up:
                    keyDown[m_msg.key]=0;
                    break;
            }
            // printf("%d - %d\n",keyDown.size(),m_msg.msg);
        }
    });


    while(is_run()){
        if(keyDown_cnt>0){
            // keystate('X') // 0/1
            // keyDown['R']  // ===

            if(keyDown['R']){
                // operation
                puts("R");
            }
            if(keystate(VK_SPACE)){
                cleardevice();
            }
            if(keystate(VK_ESCAPE)){
                End_of_program();
            }
            if(switch_state_E){
                // E open
            }
            if(switch_state_Q){
                // Q open
            }
        }
        delay_ms(10);
    }
}
// thread player_ctrl_thd(player_ctrl);
// player_ctrl_thd.join();

int main()
{
    // setinitmode(INIT_NOBORDER, 100, 100);
    initgraph(1000, 600, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);
    setcaption("Title here ~");
    thread player_ctrl_thd(player_ctrl);

    Sleep(1000);

    int color_change;
    while(is_run()){
        for(int ii=1,i=-1;true;ii*=-1){
            if(i==-1){
                do{
                    color_change=(0x010000*(1&rand()))+(0x000100*(1&rand()))+(0x000001*(1&rand()));
                }while(color_change==0);
            }
            for(i+=ii;i>=0&&i<=0xff;i+=ii){
                initgraph(1000, 600-i*2);
                setbkcolor(i*color_change);
                Sleep(7);
            }
        }
    }


    player_ctrl_thd.join();
    return 0;
}