#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
/* Name: doodle jump
 * Author: Anonymous (Wechat: wxid_u53i7cbs8l8421)
 * LastModification: 2018/9/13
 * Log: 9/11 Day1
 *      - Scene copied from Tetris
 *      - player initiated and able to jump naturally
 *      9/12 Day2
 *      - scene and player able to move interactively
 *      - todo: bordercheck, score, buttomcheck, img fileSrc
 *      9/13 Day3
 *      - done bordercheck, score, buttomcheck, img fileSrc
 *      - done basic functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <math.h>
#include <direct.h>
#include <string.h>

#define TITLE_TEXTSIZE  40
#define TITLE_MARGIN    10
#define SIDEBAR_WIDTH   200

#define PLAYER_WIDTH    60
#define PLAYER_HEIGHT   62
#define BRICK_WIDTH     50
#define BRICK_HEIGHT    15

#define MAX_HEIGHT      200

extern PIMAGE scene;

typedef struct
{
    int x;
    int y;
    bool isInMap;
}BRICK;

typedef struct
{
    float x;
    float y;
    float vertSpeed;
    float horiSpeed;
    PIMAGE img;
}PLAYER;

void welcomePage();
void initScene();
void updateScene();
void reset();

void initBrick();
void initPlayer();
void updatePlayer();
bool jumpCheck();

void getPath(char*, char*);
void resizeImg(PIMAGE &, int, int);




long score;
BRICK brick[20];
PLAYER player;
PIMAGE IMG_BRICK;
static long scoreTmp=0;     // the score displayed on screen
static bool pauseFlag=0;    // if key 'P' has pressed
static bool initFlag=0;     // if player has initiated
static bool mapMoveFlag=0;  // is map moving
static bool leftBorder=0, rightBorder=0;    // if player has reached border
static int timeBegin, currentHeight=480;    // timer and map initiation related

// to tell gamer how to play
void welcomePage()
{
    short cnt=0;
    char *flashStr="Press any key to start.";
    char *welcomeStr="Welcome to Doodle Jump!\n"
        "This is a Beta version of the game\n"
        "Thanks for playing. Have fun!\n\n"
        "How to play:\n"
        "Move: Press 'A' or 'D'\n"
        "Pause: Press 'P'";

    setfont(25, 0, "宋体");
    for( ; is_run(); delay_fps(2) )
    {
        cnt=cnt^1;
        cleardevice();

        outtextrect(30,20,640,480,welcomeStr);
        if(cnt)
            outtextxy(30, 300, flashStr);
        if(kbhit())
            break;
    }
    cleardevice();
}

// get file path of game images
void getPath(char* filePath, char* relPath)
{
    char ch=1;
    int i=0;
    getcwd(filePath, 100);
    strcat(filePath, relPath);

    // turn '\' into '\\' in filePath
    while(ch!='\0')
    {
        ch=filePath[i];
        if(ch=='\\')
        {
            for(int j=99; j>i; j--)
                filePath[j]=filePath[j-1];
            i++;
        }
        i++;
    }
}

/*  Log: 9/11 BGM disabled;
 *  Sucessfully tested
 */
// initiate game scene and all argument
void initScene()
{
    int titleWidth;
    char brickPath[100], playerPath[100];
    char *brickImg=".\\Image\\IMG_brick.png", *playerImg=".\\Image\\IMG_player.png";

    // get file path
    getPath(brickPath, brickImg);
    getPath(playerPath, playerImg);

    // initiate player img
    player.img=newimage();
    getimage(player.img, playerPath);
    resizeImg(player.img, PLAYER_WIDTH, PLAYER_HEIGHT);

    // initiate score and clock
    score=0;
    timeBegin=time(NULL);

    // initiate argu
    for(int j=0; j<20; j++)
        brick[j].isInMap=0;

    // initiate brick img
    IMG_BRICK=newimage();
    getimage(IMG_BRICK, brickPath, 0, 0);
    resizeImg(IMG_BRICK, BRICK_WIDTH, BRICK_HEIGHT);

    // initiate scene
    scene=newimage();
    setfont(TITLE_TEXTSIZE, 0, "宋体");
    titleWidth=textwidth("D");

    //PlaySound(TEXT("bgm.wav"), NULL,
    //          SND_FILENAME | SND_ASYNC | SND_LOOP);
    outtextrect(TITLE_MARGIN,
                TITLE_MARGIN,
                TITLE_MARGIN+titleWidth,//=SIZE
                TITLE_MARGIN+12*TITLE_TEXTSIZE,
                "DOODLE  JUMP");
    line(2*TITLE_MARGIN+titleWidth, 1,
         2*TITLE_MARGIN+titleWidth, ege::getheight()-1);
    line(ege::getwidth()-SIDEBAR_WIDTH, 1,
         ege::getwidth()-SIDEBAR_WIDTH, ege::getheight()-1);
    setfont(25, 0, "宋体");
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(540, 30, "Time");
    outtextxy(540, 130, "Score");
    getimage(scene, 0, 0, 640, 480);
}

