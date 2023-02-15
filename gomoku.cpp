//重复占子问题****
//棋局满的状态
#include<stdio.h>
#include<graphics.h>
#include"gomoku.h"
unsigned int Round_gomoku;
bool is_win_gomoku;
bool is_move;
IMAGE img_wuzi;
struct chess
{
    int x;
    int y;
    unsigned int  type;
};
struct chess un_gomoku;//虚影没落子的地方值记录
struct chess gomoku[15][15];//制造棋子，记录信息;
int  main_gomoku(void)
{
    initGame2();
    BeginBatchDraw();
    while(is_win_gomoku)
    {
        if(Round_gomoku==BLACK)
        {
            while(Round_gomoku==BLACK)
            {
                playChess();
                drawGame2();
                FlushBatchDraw();
                gameOver2();
            }
        }
        else
        {
            while(Round_gomoku==WHITE)
            {
                playChess();
                drawGame2();
                FlushBatchDraw();
                gameOver2();
            }
        }
        FlushBatchDraw();
        //刷掉红色显示器
        setlinecolor(BLACK);
        setfillcolor(BLACK);
        fillrectangle(620,240,780,310);
        Sleep(1000);
    }
     EndBatchDraw();
    //胜负显示
    setfillcolor(RED);
    fillrectangle(620,240,780,310);
    if(Round_gomoku==WHITE)
            outtextxy(660,250,"黑胜");
    else
            outtextxy(660,250,"白胜");
    Sleep(1000);
    closegraph();
    return 0;
}
void initGame2(void)
{
    Round_gomoku=BLACK;//回合初定
    is_win_gomoku=true;//开始运行
    loadimage(&img_wuzi,"./res/wuzi.PNG",600,600);
    initgraph(800,600);
    //回合显示表 绘图
    fillrectangle(620,70,780,140);
    //绘图
    setfillcolor(BLACK);
    setlinecolor(WHITE);
    fillrectangle(620,380,780,450);
    //初始化底层数据
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            gomoku[i][j].type=1;
            gomoku[i][j].x=-1;
            gomoku[i][j].y=-1;
        }
}
void drawGame2(void)
{
    //贴图
    putimage(0,0,&img_wuzi);
    //设置
    settextstyle(40,0,"隶书");
    setbkmode(0);
    setlinecolor(WHITE);
    //显示回合表
    if(Round_gomoku==BLACK)//黑方
    {
         settextcolor(WHITE);
         outtextxy(620,400,"黑方回合");
         setfillcolor(WHITE);
         fillrectangle(620,70,780,140);
    }
    else//白方
    {
        settextcolor(BLACK);
        outtextxy(620,90,"白方回合");
        setfillcolor(BLACK);
        fillrectangle(620,380,780,450);
    }
    //回合 画落子
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            if(gomoku[i][j].type!=1)//只在落子的地方显示
            {
                if(gomoku[i][j].type==BLACK)
                {
                 setlinecolor(BLACK);
                 setfillcolor(BLACK);
                 fillcircle(gomoku[i][j].x,gomoku[i][j].y,10);
                }
                else if(gomoku[i][j].type==WHITE)
                {
                setfillcolor(WHITE);
                solidcircle(gomoku[i][j].x,gomoku[i][j].y,10);//精华
                }
            }
        }
        //预落子状态
    if(Round_gomoku==BLACK&&
       un_gomoku.x!=-1)//归零处理
    {
        setlinecolor(BLACK);
        setfillcolor(BLACK);
        fillcircle(un_gomoku.x,un_gomoku.y,10);
    }
    else if(Round_gomoku==WHITE&&
            un_gomoku.x!=-1)
    {
        setfillcolor(WHITE);
        solidcircle(un_gomoku.x,un_gomoku.y,10);
    }
}
void playChess(void)
{
    is_move=false;//还未落子
    //点击事件
    if(MouseHit())
   {
        //点击
        MOUSEMSG msg=GetMouseMsg();
        int chess_x=(msg.x-INTER_X)/SIZE*SIZE+INTER_X;//整数距离，刚好落点上
        int chess_y=(msg.y-INTER_Y)/SIZE*SIZE+INTER_Y;
        //调整
        if(msg.x>chess_x+19&&msg.y>chess_y+19){chess_x=chess_x+SIZE;chess_y=chess_y+SIZE;};//右下处
        if(msg.x>chess_x+19&&msg.y<chess_y+19){chess_x=chess_x+SIZE;};//右处
        if(msg.x<chess_x+19&&msg.y>chess_y+19){chess_y=chess_y+SIZE;};//下处
        //目前为止，chess_x任然是像素集
        //移动事件
        if(msg.uMsg==WM_MOUSEMOVE)
        {
            //在棋盘内移动
            if(msg.x>=INTER_X-15&&msg.y>=INTER_Y-15
               &&msg.x<=14*SIZE+INTER_X+15
               &&msg.y<=14*SIZE+INTER_Y+15)
            {
                if(gomoku[(chess_y-INTER_Y)/SIZE][(chess_x-INTER_X)/SIZE].type==1)
                {
                    un_gomoku.x=chess_x;
                    un_gomoku.y=chess_y;
                }
                else//当指到有棋子的地方时，归零
                {
                    un_gomoku.x=-1;
                    un_gomoku.y=-1;
                }
            }
            //棋盘外移动，归零
            else
            {
                un_gomoku.x=-1;
                un_gomoku.y=-1;
            }
        }
        //点击事件
        if(msg.uMsg==WM_LBUTTONDOWN&&msg.x>=INTER_X-15//点击事件
          &&msg.y>=INTER_Y-15&&msg.x<=14*SIZE+INTER_X+15
          &&msg.y<=14*SIZE+INTER_Y+15)//边界条件
        {
           if(gomoku[(chess_y-INTER_Y)/SIZE][(chess_x-INTER_X)/SIZE].type==1)//落子在空的地方
           {
                 gomoku[(chess_y-INTER_Y)/SIZE][(chess_x-INTER_X)/SIZE].type=Round_gomoku;//落子
                 gomoku[(chess_y-INTER_Y)/SIZE][(chess_x-INTER_X)/SIZE].x=chess_x;
                 gomoku[(chess_y-INTER_Y)/SIZE][(chess_x-INTER_X)/SIZE].y=chess_y;
                 is_move=true;//已落子
           }
           else
           {
                setfillcolor(RED);
                fillrectangle(620,240,780,310);
                outtextxy(630,250,"重新落子");
           }
       }
   }
}
void gameOver2(void)
{
    for(int i=0;i<15;i++)
        for(int j=0;j<15;j++)
        {
            if(gomoku[i][j].type==Round_gomoku)
            {
                int k=0,l=0,m=0,n=0;//计数
                for( k=0;k<5;k++)//横着遍历
                {
                    if(gomoku[i][j+k].type!=Round_gomoku)
                        break;
                }
                if(k==5)
                {
                    is_win_gomoku=false;
                    break;
                }
                for(l=0;l<5;l++)//竖着遍历
                {
                    if(gomoku[i+l][j].type!=Round_gomoku)
                        break;
                }
                if(l==5)
                {
                       is_win_gomoku=false;
                    break;
                }
                for(m=0;m<5;m++)//右下
                {
                    if(gomoku[i+m][j+m].type!=Round_gomoku)
                        break;
                }
                if(m==5)
                {
                    is_win_gomoku=false;
                    break;
                }
                for(n=0;n<5;n++)
                {
                    if(gomoku[i+n][j-n].type!=Round_gomoku)
                    break;
                }
                if(n==5)
                {
                    is_win_gomoku=false;
                    break;
                }
            }
        }
//        判断胜负后，再决定
        if(is_move)
           Round_gomoku=Round_gomoku==BLACK?WHITE:BLACK;
}
