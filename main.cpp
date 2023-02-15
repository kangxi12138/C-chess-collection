#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<math.h>
#include<string.h>
#include"chess.h"
#include"gomoku.h"
#include"chess_two.h"
#include"menu.h"
int button_xy[][4]={{250,50,650,150},{350,140,550,230},{350,260,550,350},{350,380,550,470},{350,500,550,590}};//四个按钮的位置
int button_judge(int x,int y)//测试鼠标信号是否在按钮内
{
    if(x>button_xy[1][0]&&y>button_xy[1][1]&&x<button_xy[1][2]&&y<button_xy[1][3])return 1;//点击第一个按钮
    if(x>button_xy[2][0]&&y>button_xy[2][1]&&x<button_xy[2][2]&&y<button_xy[2][3])return 2;//点击第二个按钮
    if(x>button_xy[3][0]&&y>button_xy[3][1]&&x<button_xy[3][2]&&y<button_xy[3][3])return 3;//点击第三个按钮
    if(x>button_xy[4][0]&&y>button_xy[4][1]&&x<button_xy[4][2]&&y<button_xy[4][3])return 4;//点击第四个按钮
    return 0;//未点击按钮
}
int main(void)
{
    //游戏开始
    int width=900,height=600,flag;//设置显示窗口的大小
    IMAGE img;//图片对象
    while(true)//可重复游戏
    {
        bool quit=true;
        initgraph(width,height);//创建窗口
        //贴图
        BeginBatchDraw();//这个函数用于开始批量绘图。执行后，任何绘图操作都将暂时不输出到绘图窗口上
        loadimage(&img,"./res/封面.PNG",900,600);//加载图片对象，这个函数用于从文件中读取图像。
        putimage(0,0,&img);//这个函数的几个重载用于在当前设备上绘制指定图像。
        //设置按钮属性
        //按钮大小以及内容
        struct button *pTitel  =createButton(button_xy[0][0],button_xy[0][1],button_xy[0][2],button_xy[0][3],(char*)"通讯录");
        struct button *pButton1=createButton(button_xy[1][0],button_xy[1][1],button_xy[1][2],button_xy[1][3],(char*)"象棋");
        struct button *pButton2=createButton(button_xy[2][0],button_xy[2][1],button_xy[2][2],button_xy[2][3],(char*)"五子棋");
        struct button *pButton3=createButton(button_xy[3][0],button_xy[3][1],button_xy[3][2],button_xy[3][3],(char*)"国际象棋");
        struct button *pButton4=createButton(button_xy[4][0],button_xy[4][1],button_xy[4][2],button_xy[4][3],(char*)"退出");
        //画按钮
        drawButton(pTitel,60,40,YELLOW);//文字内容的样式以及按钮填充颜色
        drawButton(pButton1,60,40,LIGHTCYAN);
        drawButton(pButton2,50,28,LIGHTCYAN);
        drawButton(pButton3,50,20,LIGHTCYAN);
        drawButton(pButton4,60,40,LIGHTCYAN);
        //开始点击
        while(quit)
        {
            MOUSEMSG msg=GetMouseMsg();//获取鼠标消息
            switch(msg.uMsg)
            {
                case WM_MOUSEMOVE://鼠标移动事件
                    if(button_judge(msg.x,msg.y))
                    {
                        if(button_judge(msg.x,msg.y)==1)
                            drawButton(pButton1,60,40,LIGHTGRAY);//改变颜色
                        if(button_judge(msg.x,msg.y)==2)
                            drawButton(pButton2,50,28,LIGHTGRAY);
                        if(button_judge(msg.x,msg.y)==3)
                            drawButton(pButton3,50,20,LIGHTGRAY);
                        if(button_judge(msg.x,msg.y)==4)
                            drawButton(pButton4,60,40,LIGHTGRAY);
                        flag=button_judge(msg.x,msg.y);//记录被改变颜色的按钮
                    }
                    else//还原按钮原来模样
                    {
                        if(flag==1)
                        {
                            drawButton(pButton1,60,40,LIGHTCYAN);
                            flag=0;
                        }
                        if(flag==2)
                        {
                            drawButton(pButton2,50,28,LIGHTCYAN);
                            flag=0;
                        }
                        if(flag==3)
                        {
                            drawButton(pButton3,50,20,LIGHTCYAN);
                            flag=0;
                        }
                        if(flag==4)
                        {
                            drawButton(pButton4,60,40,LIGHTCYAN);
                            flag=0;
                        }
                    }
                    break;
                case WM_LBUTTONDOWN://鼠标点击事
                    switch(button_judge(msg.x,msg.y))//判断是否点击了按钮
                    {
                        case 1:main_chess();FlushMouseMsgBuffer();quit=false;break;
                        case 2:main_gomoku();FlushMouseMsgBuffer();quit=false;break;
                        case 3:main_chess_two();FlushMouseMsgBuffer();quit=false;break;
                        case 4:closegraph();//关闭绘图环境
                                exit(0);//正常退出
                        default:
                            FlushMouseMsgBuffer();//单击事件后清空鼠标消息
                            //打印鼠标坐标，方便调试时确定区域
                            break;
                    }
                    break;
                default :
                    break;
            }
            FlushBatchDraw();
        }
        EndBatchDraw();
        freetem(pButton1);//释放内存
        freetem(pTitel);
        freetem(pButton2);
        freetem(pButton3);
        freetem(pButton4);
        closegraph();
}

}






