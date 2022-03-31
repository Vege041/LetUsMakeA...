/*
	SPASE 清屏+清空撞击
	ESC   退出
	WASD  给咱速度
	E     跳过一堆帧
	和别的球相交变红
	碰到墙壁反弹
	（摁键时）命令窗口：(位置) - 速度 [帧数]
*/

#include<bits/stdc++.h>
#include<windows.h>
#include<graphics.h>

using namespace std;

#define SCR_WIDTH      800 // 窗口宽度
#define SCR_HEIGHT     600 // 窗口高度
#define MAX_BALL       501 // 球数最大值
const float ball_op_val=0.01f; // 加速参数
const float sp_go_down=0.1f;   // 减速参数
const float MAX_SP=4.1f;       // 咱的最大速度

inline void End_of_program(){
	puts("Got to the end.");
	// getch();
	cleardevice();
	// getch();
	closegraph();
	exit(0);
}

// mutex mtx;
struct _Ball {
	float x,y;     // 当前位置
	float dx,dy;   // 每帧速度
	float radius;  // 半径大小
	color_t color; // 颜色
};
_Ball ball[MAX_BALL]; // 创建五百个球
unsigned game_crash=0;
float game_time=0.f;

inline void output_crash(){
	char str[102];
	sprintf(str,"已碰撞 %d 次",game_crash/10);
	// sprintf(str,"crash %d time(s)",game_crash/10);
	outtextxy(10,10,str);
	sprintf(str,"已运行 %.0f 秒",fclock()-game_time);
	// sprintf(str,"passed %d s",(int)fclock());
	outtextxy(10,40,str);
}

inline float ball_dis(_Ball a, _Ball b){ // 俩球心距
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

inline bool ball_cross(_Ball a, _Ball b){ // 判断两球相交
	return ball_dis(a,b)<(a.radius+b.radius);
}

bitset<256>keydown;
int keydown_cnt=0;
inline void player_ctrl(bool _using=true){
	if(!_using)return;
	
	key_msg m_msg;

	bool switch_state_E=false;
	bool switch_state_Q=false;

	thread getting_key([&](){
		while(is_run()){
			// mtx.lock();mtx.unlock();
			m_msg=getkey();
			switch(m_msg.msg){
				case key_msg_down:
					keydown_cnt+=!keydown[m_msg.key];
					keydown[m_msg.key]=1;
					if(m_msg.key=='E'){
						switch_state_E=!switch_state_E;
					}
					if(m_msg.key=='Q'){
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


	while(is_run()){
		// mtx.lock();mtx.unlock();
		if(keydown_cnt>0){
			// keystate('X') // 0/1

			float aval=(MAX_SP*MAX_SP-ball[0].dx*ball[0].dx-ball[0].dy*ball[0].dy);
			ball[0].dx+=ball_op_val*aval*(keydown['D']-keydown['A']);
			ball[0].dy+=ball_op_val*aval*(keydown['S']-keydown['W']);
			ball[0].dx-=sp_go_down*(ball[0].dx>0?1:-1);
			ball[0].dy-=sp_go_down*(ball[0].dy>0?1:-1);
			printf("( %5.2f , %5.2f ) - %5.2f %5.2f [ %5.2f ]\r",ball[0].x,ball[0].y,ball[0].dx,ball[0].dy,getfps());
			// if(keydown['R']){
			// 	// operation
			// }
			if(keystate(VK_SPACE)){
				cleardevice();
				game_crash=0;
				game_time=fclock();
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
		api_sleep(10);
	}
	getting_key.join();
}

int main()
{
	// setinitmode(INIT_NOBORDER, 100, 100);
	initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL | INIT_NOFORCEEXIT);
	setcaption("Title here ~");
	thread player_ctrl_thd(player_ctrl,1);

	
	float dir = 0.0f + 1; // 初始化方向常数
	float sp = 0.5f; // 初始化速度常数

	for(int i = 0; i < MAX_BALL; ++i){
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
		// 迭代初始化方向常数，+0.1 rad
		dir += 0.1f;
	}
	ball[0].radius = 10;
	ball[0].color = 0xffffff;

	int n = 100; // 球的个数
	double t_rec = fclock(); // 记录当前窗口运行时间

	
	for(;is_run();delay_fps(90)){

		for(int _skip=keydown['E']*100+1;_skip;_skip--){

			if(keydown['E']||fclock()-t_rec>1.0){
				t_rec=fclock();
				n++;
				n%=500;
			}

			// mtx.lock();
			// cleardevice(); // 清屏
			if(!keydown['E'])imagefilter_blurring(NULL,0x77,0x99,0,0,0,0);
			ball[0].color=0xffffff; // 默认球为白色
			for(int i = n-1; i >= 0; i--) { // 遍历当前球数个球
				// 画球
				if(!keydown['E'])setfillcolor(ball[i].color); // 设置填充颜色
				if(!keydown['E'])setcolor(ball[i].color); // 设置边框颜色
				if(!keydown['E'])fillellipse(ball[i].x,ball[i].y,ball[i].radius,ball[i].radius);
				// 移动球
				ball[i].x += ball[i].dx;
				ball[i].y += ball[i].dy;
				if(ball[i].x < 0.0f || ball[i].x > SCR_WIDTH){ // 在左右边界外
					// if(randomf()>0.5)
					   ball[i].dx = -ball[i].dx; // 反弹
					// else
						// ball[i].x=SCR_WIDTH-ball[i].x; // 到另一侧
				}
				if(ball[i].y < 0.0f || ball[i].y > SCR_HEIGHT){ // 在上下边界外
					// if(randomf()>0.5)
						ball[i].dy = -ball[i].dy; //反弹
					// else
						// ball[i].y=SCR_HEIGHT-ball[i].y; // 到另一侧
				}
				if(i!=0&&ball_cross(ball[i],ball[0])){ // 有球相交
					ball[0].color=0xff0000; // 咱变红
					game_crash++;
				}
				// mtx.unlock();
			}
			output_crash();

		}
	}

	player_ctrl_thd.join();
	return 0;
}