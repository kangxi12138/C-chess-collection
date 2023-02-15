//吃 过路兵 ->特殊吃法*******
//白下A，黑上B*****
//王车易位//不得已有的bug*****能多次，且被将军时也能易位
//计算还剩多少棋子*******
#include<stdio.h>
#include<math.h>
#include<graphics.h>
#include"chess_two.h"
#include"menu.h"
bool is_win_chess_two;//判定游戏是否结束
bool is_trans;//是否可以进行王车易位
bool over_two;//用于在显示器上显示胜负
unsigned int Round_chess_two=WHITE;
struct judge//用于吃过路兵这个特殊规则
{
    int prx;
    int pry;
    bool is;
};
struct judge dead;
POINT begin_two={-1,-1},end_two={-1,-1};
IMAGE back_ground,bai_img,hei_img,biAh_img,biBh_img,chAh_img,chBh_img,waAh_img,
      waBh_img,maAh_img,maBh_img,xiAh_img,xiBh_img,
      hoAh_img,hoBh_img,biAb_img,biBb_img,chAb_img,chBb_img,
      waAb_img,waBb_img,maAb_img,maBb_img,xiAb_img,
      xiBb_img,hoAb_img,hoBb_img;
enum state_two
{   //黑是偶数 白是奇数
    hei,bai,
    biAh,biAb,chAh,chAb,maAh,maAb,xiAh,xiAb,hoAh,hoAb,waAh,waAb,//A是白方棋子（2-13）
    biBh,biBb,chBh,chBb,maBh,maBb,xiBh,xiBb,hoBh,hoBb,waBh,waBb,//B是黑方棋子（14-25）
    BEGIN,END
};
enum state_two be=BEGIN;
enum state_two  map_two[ROW_TWO][COL_TWO]={//赋值
    {chBb,maBh,xiBb,hoBh,waBb,xiBh,maBb,chBh},//0
    {biBh,biBb,biBh,biBb,biBh,biBb,biBh,biBb},//1
    {bai,hei,bai,hei,bai,hei,bai,hei},//2
    {hei,bai,hei,bai,hei,bai,hei,bai},//3
    {bai,hei,bai,hei,bai,hei,bai,hei},//4
    {hei,bai,hei,bai,hei,bai,hei,bai},//5
    {biAb,biAh,biAb,biAh,biAb,biAh,biAb,biAh},//6
    {chAh,maAb,xiAh,hoAb,waAh,xiAb,maAh,chAb},//7
};

int main_chess_two(void)
{
    initGame3();
    drawGame();
    BeginBatchDraw();
    while(is_win_chess_two)
    {
        if(Round_chess_two==WHITE&& is_win_chess_two)
        {
            printf("白方\n");
            while(Round_chess_two==WHITE&& is_win_chess_two)//白棋
            {
                playGame();
                move();
                drawGame();
                gameOver3();
                FlushBatchDraw();
            }
        }
        else
       {
           printf("黑方\n");
           while(Round_chess_two==BLACK&& is_win_chess_two)//黑棋
            {
                playGame();
                move();
                drawGame();
                gameOver3();
                FlushBatchDraw();
            }
       }
    }
    EndBatchDraw();
    Sleep(1000);
    closegraph();
    return 0;
}