// initiate bricks when map is not completely filled
void initBrick()
{
    int rand;
    static int i=0;
    // currentHeight: the height of the top brick
    while(currentHeight>40)
    {
        //if brick is already in use, skip it
        if(brick[i].isInMap==1)
        {
            i++;
            if(i==20)
                i=0;
            continue;
        }
        rand=random(350);
        currentHeight-=60;
        brick[i].isInMap=1;
        brick[i].x=rand+40;
        brick[i].y=currentHeight;
        i++;
        if(i==20)
            i=0;
    }
}

// intiate player if is not intiated
void initPlayer()
{
    if(!initFlag)
    {
        initFlag=1;
        player.x=200;
        player.y=300;
        player.vertSpeed=2;
    }
    else
        return;
}

// update player's position
char keybd;
void updatePlayer()
{
    static bool i=1;
    // if(kbhit()) keybd=getch();
    if(kbmsg()) {
        key_msg km=getkey();
        if(km.msg==key_msg_up) keybd=0;
        else keybd=km.key;
    }
    switch(keybd)
    {
        case 'a':
            if(leftBorder)  // if reached border
                break;
            if(player.horiSpeed<2&&player.horiSpeed>-6)
                player.horiSpeed=(-6+player.horiSpeed*3)/4;
            else
                player.horiSpeed=0;
            break;
        case 'd':
            if(rightBorder) // if reached border
                break;
            if(player.horiSpeed>-2&&player.horiSpeed<6)
                player.horiSpeed=(6+player.horiSpeed*3)/4;
            else
                player.horiSpeed=0;
            break;
        case 'p':
            pauseFlag=1;
            break;
        default: 
            player.horiSpeed=0;
            break;
    }
    // bordercheck
    if(player.x<45)
        leftBorder=1;
    else
        leftBorder=0;
    if(player.x+PLAYER_WIDTH>450)
        rightBorder=1;
    else
        rightBorder=0;
    if((rightBorder&&player.horiSpeed>0)||(leftBorder&&player.horiSpeed<0))
        player.horiSpeed=0;

    // decrease horizontal speed gradually
    if(player.horiSpeed>0)
        player.horiSpeed-=0.1;
    if(player.horiSpeed<0)
        player.horiSpeed+=0.1;

    // simulate gravity
    player.vertSpeed-=0.2;

    // jump
    if(jumpCheck())
        player.vertSpeed=9;

    // stop player, move map
    if(player.y<MAX_HEIGHT)// above max_height
        mapMoveFlag=1;
    if(mapMoveFlag)
    {
        currentHeight+=player.vertSpeed;
        score+=player.vertSpeed;
    }

    // if player's going down, stop map, move player
    if(player.vertSpeed<0&&player.y<MAX_HEIGHT)
        mapMoveFlag=0;

    // if player's going down (end of the game), move map
    if(player.y+PLAYER_HEIGHT>480&&score!=0)
        mapMoveFlag=1;

    // init more brick, player stopped
    if(!mapMoveFlag)
        player.y-=player.vertSpeed;

    // update player position
    player.x+=player.horiSpeed;
}

