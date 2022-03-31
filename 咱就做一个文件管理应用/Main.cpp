#include<bits/stdc++.h>
#include<graphics.h>
// #include<windows.h>

using namespace std;

#define SCR_WIDTH      800 // 窗口宽度
#define SCR_HEIGHT     600 // 窗口高度

inline void EndProgram() {
    puts("Got to the end.");
    // getch();
    cleardevice();
    // getch();
    closegraph();
    exit(0);
}

inline void RefreshScreen() {
    
    /* init */
    setbkcolor_f(0xffffff);
    cleardevice();
    const unsigned size_r = 30;
    const unsigned size_c = 40;
    const unsigned BAR_WIDTH = SCR_WIDTH/size_c;
    const unsigned BAR_HEIGHT = SCR_HEIGHT/size_r;
    const unsigned bar_tot = size_r*size_c;
    double delfps = bar_tot;

    auto setscr_fps = [&] (double fps=60.) {
        delfps = fps;
        return ;
    };

    auto setscr_s = [&] (double scr_sec=1.) {
        delfps = bar_tot/scr_sec;
        return ;
    };

    auto setbar = [&] (int x, int y, int color) {
        setfillcolor(color);
        bar(BAR_WIDTH*x, BAR_HEIGHT*y, BAR_WIDTH*(x+1), BAR_HEIGHT*(y+1));
        return ;
    };

    setscr_fps(1);
    printf("%lf\n",fclock());
    delay_fps(delfps);
    for(int i=0; i<size_c; i++) {
        for(int j=0; j<size_r; j++) {
            // getch();
            setbar(i, j, random(0xffffff)|0x666666);
        }
    }
    delay_fps(delfps);
    printf("%lf\n",fclock());

    getch();

    api_sleep(2000);
    setscr_fps(10);
    printf("%lf\n",fclock());
    delay_fps(delfps);
    for(int i=0; i<size_c; i++) {
        for(int j=0; j<size_r; j++) {
            // getch();
            setbar(i, j, 0x777777);
        }
    }
    delay_fps(delfps);

    printf("%lf\n",fclock());
    getch();

    // // int cnt=0;
    // for(;is_run();delay_fps(30)) {
    //     ;
    // }
}



int main()
{
    /* Initialization */
    // setinitmode(INIT_NOBORDER, 0, 0);
    initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);
    setcaption("Title here ~");
    randomize();
    thread refresh_screen_thd(refresh_screen);

    ;

    refresh_screen_thd.join();
    return 0;
}