/*
    SPACE ����+���ײ��+���¼�ʱ
    ESC   �˳�
    WASD  �����ٶ�
    E     ����һ��֡
    �ͱ�����ཻ���
    ����ǽ�ڷ���
    ����ڣ�(λ��) - �ٶ� [֡��]
    ��Ļ������ʾ����ײ����+��ʱ���
    ���������϶�С��
*/

#include<bits/stdc++.h>
#include<windows.h>
#include<graphics.h>

using namespace std;

#define SCR_WIDTH      1536    // ���ڿ��
#define SCR_HEIGHT     864     // ���ڸ߶�
#define MAX_BALL       501     // �������ֵ
const float ball_op_val=0.007f; // ���ٲ���
const float sp_go_down=0.07f;  // ���ٲ���
const float MAX_SP=15.2f;       // �۵�����ٶ�

inline void MySleep(unsigned dms) {
    unsigned rec_time=clock();
    while(clock()-rec_time<dms);
    return ;
}

inline void End_of_program() {
    puts("Got to the end.");
    // getch();
    cleardevice();
    // getch();
    closegraph();
    exit(0);
}

struct _Ball {
    float x,y;       // ��ǰλ��
    float dx,dy;     // ÿ֡�ٶ�
    float radius;    // �뾶��С
    color_t color;   // ��ɫ
};
_Ball ball[MAX_BALL]; // ������ٸ���
unsigned game_crash=0;
float game_time=0.f;

inline void output_crash() {
    char str[102];
    sprintf(str,"����ײ %d ��",game_crash/10);
    // sprintf(str,"crash %d time(s)",game_crash/10);
    outtextxy(10,10,str);
    sprintf(str,"������ %.0f ��",fclock()-game_time);
    // sprintf(str,"passed %d s",(int)fclock());
    outtextxy(10,40,str);
}

