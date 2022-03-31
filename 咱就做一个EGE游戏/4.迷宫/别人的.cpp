/*
https://blog.csdn.net/qq_39151563/article/details/105138732?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164805331416780265417797%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164805331416780265417797&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~sobaiduend~default-1-105138732.nonecase&utm_term=ege+%E8%BF%B7%E5%AE%AB&spm=1018.2226.3001.4450
*/

//#define SHOW_CONSOLE
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//定义四个方向的行进增量
#define DIRECTION_NUM 4
int dx[DIRECTION_NUM] = { -1, 0, 1, 0 };
int dy[DIRECTION_NUM] = { 0, -1, 0, 1 };

//自定义颜色
const int COLOR_BLUE  = EGERGB(5, 224, 218);

//迷宫大小设置
const int MAZE_INC_SIZE = 8;						//迷宫大小变化增量
const int MAZE_MAX_SIZE = 5 * MAZE_INC_SIZE;		//迷宫大小最大值
const int MAZE_MIN_SIZE = MAZE_INC_SIZE;			//迷宫大小最小值
const int GRID_MAX_NUM = 2 * MAZE_MAX_SIZE + 1;		//迷宫格子数最大值(每行)

//迷宫
typedef struct Maze
{
	int size;			//迷宫大小
	int gridNum;		//格子数(每行)
	int gridWidth;		//格子宽度
	int grid[GRID_MAX_NUM][GRID_MAX_NUM];	//格子数据
}Maze;

//方向定义
enum Direction {
	LEFT  = 0,
	UP    = 1,
	RIGHT = 2,
	DOWN  = 3
};

//块类型
enum BlockType {
	BLOCK_BLANK = 0,	//空格
	BLOCK_WALL,			//墙
	BLOCK_PATH,			//路径
};

//迷宫样式
enum MazeStyle
{
	MAZE_STYLE_GRID = 0,	//格子型
	MAZE_STYLE_WALL	= 1,	//围墙型
};


void createMaze(int size);
void initMaze();
void drawMaze(MazeStyle style, bool enablePath);
bool findPath(int x, int y);
void drawHelpTip();

void shuffle(int a[], int length);

//迷宫数据
Maze maze;

int xCur, yCur;
int scrWidth = 600, scrHeight = 600;

