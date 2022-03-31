#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 

int main()
{
	while(1)
	{
		// if(KEY_DOWN('P'))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			Sleep(1);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			Sleep(1);
		}
	}
	return 0;
}


// 重复操作：

// #include<bits/stdc++.h>

// using namespace std;

// //#include <Winable.h>
// #include <conio.h>//can getch()
// #include<windows.h>
// //基本型态定义。支援型态定义函数。使用者界面函数 图形装置界面函数。
// #include<conio.h>    //用户通过按键盘产生的对应操作 (控制台） 
// #include<stdlib.h> 
// #include<time.h> //日期和时间头文件 

// HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// //获取标准输出的句柄 <windows.h>
// //句柄 ：标志应用程序中的不同对象和同类对象中的不同的实例 方便操控，
// void gotoxy(int x, int y)//设置光标位置
// {
//      COORD pos = {(short)(x*2),(short)y};
//     //定义一个字符在控制台屏幕上的坐标POS,COORD<wincon.h>
     
//     SetConsoleCursorPosition(hConsole, pos);
//     //定位光标位置的函数<windows.h>
 
// }

// void SetColor(int a,int b=0)//设置颜色[前景a,背景b,0~15]
//  {
//     int color=(a%=16)+(b%=16)*16;
//      SetConsoleTextAttribute(hConsole, color);
// //是API设置字体颜色和背景色的函数 格式：SetConsoleTextAttribute(句柄,颜色);
//  }
// /*
//  Color XX : 
//      Dark           Light
//     0 = 黑色       8 = 灰色
//     1 = 蓝色       9 = 淡蓝色
//     2 = 绿色       A = 淡绿色
//     3 = 湖蓝色     B = 淡浅绿色
//     4 = 红色       C = 淡红色
//     5 = 紫色       D = 淡紫色
//     6 = 黄色       E = 淡黄色
//     7 = 白色       F = 亮白色
// */

// // #define Main_ctrl VK_SCROLL
// #define Main_ctrl VK_ESCAPE

// #define N_ 10001

// POINT p_now,p_las;
// int t_las;
// double spd=1;

// #define NONE -1
// struct OPERATION
// {
//     int x,y;
//     /*
//         x>=0,y>=0   鼠标点击：(x,y)
//         x=-1,y>=0   键盘输入：KEY=y
//     */
//     int delay;
//     int contn;
//     int press;
//     int endx,endy;
//     void output()
//     {
//         printf("%d %d %c %d %d %d %d %d\n",x,y,y,delay,contn,press,endx,endy);
//     }
// };
// vector<OPERATION>p_arr;

// int main()
// {

//     Sleep(500);
//     if(GetKeyState(Main_ctrl)!=1)
//     {
//         keybd_event(Main_ctrl,0,0,0);
//         keybd_event(Main_ctrl,0,KEYEVENTF_KEYUP,0);
//     }
//     if(GetKeyState(VK_CAPITAL)!=0)
//     {
//         keybd_event(VK_CAPITAL,0,0,0);
//         keybd_event(VK_CAPITAL,0,KEYEVENTF_KEYUP,0);
//     }
//     if(GetKeyState(VK_NUMLOCK)!=1)
//     {
//         keybd_event(VK_NUMLOCK,0,0,0);
//         keybd_event(VK_NUMLOCK,0,KEYEVENTF_KEYUP,0);
//     }
//     Sleep(500);