void initGame3(void)
{
    is_win_chess_two=true;//游戏开始
    loadimage(&back_ground,"./res/back.PNG",200,800);
    initgraph(COL_TWO*SPACE+200,ROW_TWO*SPACE,1);
    loadimage(&bai_img,"./res/白.PNG",SPACE,SPACE,true);
    loadimage(&hei_img,"./res/黑.PNG",SPACE,SPACE,true);
    loadimage(&biAh_img,"./res/兵A黑.PNG",SPACE,SPACE,true);
    loadimage(&biBh_img,"./res/兵B黑.PNG",SPACE,SPACE,true);
    loadimage(&chAh_img,"./res/车A黑.PNG",SPACE,SPACE,true);
    loadimage(&chBh_img,"./res/车B黑.PNG",SPACE,SPACE,true);
    loadimage(&waAh_img,"./res/国王A黑.PNG",SPACE,SPACE,true);
    loadimage(&waBh_img,"./res/国王B黑.PNG",SPACE,SPACE,true);
    loadimage(&maAh_img,"./res/马A黑.PNG",SPACE,SPACE,true);
    loadimage(&maBh_img,"./res/马B黑.PNG",SPACE,SPACE,true);
    loadimage(&xiAh_img,"./res/象A黑.PNG",SPACE,SPACE,true);
    loadimage(&xiBh_img,"./res/象B黑.PNG",SPACE,SPACE,true);
    loadimage(&hoAh_img,"./res/后A黑.PNG",SPACE,SPACE,true);
    loadimage(&hoBh_img,"./res/后B黑.PNG",SPACE,SPACE,true);
    loadimage(&biAb_img,"./res/兵A白.PNG",SPACE,SPACE,true);
    loadimage(&biBb_img,"./res/兵B白.PNG",SPACE,SPACE,true);
    loadimage(&chAb_img,"./res/车A白.PNG",SPACE,SPACE,true);
    loadimage(&chBb_img,"./res/车B白.PNG",SPACE,SPACE,true);
    loadimage(&waAb_img,"./res/国王A白.PNG",SPACE,SPACE,true);
    loadimage(&waBb_img,"./res/国王B白.PNG",SPACE,SPACE,true);
    loadimage(&maAb_img,"./res/马A白.PNG",SPACE,SPACE,true);
    loadimage(&maBb_img,"./res/马B白.PNG",SPACE,SPACE,true);
    loadimage(&xiAb_img,"./res/象A白.PNG",SPACE,SPACE,true);
    loadimage(&xiBb_img,"./res/象B白.PNG",SPACE,SPACE,true);
    loadimage(&hoAb_img,"./res/后A白.PNG",SPACE,SPACE,true);
    loadimage(&hoBb_img,"./res/后B白.PNG",SPACE,SPACE,true);
}
void drawGame(void)//绘图
{
    int white_sum=0;
    int black_sum=0;//330
    putimage(800,0,&back_ground);
    //安装按钮
    struct button *pButton=createButton(820,350,980,430,(char*)"认输");
    drawButton(pButton,50,30,YELLOW);
    for(int i=0;i<ROW_TWO;i++)
        for(int j=0;j<COL_TWO;j++)
        {
            switch(map_two[i][j])
            {
            case bai:
                putimage(j*SPACE,i*SPACE,&bai_img);
                break;
            case hei:
                putimage(j*SPACE,i*SPACE,&hei_img);
                break;
            case biAh:
                white_sum+=10;
                 putimage(j*SPACE,i*SPACE,&biAh_img);
                break;
            case biBh:
                black_sum+=10;
                 putimage(j*SPACE,i*SPACE,&biBh_img);
                break;
            case chAh:
                white_sum+=40;
                 putimage(j*SPACE,i*SPACE,&chAh_img);
                break;
            case chBh:
                black_sum+=40;
                 putimage(j*SPACE,i*SPACE,&chBh_img);
                break;
            case waAh:
                white_sum+=10;
                 putimage(j*SPACE,i*SPACE,&waAh_img);
                break;
            case waBh:
                black_sum+=10;
                 putimage(j*SPACE,i*SPACE,&waBh_img);
                break;
            case maAh:
                white_sum+=20;
                 putimage(j*SPACE,i*SPACE,&maAh_img);
                break;
            case maBh:
                black_sum+=20;
                 putimage(j*SPACE,i*SPACE,&maBh_img);
                break;
            case xiAh:
                white_sum+=20;
                 putimage(j*SPACE,i*SPACE,&xiAh_img);
                break;
            case xiBh:
                black_sum+=20;
                 putimage(j*SPACE,i*SPACE,&xiBh_img);
                break;
            case hoAh:
                white_sum+=80;
                 putimage(j*SPACE,i*SPACE,&hoAh_img);
                break;
            case hoBh:
                black_sum+=80;
                 putimage(j*SPACE,i*SPACE,&hoBh_img);
                break;
            case biAb:
                white_sum+=10;
                 putimage(j*SPACE,i*SPACE,&biAb_img);
                break;
            case biBb:
                black_sum+=10;
                 putimage(j*SPACE,i*SPACE,&biBb_img);
                break;
            case chAb:
                white_sum+=40;
                 putimage(j*SPACE,i*SPACE,&chAb_img);
                break;
            case chBb:
                black_sum+=40;
                 putimage(j*SPACE,i*SPACE,&chBb_img);
                break;
            case waAb:
                white_sum+=10;
                 putimage(j*SPACE,i*SPACE,&waAb_img);
                break;
            case waBb:
                black_sum+=10;
                 putimage(j*SPACE,i*SPACE,&waBb_img);
                break;
            case maAb:
                white_sum+=20;
                 putimage(j*SPACE,i*SPACE,&maAb_img);
                break;
            case maBb:
                black_sum+=20;
                 putimage(j*SPACE,i*SPACE,&maBb_img);
                break;
            case xiAb:
                white_sum+=20;
                 putimage(j*SPACE,i*SPACE,&xiAb_img);
                break;
            case xiBb:
                black_sum+=20;
                 putimage(j*SPACE,i*SPACE,&xiBb_img);
                break;
            case hoAb:
                white_sum+=80;
                 putimage(j*SPACE,i*SPACE,&hoAb_img);
                break;
            case hoBb:
                black_sum+=80;
                 putimage(j*SPACE,i*SPACE,&hoBb_img);
                break;
            default:
                break;
            }
        }
    if(end_two.x==-1&&begin_two.x!=-1)//美化点击
    {
        setlinecolor(LIGHTCYAN);
        setlinestyle(PS_SOLID,4);
        rectangle(begin_two.y*SPACE,begin_two.x*SPACE,(begin_two.y+1)*SPACE,(begin_two.x+1)*SPACE);
    }
    //血量值
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID,6);
    //框框
    rectangle(820,10,980,340);
    rectangle(820,440,980,770);
    //黑方
    if(black_sum<=150)
        setfillcolor(RED);
    else if(black_sum<=250&&black_sum>150)
        setfillcolor(LIGHTGREEN);
    else
        setfillcolor(LIGHTCYAN);
    //画血量
    if(is_win_chess_two)//结束直接清零
        solidrectangle(820,10,980,10+black_sum);
    else
    {
        if(Round_chess_two==BLACK)
       {
            solidrectangle(820,10,980,10);
            black_sum=0;
       }
        else
        solidrectangle(820,10,980,10+black_sum);

    }
    //显示数字
    char *number=getNum(black_sum);
    settextcolor(BLACK);
    outtextxy(820,280,number);
    outtextxy(930,280,"%");
    //白方
    if(white_sum<=150)
        setfillcolor(RED);
    else if(white_sum<=250&&white_sum>150)
        setfillcolor(LIGHTGREEN);
    else
        setfillcolor(LIGHTCYAN);
    //画血量
    if(is_win_chess_two)
        solidrectangle(820,440+(330-white_sum),980,770);
    else
    {
        if(Round_chess_two==WHITE)
        {
            solidrectangle(820,770,980,770);
            white_sum=0;
        }
        else//已然技术
        solidrectangle(820,440+(330-white_sum),980,770);

    }
    //显示数字
    char *number2=getNum(white_sum);
    settextcolor(BLACK);
    outtextxy(820,460,number2);
    outtextxy(930,460,"%");
     if(!is_win_chess_two||over_two)//输了比赛
    {
         setlinestyle(PS_SOLID,5);
         settextstyle(120,90,"隶书");
         if(Round_chess_two==WHITE)
        {
             setlinecolor(BLACK);
             settextcolor(BLACK);
             circle(364,400,120);
             circle(364,400,100);
             outtextxy(270,330,"胜");
        }
        else
        {
            setlinecolor(WHITE);
            settextcolor(WHITE);
            circle(364,400,120);
            circle(364,400,100);
            outtextxy(270,330,"胜");
        }
        if(over_two)//认输
        is_win_chess_two=false;
    }


    //释放内存
    free(number2);
    free(number);
    freetem(pButton);
}
void playGame(void)//点击事件
{
    if(MouseHit())
    {
        MOUSEMSG msg=GetMouseMsg();
        if(msg.uMsg==WM_LBUTTONDOWN)
        {
            int row =msg.y/SPACE;
            int col =msg.x/SPACE;
            //白格子都是偶数，黑格子都是奇数
           if(Round_chess_two==WHITE)//白棋回合
           {

               if(be==BEGIN&&map_two[row][col]>=2&&map_two[row][col]<=13)//开始必须动白棋
                {
                    begin_two.x=row;
                    begin_two.y=col;
                    end_two={-1,-1};
                    be=END;
                }
                else if(be==END)//只能是空or黑棋
                {
                    end_two.x=row;
                    end_two.y=col;
                    //吃过路兵(判断白方过路兵是否被吃)
                    if(map_two[begin_two.x][begin_two.y]==biAb||map_two[begin_two.x][begin_two.y]==biAh)
                    {
                           if((end_two.x==4)&&begin_two.x==6&&
                              ((map_two[end_two.x][end_two.y-1]==biBb||map_two[end_two.x][end_two.y-1]==biBh)
                                ||(map_two[end_two.x][end_two.y+1]==biBh||map_two[end_two.x][end_two.y+1]==biBb)))//旁边要有兵
                           {
                                dead.prx=end_two.x;
                                dead.pry=end_two.y;
                           }
                           else
                           {
                               if(end_two.x==dead.prx-1&&end_two.y==dead.pry)
                                    dead.is=true;
                           }
                    }
                    //王车易位
                    if(map_two[ begin_two.x][begin_two.y]==waAh&&begin_two.x==7&&end_two.x==7&&
                       ((map_two[end_two.x][end_two.y]==chAh&&end_two.y==0)||
                        (map_two[end_two.x][end_two.y]==chAb&&end_two.y==7)))
                    {
                        //进行下一步判断
                        if(end_two.y==0)
                        {
                            int i;
                            for( i=1;i<4;i++)
                            {
                                if(map_two[7][i]>=2)
                                    break;
                            }
                            if(i==4)//没有棋子当道
                            {
                                is_trans=true;
                            }
                        }
                        if(end_two.y==7)
                        {
                             int i;
                             for( i=6;i>4;i--)
                            {
                                if(map_two[7][i]>=2)
                                    break;
                            }
                            if(i==4)
                            {
                                is_trans=true;
                            }
                        }
                    }
                    if(map_two[row][col]!=bai&&map_two[row][col]!=hei&&map_two[row][col]<=13&&//吃白方棋子时判断
                       !is_trans)//王车易位失败才开始删除数据
                    {
                        begin_two.x=end_two.x;
                        begin_two.y=end_two.y;
                        end_two={-1,-1};
                    }
                    else//成功输入
                    be=BEGIN;
                }

           }
           else//黑棋回合
            {
                 if(be==BEGIN&&map_two[row][col]!=bai&&map_two[row][col]!=hei&&map_two[row][col]>13)//必须动黑棋
                {
                        begin_two.x=row;
                        begin_two.y=col;
                        end_two={-1,-1};
                        be=END;
                }
                else if(be==END)
                {
                        end_two.x=row;
                        end_two.y=col;
                        //吃过路兵
                        if(map_two[begin_two.x][begin_two.y]==biBb||map_two[begin_two.x][begin_two.y]==biBh)
                       {
                            if((end_two.x==3)&&begin_two.x==1&&
                               ((map_two[end_two.x][end_two.y-1]==biAb||map_two[end_two.x][end_two.y-1]==biAh)
                            ||(map_two[end_two.x][end_two.y+1]==biAh||map_two[end_two.x][end_two.y+1]==biAb)))//旁边要有兵
                            {
                                dead.prx=end_two.x;
                                dead.pry=end_two.y;
                            }
                            else//吃白棋
                            {
                                if(end_two.x==dead.prx+1&&end_two.y==dead.pry)
                                    dead.is=true;
                            }
                       }
                       else
                       {
                           dead={-1,-1,false};
                       }
                        //王车易位
                        if(map_two[ begin_two.x][begin_two.y]==waBb&&begin_two.x==0&&end_two.x==0&&
                       ((map_two[end_two.x][end_two.y]==chBh&&end_two.y==7)||
                        (map_two[end_two.x][end_two.y]==chBb&&end_two.y==0)))
                        {
                            if(end_two.y==0)
                            {int i;
                            for( i=1;i<4;i++)
                            {
                                if(map_two[0][i]>=2)
                                    break;
                            }
                            if(i==4)//没有棋子当道
                            {
                                is_trans=true;
                            }
                            }
                            if(end_two.y==7)
                            {
                                int i;
                             for( i=6;i>4;i--)
                            {
                                if(map_two[0][i]>=2)
                                    break;
                            }
                            if(i==4)
                            {
                                is_trans=true;
                            }
                            }
                        }
                        //读值
                        if(map_two[row][col]!=bai&&map_two[row][col]!=hei&&map_two[row][col]>13&&
                           !is_trans)
                        {
                            begin_two.x=end_two.x;
                            begin_two.y=end_two.y;
                            end_two={-1,-1};
                        }
                        else
                            be=BEGIN;
                }
            }
            //判断是否按下认输建
            if(msg.x>820&&msg.y>360&&msg.x<980&&msg.y<460)
            {
                over_two=true;
                is_win_chess_two=false;
            }
        }
    }
}
void move(void)//在无其他棋子情况下的移动
{
    extern bool guidance(enum state_two pieces);
    bool canmove=false;
    bool cantran=false;//变身
    if(begin_two.x!=-1&&end_two.y!=-1)
    {
        switch (map_two[begin_two.x][begin_two.y])
        {
            case biAh://兵
            case biAb:
                 if(((begin_two.x-end_two.x==1||(begin_two.x-end_two.x==2&&begin_two.x==6))
                     &&begin_two.y==end_two.y&&guidance(biAb))||//向上走
                    (begin_two.x-end_two.x==1&&abs(begin_two.y-end_two.y)==1
                     &&map_two[end_two.x][end_two.y]>1))//向左上走//向右上走
                   {
                        canmove=true;
                        if(end_two.x==0)
                             cantran=true;
                    }
                break;
            case biBh:
            case biBb:
                  if(((end_two.x-begin_two.x==1||(end_two.x-begin_two.x==2&&begin_two.x==1))
                      &&begin_two.y==end_two.y&&guidance(biBb))||//向下走
                    (end_two.x-begin_two.x==1&&abs(begin_two.y-end_two.y)==1
                     &&map_two[end_two.x][end_two.y]>1))//向左下走//向右下走
                    {
                        canmove=true;
                        if(end_two.x==7)
                             cantran=true;
                    }
                break;
            case chAh://车
            case chAb:
            case chBh:
            case chBb:
                if((begin_two.x==end_two.x||begin_two.y==end_two.y)&&guidance(chBb))
                        canmove=true;
                break;
            case maAh:
            case maAb:
            case maBh:
            case maBb:
                 if(((abs(end_two.x-begin_two.x)==1&&abs(end_two.y-begin_two.y)==2)||
                   (abs(end_two.y-begin_two.y)==1&&abs(end_two.x-begin_two.x)==2)))
                            canmove=true;
                break;
            case xiAb:
            case xiAh:
            case xiBh:
            case xiBb:
                  if(abs(begin_two.x-end_two.x)==abs(begin_two.y-end_two.y)&&guidance(xiBb))
                            canmove=true;
                break;
            case hoAb:
            case hoAh:
            case hoBb:
            case hoBh:
                if(((begin_two.x==end_two.x||begin_two.y==end_two.y)&&guidance(chBb))||
                   (abs(begin_two.x-end_two.x)==abs(begin_two.y-end_two.y)&&guidance(xiBb)))
                        canmove=true;
                break;
            case waAb:
            case waAh:
            case waBh:
            case waBb:
                if((abs(begin_two.x-end_two.x)==1||abs(begin_two.y-end_two.y)==1)&&guidance(xiBb)&&guidance(chBb))
                    canmove=true;
                break;
            default:
                break;
        }
            if(canmove&&!is_trans)//王车易位期间，必须避让
            {
                if(map_two[end_two.x][end_two.y]==waAb||map_two[end_two.x][end_two.y]==waAh||
                   map_two[end_two.x][end_two.y]==waBb||map_two[end_two.x][end_two.y]==waBh)
                    is_win_chess_two=false;//将王，游戏结束
            //白格开始走
                if(map_two[begin_two.x][begin_two.y]%2==1)
                {
                    if(map_two[end_two.x][end_two.y]%2==0)//走到黑
                    {
                        map_two[end_two.x][end_two.y]=(enum state_two)(map_two[begin_two.x][begin_two.y]-1);
                        map_two[begin_two.x][begin_two.y]=bai;
                    }
                    else//走到白
                    {
                        map_two[end_two.x][end_two.y]=map_two[begin_two.x][begin_two.y];
                        map_two[begin_two.x][begin_two.y]=bai;
                    }
                }
            //黑格开始走
                else if(map_two[begin_two.x][begin_two.y]%2==0)
                {
                    if(map_two[end_two.x][end_two.y]%2==1)//走到白
                    {
                        map_two[end_two.x][end_two.y]=(enum state_two)(map_two[begin_two.x][begin_two.y]+1);
                        map_two[begin_two.x][begin_two.y]=hei;
                    }
                    else//走到黑
                    {
                        map_two[end_two.x][end_two.y]=map_two[begin_two.x][begin_two.y];
                        map_two[begin_two.x][begin_two.y]=hei;
                    }
                }

                //换局
                Round_chess_two=Round_chess_two==WHITE? BLACK:WHITE;
            }
            if(is_trans)//王车易位
            {
               if(end_two.y==0)//更左边换
               {
                   if(end_two.x==0)//左上
                   {
                       //王动
                       map_two[0][2]=map_two[begin_two.x][begin_two.y];
                       map_two[begin_two.x][begin_two.y]=bai;
                       //车动
                       map_two[0][3]=(enum state_two)(map_two[end_two.x][end_two.y]-1);
                       map_two[end_two.x][end_two.y]=bai;
                   }
                   else//左下
                   {
                       map_two[7][2]=map_two[begin_two.x][begin_two.y];
                       map_two[begin_two.x][begin_two.y]=hei;

                       map_two[7][3]=(enum state_two)(map_two[end_two.x][end_two.y]+1);
                       map_two[end_two.x][end_two.y]=hei;
                   }
               }
               else//更右边换
               {
                   if(end_two.x==0)//右上
                   {
                       map_two[0][6]=map_two[begin_two.x][begin_two.y];
                       map_two[begin_two.x][begin_two.y]=bai;

                       map_two[0][5]=map_two[end_two.x][end_two.y];
                       map_two[end_two.x][end_two.y]=hei;
                   }
                   else//右下
                   {
                       map_two[7][6]=map_two[begin_two.x][begin_two.y];
                       map_two[begin_two.x][begin_two.y]=hei;

                       map_two[7][5]=map_two[end_two.x][end_two.y];
                       map_two[end_two.x][end_two.y]=bai;
                   }
               }
                //换局
                Round_chess_two=Round_chess_two==WHITE? BLACK:WHITE;
            }
            is_trans=false;//结束王车易位
            if(cantran)//变身
            {
                if(map_two[end_two.x][end_two.y]%2==0)//黑格子
                {
                    if(end_two.x==0)
                    {
                        map_two[end_two.x][end_two.y]=hoAh;
                    }
                    else
                    {
                        map_two[end_two.x][end_two.y]=hoBh;
                    }
                }
                else//白格子
                {
                    if(end_two.x==0)
                    {
                        map_two[end_two.x][end_two.y]=hoAb;
                    }
                    else
                    {
                        map_two[end_two.x][end_two.y]=hoBb;
                    }
                }
            }
            //吃过路兵
            if(dead.is)
            {
                //白格开始走
                if(map_two[begin_two.x][begin_two.y]%2==1)
                {
                    if(map_two[end_two.x][end_two.y]%2==0)//走到黑
                    {
                        map_two[end_two.x][end_two.y]=(enum state_two)(map_two[begin_two.x][begin_two.y]-1);
                        map_two[begin_two.x][begin_two.y]=bai;
                    }
                    else//走到白
                    {
                        map_two[end_two.x][end_two.y]=map_two[begin_two.x][begin_two.y];
                        map_two[begin_two.x][begin_two.y]=bai;
                    }
                }
            //黑格开始走
                else if(map_two[begin_two.x][begin_two.y]%2==0)
                {
                    if(map_two[end_two.x][end_two.y]%2==1)//走到白
                    {
                        map_two[end_two.x][end_two.y]=(enum state_two)(map_two[begin_two.x][begin_two.y]+1);
                        map_two[begin_two.x][begin_two.y]=hei;
                    }
                    else//走到黑
                    {
                        map_two[end_two.x][end_two.y]=map_two[begin_two.x][begin_two.y];
                        map_two[begin_two.x][begin_two.y]=hei;
                    }
                }
                //
                if(map_two[dead.prx][dead.pry]==biAb||map_two[dead.prx][dead.pry]==biBb)
                    map_two[dead.prx][dead.pry]=bai;
                else if(map_two[dead.prx][dead.pry]==biAh||map_two[dead.prx][dead.pry]==biBh)
                    map_two[dead.prx][dead.pry]=hei;
                //换局
                Round_chess_two=Round_chess_two==WHITE? BLACK:WHITE;
                //重置
                dead={-1,-1,false};
            }
   }
}
bool guidance(enum state_two pieces)//移动规则
{
    switch(pieces)
    {
    case biAb:
    case biAh:
    case biBh:
    case biBb:
        if(map_two[end_two.x][end_two.y]==bai||map_two[end_two.x][end_two.y]==hei)//直走情况
            return true;
        return false;
        break;
    case chAb:
    case chAh:
    case chBh:
    case chBb:
        if(end_two.x!=begin_two.x)//纵向
        {
            if(end_two.x<begin_two.x)//向上
            {
                for(int i=end_two.x+1;i<begin_two.x;i++)
                {
                    if(map_two[i][end_two.y]>1)
                    {
                         printf("走法不符合要求！\n");
                         end_two={-1,-1};
                         return false;
                    }
                }
                return true;
            }
            else//向下
            {
                for(int i=end_two.x-1;i>begin_two.x;i--)
                {
                    if(map_two[i][end_two.y]>1)
                    {
                        printf("走法不符合要求！\n");
                        end_two={-1,-1};
                        return false;
                    }
                }
                return true;
            }
        }
        else //横方向
        {
            if(end_two.y<begin_two.y)//向左
            {
                for(int i=end_two.y+1;i<begin_two.y;i++)
                {
                    if(map_two[end_two.x][i]>1)
                    {
                        printf("走法不符合要求！\n");
                        end_two={-1,-1};
                        return false;
                    }
                }
                 return true;
            }
            else//向右
            {
                for(int i=end_two.y-1;i>begin_two.y;i--)
                {
                   if(map_two[end_two.x][i]>1)
                    {
                        printf("走法不符合要求！\n");
                        end_two={-1,-1};
                        return false;
                    }
                }
                return true;
            }
        }
        break;
    case xiAb:
    case xiAh:
    case xiBh:
    case xiBb:
        //有阻碍情况
        if(end_two.x<begin_two.x)//上方
        {
            if(end_two.y<begin_two.y)//左上
            {
                for(int i=1;i<abs(begin_two.x-end_two.x);i++)
                {
                    if(map_two[end_two.x+i][end_two.y+i]>1)
                    {
                        printf("走法不符合要求！\n");
                         end_two={-1,-1};
                         return false;
                    }
                }
                return true;
            }
            else//右上
            {
                for(int i=1;i<abs(begin_two.x-end_two.x);i++)
                {
                    if(map_two[end_two.x+i][end_two.y-i]>1)
                    {
                        printf("走法不符合要求！\n");
                         end_two={-1,-1};
                         return false;
                    }
                }
                 return true;
            }
        }
        else//下方
        {
            if(end_two.y<begin_two.y)//左下
            {
                for(int i=1;i<abs(begin_two.x-end_two.x);i++)
                {
                    if(map_two[end_two.x-i][end_two.y+i]>1)
                    {
                        printf("走法不符合要求！\n");
                         end_two={-1,-1};
                         return false;
                    }
                }
                 return true;
            }
            else//右下
            {
                for(int i=1;i<abs(begin_two.x-end_two.x);i++)
                {
                    if(map_two[end_two.x-i][end_two.y-i]>1)
                    {
                         printf("走法不符合要求！\n");
                         end_two={-1,-1};
                         return false;
                    }
                }
                 return true;
            }
        }
        break;
    default :
        break;
    }
}
void  gameOver3(void)
{
   if(!is_win_chess_two)
   {
       if(Round_chess_two==WHITE)
        printf("黑棋胜");
       else
        printf("白棋胜");
   }
}
char *getNum(int sum)
{
    char num[5];
    char*p=(char*)malloc(5*sizeof(char));
    char *pn=&num[3];
    char *pc=p;
    num[4]='\0';
    if(sum==0)
    {
        *pn=sum%10+48;
        pn--;
    }
    while(sum!=0)
    {
        *pn=sum%10+48;
        sum/=10;
        pn--;
    }
    pn++;
    while(*pn)
    {
        *pc++=*pn++;
    }
    *pc='\0';
    return p;

}