inline float ball_dis(_Ball a, _Ball b) { // �����ľ�
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

inline bool ball_cross(_Ball a, _Ball b) { // �ж������ཻ
    return ball_dis(a,b)<(a.radius+b.radius);
}

inline void GoBall(int i) {
    // �ƶ���
    ball[i].x += ball[i].dx;
    ball[i].y += ball[i].dy;
    if(ball[i].x < ball[i].radius ) { // ����߽���
        ball[i].dx = fabs(ball[i].dx)*(i?1.f:.9f);
        ball[i].x=ball[i].radius;
    }
    if(ball[i].x > SCR_WIDTH-ball[i].radius) { // ���ұ߽���
        ball[i].dx = fabs(ball[i].dx)*(i?-1.f:-.9f);
        ball[i].x=SCR_WIDTH-ball[i].radius;
    }
    if(ball[i].y < ball[i].radius ) { // ���ϱ߽���
        ball[i].dy = fabs(ball[i].dy)*(i?1.f:.9f);
        ball[i].y = ball[i].radius;
    }
    if(ball[i].y > SCR_HEIGHT-ball[i].radius) { // ���±߽���
        ball[i].dy = fabs(ball[i].dy)*(i?-1.f:-.9f);
        ball[i].y = SCR_HEIGHT-ball[i].radius;
    }
}

inline void init_game() {
    game_crash=0;
    game_time=fclock();

    float dir = 0.0f + 1; // ��ʼ��������
    float sp = 0.5f; // ��ʼ���ٶȳ���

    for(int i = 0; i < MAX_BALL; ++i) {
        // ��ʼ����λ�þ���Ļ������
        ball[i].x = SCR_WIDTH / 2;
        ball[i].y = SCR_HEIGHT / 2;
        // ���������
        randomize();
        // ������ʼ���ٶȳ��� +0.01
        sp += 0.01f;
        // �����ٶȵ���ʼ��������
        ball[i].dx = sp * cosf(dir);
        ball[i].dy = sp * sinf(dir);
        // ������ɴ�С
        ball[i].radius = randomf() * 5 + 2;
        ball[i].color = random(0xff0000) + 0xffff;
        // // ������ʼ����������+0.1 rad
        // dir += 0.1f;
        dir = randomf()*3.15;
    }
    ball[0].radius = 10;
    ball[0].x = 100;
    ball[0].y = 100;
    ball[0].color = 0xffffff;
    return ;
}

bitset<256>keydown;
int keydown_cnt=0;
inline void player_ctrl(bool _using=true) {
    if(!_using)return;
    
    key_msg m_msg;
    mouse_msg m_mmsg;

    bool switch_state_E=false;
    bool switch_state_Q=false;
    bool switch_mouse_left=false;
    bool switch_mouse_right=false;
    bool switch_mouse_middle=false;

    thread getting_key([&]() {
        while(is_run()) {
            m_msg=getkey();
            switch(m_msg.msg) {
                case key_msg_down:
                    keydown_cnt+=!keydown[m_msg.key];
                    keydown[m_msg.key]=1;
                    if(m_msg.key=='E') {
                        switch_state_E=!switch_state_E;
                    }
                    if(m_msg.key=='Q') {
                        switch_state_Q=!switch_state_Q;
                    }
                    break;
                case key_msg_up:
                    keydown[m_msg.key]=0;
                    keydown_cnt--;
                    break;
            }
            // printf("%d - %d\n",keydown.size(),m_msg.msg);
        }
    });


    thread getting_mouse([&]() {
        while(is_run()) {
            // putchar(0);
            m_mmsg=getmouse();
            // printf("%d %d %d %d %d\n", m_mmsg.msg, m_mmsg.x, m_mmsg.y, m_mmsg.flags, m_mmsg.wheel);
            // printf("\t\t\t\t%d %d %d %d %d %d %d\n",m_mmsg.is_move(),m_mmsg.is_down(),m_mmsg.is_up(),m_mmsg.is_left(),m_mmsg.is_mid(),m_mmsg.is_right(),m_mmsg.is_wheel());
            // printf("%d\n",switch_mouse_left);
            switch(m_mmsg.msg) {
                case 16:
                    switch(m_mmsg.flags) {
                        case 1:
                            switch_mouse_left=true;
                            break;
                        case 2:
                            switch_mouse_right=true;
                            break;
                        case 4:
                            switch_mouse_middle=true;
                            break;
                    }
                    break;
                case 32:
                    switch(m_mmsg.flags) {
                        case 1:
                            switch_mouse_left=false;
                            break;
                        case 2:
                            switch_mouse_right=false;
                            break;
                        case 4:
                            switch_mouse_middle=false;
                            break;
                    }
                    break;
            }
        }
    });

    auto minn = [] (float b ,float a=MAX_SP/2 ) {
        return fabs(a)<fabs(b)?a*b/fabs(b):b;
    };

    while(is_run()) {
        // printf("( %5.2f , %5.2f ) - %5.2f %5.2f [ %5.2f ]\r",ball[0].x,ball[0].y,ball[0].dx,ball[0].dy,getfps());
        
        if(switch_mouse_left) {
            ball[0].dx=minn(m_mmsg.x-ball[0].x);
            ball[0].dy=minn(m_mmsg.y-ball[0].y);
            ball[0].x=m_mmsg.x;
            ball[0].y=m_mmsg.y;
        }
        else {
            // float aval=max(MAX_SP-fabs(ball[0].dx+ball[0].dy),0.07f);aval*=aval;
            float aval=(MAX_SP*MAX_SP-ball[0].dx*ball[0].dx-ball[0].dy*ball[0].dy);
            float ddx=keydown['D']-keydown['A'],ddy=keydown['S']-keydown['W']; // ������+.4
            ball[0].dx+=ball_op_val*aval*(ddx);
            ball[0].dy+=ball_op_val*aval*(ddy);
            GoBall(0);
        }

        if(fabs(ball[0].dx)>sp_go_down) {
            ball[0].dx-=sp_go_down*(ball[0].dx>0?1:-1);
        }
        else{
            ball[0].dx=0.f;
        }
        if(fabs(ball[0].dy)>sp_go_down) {
            ball[0].dy-=sp_go_down*(ball[0].dy>0?1:-1);
        }
        else{
            ball[0].dy=0.f;
        }

        if(keydown_cnt>0) {
            // keystate('X') // 0/1

            // if(keydown['R']) {
            //  // operation
            // }
            if(keystate(VK_SPACE)) {
                cleardevice();
                init_game();
            }
            if(keystate(VK_ESCAPE)) {
                End_of_program();
            }
            if(switch_state_E) {
                // E open
            }
            if(switch_state_Q) {
                // Q open
            }
        }
        MySleep(20);
    }
    getting_key.join();
}

/* ���ͼ�� */
int n = 0; // ��ĸ���
inline void draw() {
    int cls_cnt=0x7fffffff;
    for(;is_run();delay_ms(10)) { // ԭ delay_fps(90)
        cleardevice();
        // if(!keydown['E']&&cls_cnt>1) {
        //  cls_cnt=0;
        //  // ����
        //  cleardevice();
        //  // imagefilter_blurring(NULL,0x7f,0x99,0,0,0,0);
        // }
        // else{
        //  cls_cnt++;
        // }

        for(int i = n-1; i >= 0; i--) { // ������ǰ��������
            // ����
            if(!keydown['E']) {
                setfillcolor(ball[i].color); // ���������ɫ
                setcolor(ball[i].color); // ���ñ߿���ɫ
                fillellipse(ball[i].x,ball[i].y,ball[i].radius,ball[i].radius);
            }
        }
        output_crash();
    }
    return ;
}

double t_rec=0; // ��¼��ǰ��������ʱ��
inline void refresh() {

    for(;is_run();MySleep(10)) {

        for(int _skip=keydown['E']*100+1;_skip;_skip--) {

            if(keydown['E']||fclock()-t_rec>1.0) {
                t_rec=fclock();
                n++;
                n%=500;
            }

            ball[0].color=0xffffff; // Ĭ����Ϊ��ɫ
            for(int i = n-1; i > 0; i--) { // ������ǰ��������
                GoBall(i);
                if(i!=0&&ball_cross(ball[i],ball[0])) { // �����ཻ
                    ball[0].color=0xff0000; // �۱��
                    game_crash++;
                }
            }

        }
    }
    return ;
}

int main() {
    setinitmode(INIT_NOBORDER, 0, 0);
    initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);
    setcaption("Title here ~");
    thread player_ctrl_thd(player_ctrl,1);

    
    init_game();


    thread draw_thd(draw);
    // draw_thd.detach();
    thread refresh_thd(refresh);
    // refresh_thd.detach();

    

    player_ctrl_thd.join();
    draw_thd.join();
    refresh_thd.join();
    return 0;
}