//     puts("Ready");
//     while(GetKeyState(Main_ctrl)==1);// 摁一下ScrLK开始记录
//     puts("Recode Begin");
//     while(GetKeyState(Main_ctrl)!=0);
//     t_las=clock();
//     bool do_las,do_sth;
//     while(GetKeyState(Main_ctrl)==0)
//     {
//         // GetCursorPos(&p_now);
//         // printf("CursorPos = %d %d                            \r",p_now.x,p_now.y);
//         do_las=!do_sth;
//         do_sth=false;
//         if(do_las)
//         {
//             if(GetKeyState(VK_LBUTTON)<0) // mouse_L
//             {
//                 do_sth=true;
//                 GetCursorPos(&p_now);
//                 p_arr.push_back({p_now.x,p_now.y,clock()-t_las});
//             }
//             for(int i=2;i<=254;i++)
//             {
//                 if(GetKeyState(i)<0)
//                 {
//                     // printf("%d\n",i);
//                     do_sth=true;
//                     p_arr.push_back({NONE,i,clock()-t_las});
//                 }
//             }
//             if(do_sth)
//             {
//                 t_las=clock();
//             }
//         }
//         else
//         {
//             if(p_arr.back().x!=NONE)
//             {
//                 if(GetKeyState(VK_LBUTTON)<0) // mouse_L
//                 {
//                     do_sth=true;
//                     GetCursorPos(&p_now);
//                     p_arr.back().endx=p_now.x;
//                     p_arr.back().endy=p_now.y;
//                 }
//             }
//             else
//             {
//                 if(GetKeyState(p_arr.back().y)<0)
//                     do_sth=true;
//             }
//             if(!do_sth)
//             {
//                 p_arr.back().contn=clock()-t_las;
//                 t_las=clock();
//             }
//         }
//     }
//     if(p_arr.back().x==NONE&&p_arr.back().y==Main_ctrl)
//         p_arr.pop_back();
//     puts("Recode Over");

//     for(auto it:p_arr)
//         it.output();

//     Before_Operate_Begin:
//     if(GetKeyState(Main_ctrl)!=0)
//     {
//         keybd_event(Main_ctrl,0,0,0);
//         keybd_event(Main_ctrl,0,KEYEVENTF_KEYUP,0);
//     }
//     puts("Operate Begin");
//     while(GetKeyState(Main_ctrl)!=1)
//     {
//         if(kbhit())
//         {
//             scanf("%lf",&spd);
//         }
//     }
//     Sleep(1000);
//     while(GetKeyState(Main_ctrl)==1)
//     {
//         for(int i=30;i;i--)
//         {
//             p_las.x=NONE;
//             p_las.y=NONE;
//             for(auto it=p_arr.begin();it!=p_arr.end();it++)
//             {
//                 if(GetKeyState(Main_ctrl)!=1)
//                     goto Before_Operate_Begin;
//                 t_las=clock();
//                 if(it->x!=NONE)
//                 {
//                     printf("pressing mouseL : ( %d , %d )\n",it->x,it->y);
//                     SetCursorPos(it->x,it->y);
//                     // Sleep((it->delay/spd));
//                     if(p_las.x!=NONE)
//                     {
//                         while(clock()-t_las<(it->delay/spd))
//                         {
//                             int temp_x=p_las.x,temp_y=p_las.y;
//                             temp_x+=(it->x-p_las.x)*(clock()-t_las)/(it->delay/spd);
//                             temp_y+=(it->y-p_las.y)*(clock()-t_las)/(it->delay/spd);
//                             SetCursorPos(temp_x,temp_y);
//                         }
//                     }
//                     SetCursorPos(it->x,it->y);
//                     p_las={it->x,it->y};
                    
//                     mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
//                     Sleep(40);
//                     t_las=clock();
//                     while(clock()-t_las<(it->contn/spd))
//                     {
//                         int temp_x=p_las.x,temp_y=p_las.y;
//                         temp_x+=(it->endx-p_las.x)*(clock()-t_las)/(it->contn/spd);
//                         temp_y+=(it->endy-p_las.y)*(clock()-t_las)/(it->contn/spd);
//                         SetCursorPos(temp_x,temp_y);
//                     }
//                     Sleep(40);
//                     SetCursorPos(it->endx,it->endy);
//                     mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
//                     p_las={it->endx,it->endy};
//                 }
//                 else
//                 {
//                     printf("pressing key : %c\n",it->y);
//                     Sleep((it->delay/spd));
//                     keybd_event(it->y,0,0,0);
//                     Sleep((it->contn/spd));
//                     keybd_event(it->y,0,KEYEVENTF_KEYUP,0);
//                 }
//             }
//             Sleep(30);
//             // puts("\n\n\n");
//         }
//         goto Before_Operate_Begin;
//     }
//     return 0;
// }

// /*

// POINT p;

// Sleep(1000);

// GetCursorPos(&p);

// SetCursorPos(p.x,p.y);

// mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
// mouse_event(MOUSEEVENTF_MOVE,1,1,0,0);

// keybd_event('A',0,0,0);
// keybd_event('A',0,KEYEVENTF_KEYUP,0);

// GetKeyState('A');   0 -[donw]> -127 -[up]> 1 -[down]> -128 -[up]> 0

// */