int main()
{
	setinitmode(INIT_RENDERMANUAL);
	createMaze(3 * MAZE_INC_SIZE);
	setcaption("迷宫");
	setbkmode(TRANSPARENT);

	MazeStyle style = MAZE_STYLE_GRID;	//样式
	bool flag_showPath = true;			//路径显示标志位
	bool flag_showHelp = false;
	bool flag_firstEnterGame = true;
	bool redraw = true;					//重绘标志位

	srand((unsigned)time(0));

	drawMaze(style, false);				//绘制迷宫
	drawHelpTip();

	while (is_run()) {
		key_msg msg = getkey();

		int d = -1;
		if (msg.msg == key_msg_down) {
			//方向移动
			switch (msg.key) {
			case 'A': case key_left:	d = LEFT;	break;
			case 'W': case key_up:		d = UP;		break;
			case 'D': case key_right:	d = RIGHT;	break;
			case 'S': case key_down:	d = DOWN;	break;

			//功能键

			case key_enter:	//回车键，查找路径
				findPath(xCur, yCur);
				flag_showPath = true;	//同时设置显示路径
				redraw = true;
				break;

			case key_space:	//空格键，显示/隐藏路径
				flag_showPath = !flag_showPath;
				redraw = true;
				break;

			case key_esc:	//ESC键，重新开始
				initMaze();
				redraw = true;
				break;

			case key_f1:	//F1键，显示帮助
			{
				flag_showHelp = !flag_showHelp;
				//按任意键关闭帮助提示
				redraw = true;
				break;
			}
			case 'T':							//T键，切换迷宫样式
				style = (style == MAZE_STYLE_GRID) ? MAZE_STYLE_WALL : MAZE_STYLE_GRID;
				redraw = true;
				break;
			case key_plus: case VK_ADD:			// +键，增大迷宫复杂度
				if (maze.size < 40) {
					createMaze(maze.size + 8);
					redraw = true;
				}
				break;
			case key_minus: case VK_SUBTRACT:	// -键，减小迷宫复杂度
				if (maze.size > 8) {
					createMaze(maze.size - 8);
					flag_showPath = true;
					redraw = true;
				}
				break;
			}
		}

		//按方向移动(单次移动两格)
		if (d != -1) {
			//分别计算按指定方向行进一格和行进两格的位置
			int xNext1 = xCur + dx[d],     yNext1 = yCur + dy[d];
			int xNext2 = xCur + 2 * dx[d], yNext2 = yCur + 2 * dy[d];

			//判断是否可以按指定方向移动(目标方向上无墙格挡即可移动)
			if (0 <= xNext1 && xNext1 < maze.gridNum && 0 <= yNext1 && yNext1 < maze.gridNum
				&& (maze.grid[yNext1][xNext1] != BLOCK_WALL)) {

				//根据路线上的格子类型，修改路线样式
				switch (maze.grid[yNext1][xNext1]) {
				case BLOCK_PATH: //目标地点为路径，则消除路线上的路径标记，走过地方设置为空格
					maze.grid[yCur][xCur] = BLOCK_BLANK;
					maze.grid[yNext1][xNext1] = BLOCK_BLANK;
					break;

				case BLOCK_BLANK: //目标地点为空格，则路线上的格子设置为当前的值
					maze.grid[yNext1][xNext1] = maze.grid[yCur][xCur];
					maze.grid[yNext2][xNext2] = maze.grid[yCur][xCur];
					break;
				}

				//移动到目标位置
				xCur = xNext2;
				yCur = yNext2;

				//判断是否到终点(右下角)
				if ((xCur == maze.gridNum - 2) && (yCur == maze.gridNum - 2))
					initMaze();

				redraw = true;
			}
		}

		//首次进入游戏按任意键关闭帮助提示
		if (flag_firstEnterGame) {
			flag_firstEnterGame = false;
			flag_showHelp = false;
			redraw = true;
		}

		//重绘
		if (redraw) {
			redraw = false;
			drawMaze(style, flag_showPath);
			if (flag_showHelp) {
				drawHelpTip();
			}
		}
	}
	closegraph();
	return 0;
}

//创建迷宫
void createMaze(int size)
{
	if ((size > MAZE_MAX_SIZE) || (maze.size == size))
		return;

	maze.size = size;
	maze.gridNum = 2 * size + 1;

	//预设窗口宽高为600左右
	maze.gridWidth = (int)ceil(600.0f / maze.gridNum);	

	initMaze();

	//调整窗口大小
	scrWidth = maze.gridNum * maze.gridWidth;
	scrHeight = maze.gridNum * maze.gridWidth;
	initgraph(scrWidth, scrHeight);
}


//随机置乱
void shuffle(int a[], int length)
{
	for (int i = length - 1; i > 0; i--) {
		int r = rand() % i + 1;
		int temp = a[i];
		a[i] = a[r];
		a[r] = temp;
	}
}

