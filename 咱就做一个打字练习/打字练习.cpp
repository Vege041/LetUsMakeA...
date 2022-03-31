#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
inline void set_position(int x=0, int y=0){
SetConsoleCursorPosition(hConsole,COORD{(short)(x),(short)(y)});}
inline void set_color(int a=15,int b=0){
SetConsoleTextAttribute(hConsole,(a%=16)+(b%=16)*16);}

enum Color_
{
	Color_black, Color_blue, Color_green, Color_lake_blue,
	Color_red, Color_puerple, Color_yellow, Color_white,
	Color_gray, Color_light_blue, Color_light_green, Color_cyon,
	Color_light_red, Color_mauve, Color_light_yellow, Color_bright_white
};

inline void putch(char ch,int a=15,int b=0)
{
	set_color(a,b);
	putchar(ch);
	return;
}

unsigned cnt_stay=1;
inline void loop()
{
	unsigned i=15;
	char ch;

	string str="";
	srand(time(0));
	while(i--)
	{
		ch=rand()%28;
		str.push_back(ch<26?ch+'a':' ');
	}
	while(str.back()==' ')
	{
		str.pop_back();
	}
	// string str="咱就是说这是我们的目标内容。";

	puts(str.c_str());
	vector< pair<char,char> > cnt_wrong;
	unsigned time_start=clock();
	i=0;
	while(i<str.size())
	{
		ch=getch();
		set_position(i,cnt_stay);

		if(ch==str[i])
		{
			putch(str[i]);
			i++;
		}
		else
		{
			putch(ch,Color_light_yellow,Color_red);
			cnt_wrong.push_back({str[i],ch});
		}
	}

	putchar('\n');
	putchar('\n');
	set_color(Color_light_green);
	printf("Totle time use = %lf s\n",(clock()-time_start)/1000.);
	set_color(Color_mauve);
	printf("Totle chars in = %llu\n",str.size());
	set_color(Color_light_red);
	printf("Totle WRONG in = %llu\n",cnt_wrong.size());
	for(auto it:cnt_wrong)
	{
		putchar('\t');
		putch(it.first,Color_light_yellow);
		putch(' ');
		putch('-');
		putch('>');
		putch(' ');
		putch(it.second,Color_cyon);
		putchar('\n');
	}
	set_color();
	cnt_stay+=7+cnt_wrong.size();
	puts("-----------------------------------");

	return;
}

int main()
{
	while(true)
	{
		loop();
	}
	return 0;
}