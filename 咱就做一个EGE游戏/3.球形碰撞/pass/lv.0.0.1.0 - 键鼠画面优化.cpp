/*
    SPACE 清屏+清空撞击+重新计时
    ESC   退出
    WASD  给咱速度
    E     跳过一堆帧
    和别的球相交变红
    碰到墙壁反弹
    命令窗口：(位置) - 速度 [帧数]
    屏幕左上显示：碰撞次数+计时情况
    鼠标左键：拖动小球
*/

#include<bits/stdc++.h>
#include<windows.h>
#include<graphics.h>

using namespace std;

#define SCR_WIDTH      1536    // 窗口宽度
#define SCR_HEIGHT     864     // 窗口高度
#define MAX_BALL       501     // 球数最大值
const float ball_op_val=0.007f; // 加速参数
const float sp_go_down=0.07f;  // 减速参数
const float MAX_SP=15.2f;       // 咱的最大速度

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
    float x,y;       // 当前位置
    float dx,dy;     // 每帧速度
    float radius;    // 半径大小
    color_t color;   // 颜色
};
_Ball ball[MAX_BALL]; // 创建五百个球
unsigned game_crash=0;
float game_time=0.f;

inline void output_crash() {
    char str[102];
    sprintf(str,"已碰撞 %d 次",game_crash/10);
    // sprintf(str,"crash %d time(s)",game_crash/10);
    outtextxy(10,10,str);
    sprintf(str,"已运行 %.0f 秒",fclock()-game_time);
    // sprintf(str,"passed %d s",(int)fclock());
    outtextxy(10,40,str);
}

inline float ball_dis(_Ball a, _Ball b) { // 俩球心距
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

inline bool ball_cross(_Ball a, _Ball b) { // 判断两球相交
    return ball_dis(a,b)<(a.radius+b.radius);
}

inline void GoBall(int i) {
    // 移动球
    ball[i].x += ball[i].dx;
    ball[i].y += ball[i].dy;
    if(ball[i].x < ball[i].radius ) { // 在左边界外
        ball[i].dx = fabs(ball[i].dx)*(i?1.f:.9f);
        ball[i].x=ball[i].radius;
    }
    if(ball[i].x > SCR_WIDTH-ball[i].radius) { // 在右边界外
        ball[i].dx = fabs(ball[i].dx)*(i?-1.f:-.9f);
        ball[i].x=SCR_WIDTH-ball[i].radius;
    }
    if(ball[i].y < ball[i].radius ) { // 在上边界外
        ball[i].dy = fabs(ball[i].dy)*(i?1.f:.9f);
        ball[i].y = ball[i].radius;
    }
    if(ball[i].y > SCR_HEIGHT-ball[i].radius) { // 在下边界外
        ball[i].dy = fabs(ball[i].dy)*(i?-1.f:-.9f);
        ball[i].y = SCR_HEIGHT-ball[i].radius;
    }
}

inline void init_game() {
    game_crash=0;
    game_time=fclock();

    float dir = 0.0f + 1; // 初始化方向常数
    float sp = 0.5f; // 初始化速度常数

    for(int i = 0; i < MAX_BALL; ++i) {
        // 初始化球位置居屏幕正中心
        ball[i].x = SCR_WIDTH / 2;
        ball[i].y = SCR_HEIGHT / 2;
        // 生成随机数
        randomize();
        // 迭代初始化速度常数 +0.01
        sp += 0.01f;
        // 分配速度到初始化方向上
        ball[i].dx = sp * cosf(dir);
        ball[i].dy = sp * sinf(dir);
        // 随机生成大小
        ball[i].radius = randomf() * 5 + 2;
        ball[i].color = random(0xff0000) + 0xffff;
        // // 迭代初始化方向常数，+0.1 rad
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
            float ddx=keydown['D']-keydown['A'],ddy=keydown['S']-keydown['W']; // 加重力+.4
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

/* 输出图像 */
int n = 0; // 球的个数
inline void draw() {
    int cls_cnt=0x7fffffff;
    for(;is_run();delay_ms(10)) { // 原 delay_fps(90)
        cleardevice();
        // if(!keydown['E']&&cls_cnt>1) {
        //  cls_cnt=0;
        //  // 清屏
        //  cleardevice();
        //  // imagefilter_blurring(NULL,0x7f,0x99,0,0,0,0);
        // }
        // else{
        //  cls_cnt++;
        // }

        for(int i = n-1; i >= 0; i--) { // 遍历当前球数个球
            // 画球
            if(!keydown['E']) {
                setfillcolor(ball[i].color); // 设置填充颜色
                setcolor(ball[i].color); // 设置边框颜色
                fillellipse(ball[i].x,ball[i].y,ball[i].radius,ball[i].radius);
            }
        }
        output_crash();
    }
    return ;
}

double t_rec=0; // 记录当前窗口运行时间
inline void refresh() {

    for(;is_run();MySleep(10)) {

        for(int _skip=keydown['E']*100+1;_skip;_skip--) {

            if(keydown['E']||fclock()-t_rec>1.0) {
                t_rec=fclock();
                n++;
                n%=500;
            }

            ball[0].color=0xffffff; // 默认球为白色
            for(int i = n-1; i > 0; i--) { // 遍历当前球数个球
                GoBall(i);
                if(i!=0&&ball_cross(ball[i],ball[0])) { // 有球相交
                    ball[0].color=0xff0000; // 咱变红
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