bool jumpCheck()
{
    // if going up, ignore bricks
    if(player.vertSpeed>0)
        return false;
    // be friendly to new gamers.
    // if no score, no end of game
    if(player.y+PLAYER_HEIGHT>=470)
    {
        if(scoreTmp)
            return false;
        else
            return true;
    }

    // check for bricks
    for(int i=0; i<20; i++)
    {
        if(brick[i].isInMap==1)
        {
            if(player.x+PLAYER_WIDTH-20>brick[i].x // player left range
               &&player.x+20<brick[i].x+BRICK_WIDTH// player right range
               &&fabs(player.y+PLAYER_HEIGHT-brick[i].y-5)<5)
               return true;
        }
    }
    return false;
}

void updateScene()
{
    char timeStr[10], scoreStr[10];
    time_t timeCurrent, gameTime;

    // update time
    timeCurrent=time(NULL);
    gameTime=timeCurrent-timeBegin;
    sprintf(timeStr, "%02d:%02d", gameTime/60, gameTime%60);
    outtextxy(540, 60, timeStr);

    // update score
    // if player is going down, stop score update
    if(score-scoreTmp>0)
        scoreTmp=score;
    sprintf(scoreStr, "%d", scoreTmp);
    outtextxy(540, 160, scoreStr);
    // player going down. if all bricks are gone, end game
    if(score-scoreTmp<-480)
    {
        char endStr[20];
        sprintf(endStr, "Final Score:%d", scoreTmp);
        setfont(30, 0, "宋体");
        outtextxy(240, 120, "GAME OVER");
        outtextxy(240, 150, endStr);
        outtextxy(240, 180, "Press any key");
        outtextxy(240, 210, "to start a new game");
        getch();
        reset();
    }

    // update brick and player
    for(int i=0; i<20; i++)
    {
        if(brick[i].isInMap==1)
        {
            // if brick goes down, delete it from map
            if(brick[i].y>470)
            {
                brick[i].isInMap=0;
                continue;
            }
            // if map moves, brick moves
            if(mapMoveFlag)
            {
                brick[i].y+=player.vertSpeed;
            }
            putimage_transparent(NULL, IMG_BRICK, brick[i].x, brick[i].y, BLACK);
        }
    }
    putimage_transparent(NULL, player.img, player.x, player.y, BLACK);

    // pause game
    if(pauseFlag)
    {
        //PlaySound(NULL,NULL,SND_FILENAME);
        getch();
        pauseFlag=0;

        //PlaySound(TEXT("bgm.wav"), NULL,
        //      SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
}

void reset()
{
    for(int i=0; i<20; i++)
    {
        brick[i].isInMap=0;
    }
    score=0;
    scoreTmp=0;
    currentHeight=480;
    timeBegin=time(NULL);
    pauseFlag=0, initFlag=0, mapMoveFlag=0;
}

/*  Author: stophin@BaiduTieba
 *  Site:   http://tieba.baidu.com/p/5561082957
 */
void resizeImg(PIMAGE &img, int pwidth, int pheight)
{
    int width,height;
    //Resize image to pwidth*pheight
    PIMAGE save=gettarget();
    //Get image size
    settarget(img);
    width=getwidth();
    height=getheight();

    settarget(save);
    PIMAGE backproc=newimage(pwidth, pheight);
    //Resize
    putimage(backproc, 0, 0, pwidth, pheight, img, 0, 0, width, height);
    getimage(img, backproc, 0, 0, pwidth, pheight);
    delimage(backproc);
}



PIMAGE scene;

void mainloop()
{
	welcomePage();
	initScene();
	for ( ; is_run(); delay_fps(60) )
	{
		cleardevice();

        putimage(0, 0, scene);
        initBrick();
        initPlayer();

        updateScene();
        updatePlayer();
	}
}

int main(void)
{
	setinitmode(INIT_ANIMATION);    // 设置ege模式为动画模式
	initgraph(640, 480);            // 图形初始化，窗口尺寸640x480
	randomize();                    // 随机数初始化，如果需要使用随机数的话
	setrendermode(RENDER_MANUAL);   // 绘图更新模式设置为手动，避免闪烁

	setcaption("DoodleJump Beta");
	// 程序主循环
	mainloop();
	// 关闭绘图设备
	closegraph();
	return 0;
}
