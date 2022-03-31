/*
	SPACE ����+���ײ��+���¼�ʱ
	ESC   �˳�
	WASD  �����ٶ�
	E     ����һ��֡
	�ͱ�����ཻ���
	����ǽ�ڷ���
	����ڣ�(λ��) - �ٶ� [֡��]
	��Ļ������ʾ����ײ����+��ʱ���

*/

#include<bits/stdc++.h>
#include<windows.h>
#include<graphics.h>

using namespace std;

#define SCR_WIDTH      800     // ���ڿ��
#define SCR_HEIGHT     600     // ���ڸ߶�
#define MAX_BALL       501     // �������ֵ
const float ball_op_val=0.01f; // ���ٲ���
const float sp_go_down=0.07f;  // ���ٲ���
const float MAX_SP=5.2f;       // �۵�����ٶ�

inline void MySleep(unsigned dms) {
	unsigned rec_time=clock();
	while(clock()-rec_time<dms);
	return ;
}

inline void End_of_program(){
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

inline void output_crash(){
	char str[102];
	sprintf(str,"����ײ %d ��",game_crash/10);
	// sprintf(str,"crash %d time(s)",game_crash/10);
	outtextxy(10,10,str);
	sprintf(str,"������ %.0f ��",fclock()-game_time);
	// sprintf(str,"passed %d s",(int)fclock());
	outtextxy(10,40,str);
}

inline float ball_dis(_Ball a, _Ball b){ // �����ľ�
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

inline bool ball_cross(_Ball a, _Ball b){ // �ж������ཻ
	return ball_dis(a,b)<(a.radius+b.radius);
}

inline void init_game(){
	game_crash=0;
	game_time=fclock();

	float dir = 0.0f + 1; // ��ʼ��������
	float sp = 0.5f; // ��ʼ���ٶȳ���

	for(int i = 0; i < MAX_BALL; ++i){
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
inline void player_ctrl(bool _using=true){
	if(!_using)return;
	
	key_msg m_msg;

	bool switch_state_E=false;
	bool switch_state_Q=false;

	thread getting_key([&](){
		while(is_run()){
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
		// printf("( %5.2f , %5.2f ) - %5.2f %5.2f [ %5.2f ]\r",ball[0].x,ball[0].y,ball[0].dx,ball[0].dy,getfps());
		
		// float aval=max(MAX_SP-fabs(ball[0].dx+ball[0].dy),0.07f);aval*=aval;
		float aval=(MAX_SP*MAX_SP-ball[0].dx*ball[0].dx-ball[0].dy*ball[0].dy);
		int ddx=keydown['D']-keydown['A'],ddy=keydown['S']-keydown['W'];
		ball[0].dx+=ball_op_val*aval*(ddx);
		ball[0].dy+=ball_op_val*aval*(ddy);

		if(fabs(ball[0].dx)>sp_go_down){
			ball[0].dx-=sp_go_down*(ball[0].dx>0?1:-1);
		}
		else{
			ball[0].dx=0.f;
		}
		if(fabs(ball[0].dy)>sp_go_down){
			ball[0].dy-=sp_go_down*(ball[0].dy>0?1:-1);
		}
		else{
			ball[0].dy=0.f;
		}

		if(keydown_cnt>0){
			// keystate('X') // 0/1

			// if(keydown['R']){
			// 	// operation
			// }
			if(keystate(VK_SPACE)){
				cleardevice();
				init_game();
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
		MySleep(10);
	}
	getting_key.join();
}

int n = 0; // ��ĸ���
inline void draw(){
	int cls_cnt=0x7fffffff;
	for(;is_run();delay_ms(10)){
		cleardevice();
		// if(!keydown['E']&&cls_cnt>1){
		// 	cls_cnt=0;
		// 	// ����
		// 	cleardevice();
		// 	// imagefilter_blurring(NULL,0x7f,0x99,0,0,0,0);
		// }
		// else{
		// 	cls_cnt++;
		// }

		for(int i = n-1; i >= 0; i--) { // ������ǰ��������
			// ����
			if(!keydown['E']){
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
inline void refresh(){

	for(;is_run();MySleep(10)){

		for(int _skip=keydown['E']*100+1;_skip;_skip--){

			if(keydown['E']||fclock()-t_rec>1.0){
				t_rec=fclock();
				n++;
				n%=500;
			}

			ball[0].color=0xffffff; // Ĭ����Ϊ��ɫ
			for(int i = n-1; i >= 0; i--) { // ������ǰ��������
				// �ƶ���
				ball[i].x += ball[i].dx;
				ball[i].y += ball[i].dy;
				if(ball[i].x < ball[i].radius ){ // ����߽���
					ball[i].dx = fabs(ball[i].dx)*(i?1.f:1.1f);
					ball[i].x=ball[i].radius;
				}
				if(ball[i].x > SCR_WIDTH-ball[i].radius){ // ���ұ߽���
					ball[i].dx = fabs(ball[i].dx)*(i?-1.f:-1.1f);
					ball[i].x=SCR_WIDTH-ball[i].radius;
				}
				if(ball[i].y < ball[i].radius ){ // ���ϱ߽���
					ball[i].dy = fabs(ball[i].dy)*(i?1.f:1.1f);
					ball[i].y = ball[i].radius;
				}
				if(ball[i].y > SCR_HEIGHT-ball[i].radius){ // ���±߽���
					ball[i].dy = fabs(ball[i].dy)*(i?-1.f:-1.1f);
					ball[i].y = SCR_HEIGHT-ball[i].radius;
				}
				if(i!=0&&ball_cross(ball[i],ball[0])){ // �����ཻ
					ball[0].color=0xff0000; // �۱��
					game_crash++;
				}
			}

		}
	}
	return ;
}

int main(){
	// setinitmode(INIT_NOBORDER, 100, 100);
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