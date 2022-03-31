/*
	SPASE ����+���ײ��
	ESC   �˳�
	WASD  �����ٶ�
	E     ����һ��֡
	�ͱ�����ཻ���
	����ǽ�ڷ���
	������ʱ������ڣ�(λ��) - �ٶ� [֡��]
*/

#include<bits/stdc++.h>
#include<windows.h>
#include<graphics.h>

using namespace std;

#define SCR_WIDTH      800 // ���ڿ��
#define SCR_HEIGHT     600 // ���ڸ߶�
#define MAX_BALL       501 // �������ֵ
const float ball_op_val=0.01f; // ���ٲ���
const float sp_go_down=0.1f;   // ���ٲ���
const float MAX_SP=4.1f;       // �۵�����ٶ�

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
	float x,y;     // ��ǰλ��
	float dx,dy;   // ÿ֡�ٶ�
	float radius;  // �뾶��С
	color_t color; // ��ɫ
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
		// ������ʼ����������+0.1 rad
		dir += 0.1f;
	}
	ball[0].radius = 10;
	ball[0].color = 0xffffff;

	int n = 100; // ��ĸ���
	double t_rec = fclock(); // ��¼��ǰ��������ʱ��

	
	for(;is_run();delay_fps(90)){

		for(int _skip=keydown['E']*100+1;_skip;_skip--){

			if(keydown['E']||fclock()-t_rec>1.0){
				t_rec=fclock();
				n++;
				n%=500;
			}

			// mtx.lock();
			// cleardevice(); // ����
			if(!keydown['E'])imagefilter_blurring(NULL,0x77,0x99,0,0,0,0);
			ball[0].color=0xffffff; // Ĭ����Ϊ��ɫ
			for(int i = n-1; i >= 0; i--) { // ������ǰ��������
				// ����
				if(!keydown['E'])setfillcolor(ball[i].color); // ���������ɫ
				if(!keydown['E'])setcolor(ball[i].color); // ���ñ߿���ɫ
				if(!keydown['E'])fillellipse(ball[i].x,ball[i].y,ball[i].radius,ball[i].radius);
				// �ƶ���
				ball[i].x += ball[i].dx;
				ball[i].y += ball[i].dy;
				if(ball[i].x < 0.0f || ball[i].x > SCR_WIDTH){ // �����ұ߽���
					// if(randomf()>0.5)
					   ball[i].dx = -ball[i].dx; // ����
					// else
						// ball[i].x=SCR_WIDTH-ball[i].x; // ����һ��
				}
				if(ball[i].y < 0.0f || ball[i].y > SCR_HEIGHT){ // �����±߽���
					// if(randomf()>0.5)
						ball[i].dy = -ball[i].dy; //����
					// else
						// ball[i].y=SCR_HEIGHT-ball[i].y; // ����һ��
				}
				if(i!=0&&ball_cross(ball[i],ball[0])){ // �����ཻ
					ball[0].color=0xff0000; // �۱��
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