//绘制带路径的迷宫
void drawMaze(MazeStyle style, bool enablePath)
{
	setbkcolor_f(BLACK);
	cleardevice();

	//格子内部边缘空隙宽度，取格子宽度1/8
	int padding = maze.gridWidth / 8;

	//根据迷宫样式，进行不同的绘制
	switch (style)
	{
	case MAZE_STYLE_GRID:	//方格样式
		setfillcolor(COLOR_BLUE);
		for (int i = 0; i < maze.gridNum; i++) {
			for (int j = 0; j < maze.gridNum; j++) {
				if (maze.grid[i][j] == BLOCK_WALL)
					bar(j * maze.gridWidth + padding, i * maze.gridWidth + padding, (j + 1) * maze.gridWidth - padding, (i + 1) * maze.gridWidth - padding);
			}
		}
		break;
	case MAZE_STYLE_WALL:	//围墙样式
		setcolor(WHITE);
		setlinewidth(2);
		//横围墙
		for (int i = 0; i < maze.gridNum; i += 2) {
			for (int j = 1; j < maze.gridNum; j += 2) {
				if (maze.grid[i][j] == BLOCK_WALL)
					line((j - 1) * maze.gridWidth + maze.gridWidth / 2, i * maze.gridWidth + maze.gridWidth / 2,
						(j + 1) * maze.gridWidth + maze.gridWidth / 2, i * maze.gridWidth + maze.gridWidth / 2);
			}
		}
		//竖围墙
		for (int i = 1; i < maze.gridNum; i += 2) {
			for (int j = 0; j < maze.gridNum; j += 2) {
				if (maze.grid[i][j] == BLOCK_WALL)
					line(j * maze.gridWidth + maze.gridWidth / 2, (i - 1) * maze.gridWidth + maze.gridWidth / 2,
						j * maze.gridWidth + maze.gridWidth / 2, (i + 1) * maze.gridWidth + maze.gridWidth / 2);
			}
		}
		break;
	}

	//绘制到终点的路径
	if (enablePath) {
		setfillcolor(YELLOW);

		for (int i = 1; i < maze.gridNum; i++) {
			for (int j = 1; j < maze.gridNum; j++) {
				if (maze.grid[i][j] == BLOCK_PATH)
					bar(j * maze.gridWidth + padding, i * maze.gridWidth + padding,
						(j + 1) * maze.gridWidth - padding, (i + 1) * maze.gridWidth - padding);
			}
		}
	}

	//绘制当前位置
	setfillcolor(RED);

	bar(xCur * maze.gridWidth + padding, yCur * maze.gridWidth + padding,
		(xCur + 1) * maze.gridWidth - padding, (yCur + 1) * maze.gridWidth - padding);

}

void drawHelpTip()
{
	//帮助文本内容
	const char* text[] = {
		"F1 键 ：显示/关闭帮助提示",
		"方向键：移动",
		"AWDS键：移动",
		"空格  ：显示/隐藏路径",
		"回车  ：查找路径",
		"ESC键 ：重新开始",
		"T  键 ：迷宫样式",
		"+  键 ：增加难度",
		"-  键 ：减小难度",
	};

	int textNum = sizeof(text) / sizeof(text[0]);
	int textAreaLeft = 120, textAreaTop = 100;
	int lineHeight = 30;

	//绘制一个半透明的白色
	setfillcolor(EGEACOLOR(0xD0, WHITE));
	ege_fillrect(textAreaLeft, textAreaTop, scrWidth - 2 * textAreaLeft, textNum * lineHeight);


	//绘制文本内容
	setcolor(BLACK);
	setfont(24, 0, "楷体");
	int textLeft = textAreaLeft + 60, textCurY = 100;

	for (int i = 0; i < textNum; i++) {
		xyprintf(textLeft, textCurY, text[i]);
		textCurY += lineHeight;
	}
}

//带权的路径压缩并查集
class UnionFindSet
{
private:
	int* set;
	int* weight;
	int size;
	int count;
public:
	UnionFindSet(int size)
	{
		this->size = size;
		set = new int[size];
		weight = new int[size];

		init();
	}
	~UnionFindSet() {
		delete[] set;
		delete[] weight;
	}

	void init()
	{
		count = size;
		for (int i = 0; i < size; i++) {
			set[i] = i;
			weight[i] = 1;
		}
	}

	int find(int id)
	{
		while (set[id] != id)
			id = set[id];

		return id;
	}


	int quickFind(int id)
	{
		int root = id;
		while (set[root] != root)
			root = set[root];

		//路径压缩，将沿路的结点直接连到根节点上
		while (set[id] != root) {
			int t = id;
			id = set[id];
			set[t] = root;
		}

		return root;
	}

