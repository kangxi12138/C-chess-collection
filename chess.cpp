#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include"chess.h"
#include"menu.h"
unsigned int Round;//设置回合标志,开始红方先走
bool is_win,over,ret;//is_win(赢了就退出),over是认输机制 ret是悔棋机制
IMAGE img,img2;//照片对象
enum PIECES//枚举棋子
{
    SPACE=-1,
	JU,MA,XI,SI,JI,PA,BI,//黑方
	ju,ma,xi,si,ji,pa,bi,//红方
	BEGIN,END
};
enum PIECES blackChess[] = { JU,MA,XI,SI,JI,PA,BI};//黑方
enum PIECES redChess[] = {ju,ma,xi,si,ji,pa,bi};//红方
const char *chessName[] = { "車", "馬", "象", "士", "将", "砲", "卒", "俥", "马", "相", "仕", "帥", "炮", "兵" };//棋子上写字
enum PIECES state;//标志点击事件开始与结束
POINT begin,end;//点击事件开始与结束初始化（function:记录两次点击位置）
struct Chess//棋子属性
{
	int x;
	int y;
	enum PIECES id;//棋子名字
    int type;//是哪一方的棋子 红or黑
	bool river;//判断小兵是否过了河
};
struct Chess red_chess[2],black_chess[2];//记录棋子的结构体(用于悔棋)
struct Chess  map[ROW][COL];//结构体数组，用来保存每一个点的信息
struct Chess cp_chess;//被吃的子
int main_chess(void)//mistake
{
	GameInit();//游戏初始化
    BeginBatchDraw();
    while(is_win)//游戏未结束，则继续循环
    {
        if(Round==RED)
        {
            printf("红方回合\n");
            while(Round==RED&&is_win)//加上is_win是为了游戏结束直接退出
            {
                MouseControl();
                ChessMove();
                GameDraw();//每次数据更新后，进行画图
                FlushBatchDraw();//重要
                GameOver();
            }
        }
        else
        {
            printf("黑方回合\n");
            while(Round==BLACK&&is_win)//加上is_win是为了游戏结束直接退出
           {
                MouseControl();
                ChessMove();
                GameDraw();//每次数据更新后，进行画图
                FlushBatchDraw();
                GameOver();
           }
        }
        FlushBatchDraw();
        Sleep(1000);
        //净化页面
        begin={-1,-1};
    }
    EndBatchDraw();
    Sleep(1000);
    printf("游戏结束！\n");
    Sleep(1000);
    closegraph();//关闭绘图界面
	return 0;
}
void GameInit(void)
{
    Round =RED;//开局红方回合
    is_win=true;//开启循环
    begin ={-1,-1},end={-1,-1};
    state=BEGIN;
    cp_chess={-1,-1,SPACE,SPACE,false};
    red_chess[1]={-1,-1,SPACE,SPACE,false};
    red_chess[0]={-1,-1,SPACE,SPACE,false};
    black_chess[0]={-1,-1,SPACE,SPACE,false};
    black_chess[1]={-1,-1,SPACE,SPACE,false};

    loadimage(&img,"./res/Chess.PNG");//上传图片
    loadimage(&img2,"./res/back.PNG",200,804);

	//创建窗口

	initgraph(img.getwidth()+200,img.getheight(),1);
    //给棋属性赋值

	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
            enum PIECES chessID=SPACE;//先把所有棋子置为SPACE（无棋）
            int chessType=SPACE;//定义棋的阵营，SPAce表示无棋子的地方
            //安置黑棋
            if(i<=4)
            {
                chessType=BLACK;//判断是黑方阵营
                //设置第一行棋子
                if(i==0)
                {
                    if(k<=4)
                    {
                        chessID=blackChess[k];
                    }
                    else
                    {
                        chessID=blackChess[8-k];
                    }
                }//安排炮
                if(i==2&&(k==1||k==7))
                {
                    chessID=blackChess[5];
                }//放置兵
                if(i==3&&k%2==0)
                {
                    chessID=blackChess[6];
                }
            }
            //安置红棋
            else
            {
                chessType=RED;//红方
                if(i==9)
                {
                    if(k<=4)
                    {
                        chessID=redChess[k];
                    }
                    else
                    {
                        chessID=redChess[8-k];
                    }
                }//安置炮
                if(i==7&&(k==1||k==7))
                {

                    chessID=redChess[5];
                }//安置兵
                if(i==6&&k%2==0)
                {
                    chessID=redChess[6];
                }
            }
			map[i][k].id =chessID;//分配棋名
			if(map[i][k].id!=SPACE)//若有棋则分配BLACK or RED
                map[i][k].type=chessType;
            else//无棋则置为SPACE
                map[i][k].type=SPACE;
			map[i][k].river = false;//初始化小兵未过河
			map[i][k].x = k*CHESS_SIZE + INTERVAL;//记录所有地方的像素位置
			map[i][k].y = i*CHESS_SIZE + INTERVAL;
		}
	}

}
void MouseControl(void)//成功输入两个移动坐标，为移动棋做准备//第一个值永远是Round，第二个值永远是空格或!Round
{
    if(MouseHit())
    {
         MOUSEMSG msg=GetMouseMsg();
         if(msg.uMsg==WM_LBUTTONDOWN)
         {
             int row=(msg.y-INTERVAL)/CHESS_SIZE;//转换像素点位置为数组位置
             int col=(msg.x-INTERVAL)/CHESS_SIZE;
             if(msg.x>map[row][col].x+30 && msg.y<map[row][col].y+30){col++;}
             if(msg.x<map[row][col].x+30 && msg.y>map[row][col].y+30){row++;}
             if(msg.x>map[row][col].x+30 && msg.y>map[row][col].y+30){row++;col++;}//微调修正

             if(Round==RED)//红方回合
             {
                 //begin.点击黑棋才报幕：这是啥回合
                 if(state==BEGIN&&col<9)//输入开始值//防止出界
                 {
                     if(map[row][col].type==BLACK)//点击黑棋报警
                     {
                         printf("这是红方回合!\n");
                     }
                     if(map[row][col].type==RED)//点击红棋输入，但是点击空格出不会输入
                     {
                        begin.x=row;
                        begin.y=col;
                        state=END;
                        end={-1,-1};//清空之前的数值
                     }
                 }
                else if(state==END&&col<9)//输入结束值
                {
                    end.x=row;
                    end.y=col;
                    if(map[end.x][end.y].type==RED)//结束值只能是空处or黑棋，否则重新输入结束值，并更改id
                    {
                        begin.x=end.x;
                        begin.y=end.y;
                        end={-1,-1};//清空之前的数值
                    }
                    else//满足要求，则结束
                        state=BEGIN;

                }
             }
             if(Round==BLACK)//黑方回合
             {
                if(state==BEGIN&&col<9)
                {
                    if(map[row][col].type==RED)
                      printf("这是黑方回合!\n");
                    if(map[row][col].type==BLACK)
                    {
                        begin.x=row;
                        begin.y=col;
                        state=END;
                        end={-1,-1};
                    }
                }
                else if(state==END&&col<9)
                {
                    end.x=row;
                    end.y=col;
                    if(map[end.x][end.y].type==BLACK)
                    {
                        begin.x=end.x;
                        begin.y=end.y;
                        end={-1,-1};
                    }
                    else
                        state=BEGIN;
                }
             }
             if(msg.x>730&&msg.x<910&&msg.y>400&&msg.y<500)//判断是否按下认输建
            {
                over=true;
                is_win=false;//标志游戏结束
            }
            if(msg.x>730&&msg.x<910&&msg.y>230&&msg.y<330)//判断是否按下悔棋键,完成数据转换
                ret=true;
         }
    }
}
void ChessMove(void)
{
    extern bool Guideline(enum PIECES chess);//同上，move规则函数
    bool canMove=false;//置为不能动
    cp_chess={-1,-1,SPACE,SPACE,false};//如果棋子不能移动，要把上次的清除，不然只要走法非法，值不变，一直显示吃
    //什么情况可以移动棋子
    if(begin.x!=-1&&end.x!=-1)
    {
        switch(map[begin.x][begin.y].id)//先考虑无其他棋子的情况，
        {
            //字母大写为黑方，小写为红方
            case PIECES::JU://車
                if((begin.x==end.x||begin.y==end.y)//只能横竖走
                   &&Guideline(JU))//再判断特殊条件
                        canMove=true;
                    break;
            case PIECES::ju:
                if((begin.x==end.x||begin.y==end.y)
                    &&Guideline(ju))
                        canMove=true;
                    break;
            case PIECES::MA://马
                if(((abs(end.x-begin.x)==1&&abs(end.y-begin.y)==2)||//走日
                   (abs(end.y-begin.y)==1&&abs(end.x-begin.x)==2))
                      &&Guideline(MA))
                            canMove=true;
                break;
            case PIECES::ma:
                    if(((abs(end.x-begin.x)==1&&abs(end.y-begin.y)==2)||
                       (abs(end.y-begin.y)==1&&abs(end.x-begin.x)==2))&&
                        Guideline(ma))//
                            canMove=true;
                break;
            case PIECES::XI://相
                    if(abs(end.x-begin.x)==2&&
                       abs(end.y-begin.y)==2
                        &&Guideline(XI))
                        canMove=true;
                    break;
            case PIECES::xi:
                    if(abs(end.x-begin.x)==2&&
                       abs(end.y-begin.y)==2&&
                       Guideline(xi))
                        canMove=true;
                    break;
            case PIECES::SI://士
                    if((abs(end.x-begin.x)==1&&
                       abs(end.y-begin.y)==1)&&
                       Guideline(SI))
                        canMove=true;
                    break;
            case PIECES::si:
                    if((abs(end.x-begin.x)==1&&
                       abs(end.y-begin.y)==1)&&
                      Guideline(si))
                        canMove=true;
                    break;
            case PIECES::JI://将
                    if(((abs(end.x-begin.x)==1&&end.y==begin.y)||//竖走一格
                        (abs(end.y-begin.y)==1&&end.x==begin.x))&&//横走一格
                        Guideline(JI))
                            canMove=true;
                    break;
            case PIECES::ji://帅
                    if(((abs(end.x-begin.x)==1&&end.y==begin.y)||//竖走一格
                        (abs(end.y-begin.y)==1&&end.x==begin.x))&&//横走一格
                        Guideline(ji))
                            canMove=true;
                    break;
            case PIECES::PA://炮
                    if((begin.x==end.x||begin.y==end.y)&&//車类似走法
                       Guideline(PA))
                         canMove=true;
                    break;
            case PIECES::pa:
                     if((begin.x==end.x||begin.y==end.y)&&
                        Guideline(pa))
                         canMove=true;
                    break;
                break;
            case PIECES::BI://兵
                    if(((abs(end.x-begin.x)==1&&end.y==begin.y)//跟将军走法类似
                        ||(abs(end.y-begin.y)==1&&end.x==begin.x))&&
                        Guideline(BI))
                            canMove=true;
                    break;
            case PIECES::bi:
                    if(((abs(end.x-begin.x)==1&&end.y==begin.y)
                        ||(abs(end.y-begin.y)==1&&end.x==begin.x))&&
                       Guideline(bi))
                            canMove=true;
                    break;
                break;
            default:
                    break;
        }
        if(canMove)
        {
            //移动之前
            //储存被吃的棋子
                cp_chess=map[end.x][end.y];
            //存储移动轨迹,用于悔棋
                if(Round==RED)//在底层数据交换之前，记录红方移动轨迹
                {
                    red_chess[1]=map[end.x][end.y];
                    red_chess[0]=map[begin.x][begin.y];
                }
                else
                {
                    black_chess[1]=map[end.x][end.y];
                    black_chess[0]=map[begin.x][begin.y];
                }
            //判断吃的棋子 分两种好报幕
                if(map[end.x][end.y].id!=SPACE&&map[end.x][end.y].type==RED)//被吃的棋子是红方时
                {
                    printf("红方棋子%s被黑方%s吃\n",chessName[map[end.x][end.y].id],chessName[map[begin.x][begin.y].id]);
                }
                if(map[end.x][end.y].id!=SPACE&&map[end.x][end.y].type==BLACK)
                {
                    printf("黑方棋子%s被红方%s吃\n",chessName[map[end.x][end.y].id],chessName[map[begin.x][begin.y].id]);
                }
                //判断是否将帅被吃
                if(map[end.x][end.y].id==JI||map[end.x][end.y].id==ji)
                    is_win=false;//将被吃，结束游戏
            //开始移动
                //过河
                if((Round==RED&&end.x<=4)||(Round==BLACK&&end.x>4))
                    map[begin.x][begin.y].river=true;
                //换ID
                map[end.x][end.y].id=map[begin.x][begin.y].id;
                map[begin.x][begin.y].id=SPACE;
                //换过河表示
                map[end.x][end.y].river=map[begin.x][begin.y].river;
                map[begin.x][begin.y].river=false;
                //换type
                map[end.x][end.y].type=map[begin.x][begin.y].type;
                map[begin.x][begin.y].type=SPACE;
                Round=Round==RED?BLACK:RED;//棋成功走完，本回合结束！
        }
    }
    if(ret)//悔棋算法
    {
                if(Round==RED)//红方想悔棋
                {
                    int row=(black_chess[1].y-INTERVAL)/CHESS_SIZE;
                    int col=(black_chess[1].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=black_chess[1];

                    row=(black_chess[0].y-INTERVAL)/CHESS_SIZE;
                    col=(black_chess[0].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=black_chess[0];

                    row=(red_chess[1].y-INTERVAL)/CHESS_SIZE;
                    col=(red_chess[1].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=red_chess[1];

                    row=(red_chess[0].y-INTERVAL)/CHESS_SIZE;
                    col=(red_chess[0].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=red_chess[0];
                    ret=false;
                }
                else//黑方想悔棋
                {
                    int row=(red_chess[1].y-INTERVAL)/CHESS_SIZE;
                    int col=(red_chess[1].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=red_chess[1];

                    row=(red_chess[0].y-INTERVAL)/CHESS_SIZE;
                    col=(red_chess[0].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=red_chess[0];

                    row=(black_chess[1].y-INTERVAL)/CHESS_SIZE;
                    col=(black_chess[1].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=black_chess[1];

                    row=(black_chess[0].y-INTERVAL)/CHESS_SIZE;
                    col=(black_chess[0].x-INTERVAL)/CHESS_SIZE;
                    map[row][col]=black_chess[0];

                    ret=false;
                }
    }
}
void GameDraw(void)
{
    putimage(0,0,&img);//必须不断的贴图，来覆盖之前的棋局
	putimage(725,0,&img2);
	//画按钮
	struct button *pButton1=createButton(730,230,910,330,(char*)"悔棋");//按钮大小以及内容
    struct button *pButton2=createButton(730,400,910,500,(char*)"认输");
    drawButton(pButton1,60,40,LIGHTCYAN);//文字内容的样式以及填充颜色
    drawButton(pButton2,60,40,LIGHTCYAN);
    //画棋子
	settextstyle(30,0,"隶书");
	setbkmode(0);
	for (int i=0;i<ROW;i++)
    {
        for(int k=0;k<COL;k++)
        {
            if(map[i][k].id!=SPACE)
            {
                //设置初始化
                setfillcolor(RGB(253,216,161));  //  253 216 161 *棋色 三原色设值
                setlinecolor(map[i][k].type);
                setlinestyle(PS_SOLID,3);
                settextcolor(map[i][k].type);
                //画同心圆
                fillcircle(map[i][k].x,map[i][k].y,30);
                fillcircle(map[i][k].x,map[i][k].y,25);
                //输出文本
                outtextxy(map[i][k].x-13,map[i][k].y-15,chessName[map[i][k].id]);
            }
        }
	}
	//美化点击事件
    if(begin.x!=-1&&end.x==-1)
    {
        setlinecolor(RGB(0,47,167) );
        setlinestyle(PS_SOLID,6);
        circle(map[begin.x][begin.y].x,map[begin.x][begin.y].y,35);
    }
	//吃的效果
    if(end.x!=-1&&begin.x!=-1&&cp_chess.id!=SPACE&&
       cp_chess.id!=JI&&cp_chess.id!=ji)//被吃后，到这里就是‘吃棋’的值了
    {
        setlinestyle(PS_SOLID,5);
        setlinecolor(cp_chess.type);
        settextcolor(cp_chess.type);
        circle(364,400,120);
        circle(364,400,100);
        settextstyle(120,90,"隶书");
        outtextxy(270,330,chessName[cp_chess.id]);
        if(Round==RED)
            settextcolor(BLACK);
        else
            settextcolor(RED);
        outtextxy(270,330,"吃");
    }
    if(over||!is_win)//认输
    {
         setlinestyle(PS_SOLID,5);
         settextstyle(120,90,"隶书");
         if(Round==BLACK)
        {
             setlinecolor(RED);
             settextcolor(RED);
             circle(364,400,120);
             circle(364,400,100);
             outtextxy(270,330,"胜");
             is_win=false;
        }
        else
        {
            setlinecolor(BLACK);
            settextcolor(BLACK);
            circle(364,400,120);
            circle(364,400,100);
            outtextxy(270,330,"胜");
            is_win=false;
        }
    }
    //释放，反正用不到
    freetem(pButton1);
    freetem(pButton2);
}

bool Guideline(enum PIECES chess)//传进来的值一定在无棋子的情况下，都能走
{
    int n=0;//炮能不能吃
    switch(chess)
    {
        case PIECES::JU:
        case PIECES::ju:
        if(end.x!=begin.x)//纵向
        {
            if(end.x<begin.x)//向上移动
            {
                for(int i=end.x+1;i<begin.x;i++)//主要判断不能跨棋子
                {
                    if(map[i][end.y].id!=SPACE)//中间有棋子当道
                    {
                        printf("走法不符合要求！\n");
                        end={-1,-1};
                        return false;
                    }

                }
                return true;
            }
            else
            {
                for(int i=end.x-1;i>begin.x;i--)
                {
                    if(map[i][end.y].id!=SPACE)
                    {
                        printf("走法不符合要求！\n");
                        end={-1,-1};
                        return false;
                    }
                }
                return true;
            }
        }
        else //横方向
        {
            if(end.y<begin.y)//向左移动
            {
                for(int i=end.y+1;i<begin.y;i++)
                {
                    if(map[end.x][i].id!=SPACE)
                    {
                        printf("走法不符合要求！\n");
                        end={-1,-1};
                        return false;
                    }

                }
                 return true;
            }
            else
            {
                for(int i=end.y-1;i>begin.y;i--)
                {
                    if(map[end.x][i].id!=SPACE)
                    {
                        printf("走法不符合要求！\n");
                        end={-1,-1};
                        return false;
                    }
                }
                return true;
            }
        }
            break;
        case PIECES ::MA:
        case PIECES::ma:
            if(end.x<begin.x)//向上
            {
                if(begin.x-end.x==2)
                {
                    if(map[begin.x-1][begin.y].id!=SPACE)
                    {
                         printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                    }
                }
                else
                {
                    if(begin.y-end.y==2)//左
                    {
                         if(map[begin.x][begin.y-1].id!=SPACE)
                         {
                              printf("走法不符合要求！\n");
                            //初始化点数
                            end ={-1,-1};
                            return false;
                         }
                    }
                    else//右
                    {
                        if(map[begin.x][begin.y+1].id!=SPACE)
                        {
                             printf("走法不符合要求！\n");
                            //初始化点数
                            end ={-1,-1};
                            return false;
                        }
                    }
                }
            }
            else
            {
                if(end.x-begin.x==2)
                {
                    if(map[begin.x+1][begin.y].id!=SPACE)
                    {
                         printf("走法不符合要求！\n");
                        end ={-1,-1};
                        return false;
                    }
                }
                else
                {
                    if(end.y-begin.y==2)//右
                    {
                        if(map[begin.x][begin.y+1].id!=SPACE)
                        {
                             printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                        }
                    }
                    else//左
                    {
                        if(map[begin.x][begin.y-1].id!=SPACE)
                        {
                        printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                        }
                    }
                }
            }
            return true;
            break;
        case PIECES::XI:
        case PIECES::xi:
            //不能过河
                if(end.x<=4&&map[begin.x][begin.y].type==RED)
                {
                     printf("走法不符合要求！\n");
                    //初始化点数
                    end ={-1,-1};
                    return false;
                }
                else if(end.x>=5&&map[begin.x][begin.y].type==BLACK)
                {
                     printf("走法不符合要求！\n");
                    //初始化点数
                    end ={-1,-1};
                    return false;
                }
                if(end.x<begin.x)//上
                {
                    if(end.y<begin.y)//左上
                    {
                        if(map[begin.x-1][begin.y-1].id!=SPACE)
                        {
                             printf("走法不符合要求！\n");
                            //初始化点数
                            end ={-1,-1};
                            return false;
                        }
                    }
                    else//右上
                    {
                        if(map[begin.x-1][begin.y+1].id!=SPACE)
                        {
                             printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                        }
                    }
                }
                else//下
                {
                     if(end.y<begin.y)//左下
                    {
                        if(map[begin.x+1][begin.y-1].id!=SPACE)
                        {
                             printf("走法不符合要求！\n");
                            //初始化点数
                            end ={-1,-1};
                            return false;
                        }
                    }
                    else//右下
                    {
                        if(map[begin.x+1][begin.y+1].id!=SPACE)
                        {
                             printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                        }
                    }
                }
                return true;
            break;
        case PIECES::JI:
        case PIECES::ji:
        case PIECES::SI:
        case PIECES::si:
                //不能出九宫格(排除所有错点，余下的就是正确点)
                if((end.y<3||end.y>5)||(end.x>2&&end.x<7))
                {
                            printf("走法不符合要求！\n");
                            //初始化点数
                            end ={-1,-1};
                            return false;
                }
                return true;
            break;
        case PIECES::PA:
        case PIECES::pa:
                if(end.x!=begin.x)//上下
                {
                    if(end.x<begin.x)//向上
                    {
                        for(int i=begin.x-1;i>end.x;i--)//顺序遍历不能反
                        {
                           if(map[i][begin.y].id!=SPACE) n++;
                        }
                        if(n==1||(n==0&&map[end.x][end.y].id==SPACE))
                            return true;
                        else
                        {
                             printf("走法不符合要求！\n");
                                    //初始化点数
                                    end={-1,-1};
                                    return false;
                        }
                    }
                    else//向下
                    {
                        for(int i=begin.x+1;i<end.x;i++)
                        {
                           if(map[i][begin.y].id!=SPACE)n++;
                        }
                        if(n==1||(n==0&&map[end.x][end.y].id==SPACE))
                            return true;
                        else
                        {
                             printf("走法不符合要求！\n");
                                    //初始化点数
                                    end={-1,-1};
                                    return false;
                        }
                    }
                }
                else//左右
                {
                    if(end.y<begin.y)//左移动
                    {
                        for(int i=begin.y-1;i>end.y;i--)
                        {
                            if(map[begin.x][i].id!=SPACE)n++;
                        }
                        if(n==1||(n==0&&map[end.x][end.y].id==SPACE))
                            return true;
                        else
                         {
                              printf("走法不符合要求！\n");
                                    //初始化点数
                                    end={-1,-1};
                                    return false;
                         }
                    }
                    else//右移动
                    {
                        for(int i=begin.y+1;i<end.y;i++)
                        {
                            if(map[begin.x][i].id!=SPACE)n++;
                        }
                        if(n==1||(n==0&&map[end.x][end.y].id==SPACE))
                            return true;
                        else
                        {
                             printf("走法不符合要求！\n");
                                    //初始化点数
                                    end={-1,-1};
                                    return false;
                        }
                    }
                }
            break;
        case PIECES::BI:
        case PIECES::bi://兵
            if(Round==RED)//红方回合
            {
                //不能后退
                if(map[begin.x][begin.y].river)//红方过河
                {
                    if(end.x>begin.x)
                    {
                        printf("走法不符要求！\n");
                        end={-1,-1};
                        return false;
                    }
                    else
                        return true;
                }
                else//未过河
                {
                    if(end.x<begin.x)
                        return true;
                    else
                    {
                         printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                    }
                    return false;
                }
            }
            else               //卒。。黑方回合
            {
                if(map[begin.x][begin.y].river)//过河能左右走，也能前进
                {
                    if(end.x<begin.x)
                    {
                         printf("走法不符要求！\n");
                        end={-1,-1};
                        return false;
                    }
                    else
                        return true;
                }
                else//未过河
                {
                    if(end.x>begin.x)
                        return true;
                    else
                    {
                         printf("走法不符合要求！\n");
                        //初始化点数
                        end ={-1,-1};
                        return false;
                    }
                }

            }
            break;
        default ://象征性规范编程
            break;
    }
    return false;
}

void  GameOver(void)
{
    if(!is_win)
    {
        Sleep(1000);
        if(Round==BLACK)
        {
            printf("红方胜！\n");
        }
        else
        {
            printf("黑方胜！\n");
        }
    }
    if(Round==BLACK)//对视
    {
        for(int i=0;i<3;i++)
            for(int j=3;j<6;j++)
            if(map[i][j].id==blackChess[4])//找到将位置
            {
                    for(int k=i+1;k<10;k++)
                    {
                        if(map[k][j].id!=SPACE)
                        {
                            if(map[k][j].id==redChess[4])
                            {
                                printf("黑方胜！\n");
                                is_win=false;
                                break;
                            }
                            else
                                break;
                        }
                    }
            }
    }
    if(Round==RED)
    {
        for(int i=7;i<10;i++)
            for(int j=3;j<6;j++)
            {
                if(map[i][j].id==redChess[4])
                {
                    for(int k=i-1;k>=0;k--)
                        if(map[k][j].id!=SPACE)
                        {
                            if(map[k][j].id==blackChess[4])
                            {
                                printf("红方胜!\n");
                                is_win=false;
                                break;
                            }
                            else
                                break;
                        }
                }
            }
    }
}
