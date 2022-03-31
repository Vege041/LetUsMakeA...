#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
inline void set_color(int a=15,int b=0){
SetConsoleTextAttribute(hConsole,(a%=16)+(b%=16)*16);}
inline void str2upper(string&op)
{
	for(unsigned i=op.size()-1;~i;i--){
		op[i]=toupper(op[i]);
	}
	return ;
}
enum Color_
{
	Color_black, Color_blue, Color_green, Color_lake_blue,
	Color_red, Color_puerple, Color_yellow, Color_white,
	Color_gray, Color_light_blue, Color_light_green, Color_cyon,
	Color_light_red, Color_mauve, Color_light_yellow, Color_bright_white
};
#define Color_DF Color_white // default
#define Color_OK Color_green
#define Color_ER Color_light_red



// 用于存储当前时间
short cur_week;   // week of the year(0~53)
short cur_day;    // day of the week(0~6,Sunday=0)
short cur_year;   // year
short cur_month;  // month(01~12)
short cur_date;   // day of the month(01~12)
short cur_hour;   // hour(00~23)
short cur_minute; // minute(00-59)
short cur_second; // second(00-61)

// 初始化并且获取当前时间
inline void init_get_current_time()
{
	// get current time
	time_t cur_time_t;
	time(&cur_time_t);
	tm *cur_tm;
	cur_tm=localtime(&cur_time_t);

	// cur_day=cur_tm->tcur_wday;
	// cur_year=cur_tm->tcur_year;
	// cur_month=cur_tm->mon+1;
	// cur_date=cur_tm->...;
	// cur_hour=cur_tm;
	// cur_minute=cur_tm;
	// cur_second=cur_tm;

	// to string then to integer
	char cur_time_str[102];
	strftime(cur_time_str,102,"%W",cur_tm); // week of the year(0~53)
	cur_week=atoi(cur_time_str);
	strftime(cur_time_str,102,"%w",cur_tm); // day of the week(0~6,Sunday=0)
	cur_day=atoi(cur_time_str);
	strftime(cur_time_str,102,"%Y",cur_tm); // year
	cur_year=atoi(cur_time_str);
	strftime(cur_time_str,102,"%m",cur_tm); // month(01~12)
	cur_month=atoi(cur_time_str);
	strftime(cur_time_str,102,"%d",cur_tm); // day of the month(01~12)
	cur_date=atoi(cur_time_str);
	strftime(cur_time_str,102,"%H",cur_tm); // hour(00~23)
	cur_hour=atoi(cur_time_str);
	strftime(cur_time_str,102,"%M",cur_tm); // minute(00-59)
	cur_minute=atoi(cur_time_str);
	strftime(cur_time_str,102,"%S",cur_tm); // second(00-61)
	cur_second=atoi(cur_time_str);

	// // try to confirm
	// printf("cur_week    = %4d\n",cur_week);
	// printf("cur_day     = %4d\n",cur_day);
	// printf("cur_year    = %4d\n",cur_year);
	// printf("cur_month   = %4d\n",cur_month);
	// printf("cur_date    = %4d\n",cur_date);
	// printf("cur_hour    = %4d\n",cur_hour);
	// printf("cur_minute  = %4d\n",cur_minute);
	// printf("cur_second  = %4d\n",cur_second);
	
	return ;
}

struct _item
{
	string name;
	short week;
	short day;
	short year;
	short month;
	short date;
	short hour;
	short minute;
	short second;
};

#define OPCODE_NONE	0
#define OPCODE_ADD	1
map<string,int>map_opcode={
	{"NONE",OPCODE_NONE},
	{"ADD",OPCODE_ADD}
};
map<string,_item>items;
inline void init_deal_schedule()
{
	ifstream schedule_file("schedule.txt",ios::in);
	char str_line[410];
	string op;
	while(!schedule_file.eof())
	{
		schedule_file>>op;
		str2upper(op);
		// cout<<op<<endl;
		schedule_file.getline(str_line,410);
		// cout<<str_line<<endl;

		switch(map_opcode[op])
		{
			case OPCODE_ADD:
				set_color(Color_OK);
				puts("READ add");
				break;
			case OPCODE_NONE:
				set_color(Color_OK);
				puts("READ none");
				break;
		}
		
	}
	return ;
}



int main()
{
	// system("chcp 936 & cls");
	// init_get_current_time();
	init_deal_schedule();

	;

	set_color(Color_DF);
	return 0;
}