	//合并两个集合，返回是否进行了合并操作
	bool unionSet(int p, int q)
	{
		int i = quickFind(p), j = quickFind(q);
		if (i == j)
			return false;

		count--;
		if (weight[i] < weight[j]) {
			set[i] = j;
			weight[j] += weight[i];
		}
		else {
			set[j] = i;
			weight[i] += weight[j];
		}

		return true;
	}

	//返回集合数
	int setCount() const { return count; }
private:
	//禁止复制(重点在算法，减少其它方面的代码)
	UnionFindSet(const UnionFindSet& set);
	UnionFindSet& operator=(const UnionFindSet& set);
};

//查找路径，返回是否查找到至终点的路径
bool findPath(int x, int y)
{
	if (maze.grid[y][x] == BLOCK_BLANK) {
		maze.grid[y][x] = BLOCK_PATH;

		if ((x == maze.gridNum - 2) && (y == maze.gridNum - 2))
			return true;

		//搜索四个方向
		for (int i = 0; i < DIRECTION_NUM; i++) {
			if (findPath(x + dx[i], y + dy[i]))
				return true;
		}

		maze.grid[y][x] = BLOCK_BLANK;
	}
	return false;
}

//迷宫初始化
void initMaze()
{
	//初始化
	for (int i = 0; i < maze.gridNum; i++) {
		for (int j = 0; j < maze.gridNum; j++) {
			maze.grid[i][j] = BLOCK_WALL;
		}
	}

	for (int i = 1; i < maze.gridNum; i += 2) {
		for (int j = 1; j < maze.gridNum; j += 2) {
			maze.grid[i][j] = BLOCK_BLANK;
		}
	}

	//随机开始地点(空格)
	xCur = rand() % maze.size;
	yCur = rand() % maze.size;

	//不出现在右下角，避免出现过于简单的情况
	if ((xCur >= maze.size / 2) && (yCur >= maze.size / 2)) {
		xCur -= maze.size / 2;
		yCur -= maze.size / 2;
	}

	//对应到实际的数组坐标
	xCur = xCur * 2 + 1;
	yCur = yCur * 2 + 1;


	//随机拆墙，创建迷宫

	//创建一个随机的拆墙顺序
	//空格左右的可拆墙编号为0 ~ maze.size * (msze.size-1) - 1
	//空格上下的可拆墙编号为maze.size * (msze.size-1) ~ 2 * maze.size * (msze.size-1) - 1
	//这两种墙有点不同，所以分开来

	const int WALL_SIZE = 2 * maze.size * (maze.size - 1);	//所有可拆墙数目
	int* randWall = new int[WALL_SIZE];

	for (int i = 0; i < WALL_SIZE; i++) {
		randWall[i] = i;
	}

	//随机打乱
	shuffle(randWall, WALL_SIZE);

	//并查集
	UnionFindSet ufSet(maze.size * maze.size);
	
	//被简化过
	//根据randWall中的顺序连通所有空格，连通时拆除两个空格之间的墙
	for (int i = 0; i < WALL_SIZE; i++) {

		//如果已全部连通，退出
		if (ufSet.setCount() <= 1)
			break;

		//下面代码压缩过，通过编号计算出墙的索引，两边空格的索引。

		int xWall, yWall, width, tx = 0, ty = 0;

		//空格左右的可拆墙
		if (randWall[i] < WALL_SIZE / 2) {
			width = maze.size - 1;
			tx = 1;
		}
		//空格上下的可拆墙
		else {
			randWall[i] -= WALL_SIZE / 2;
			width = maze.size;
			ty = 1;
		}

		//通过编号计算墙在grid数组中的实际索引
		xWall = randWall[i] % width;
		yWall = randWall[i] / width;

		//墙两边的空格在grid数组中的实际索引
		int x1 = xWall, y1 = yWall, x2 = xWall + tx, y2 = yWall + ty;

		//如果进行了连通操作，那么拆墙
		if (ufSet.unionSet(y1 * maze.size + x1, y2 * maze.size + x2)) {
			maze.grid[yWall + y2 + 1][xWall + x2 + 1] = BLOCK_BLANK;
		}
	}

	delete[] randWall;
}

