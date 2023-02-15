#include<stdio.h>//不能省略
#include<graphics.h>//不能省略
//页面按钮制作
struct button//全局结构体
{
    int x;//按钮位置
    int y;
    int width;//按钮宽度
    int height;//按钮高度
    char *pstr;//按钮内容
};
struct button *createButton(int x,int y,int width,int height,char *pstr)//创建按钮
{
     struct button* pButton=(struct button *)malloc(sizeof(struct button));
     pButton->x=x;
     pButton->y=y;
     pButton->width=width;
     pButton->height=height;
     pButton->pstr=(char*)malloc(strlen(pstr)+1);
     strcpy(pButton->pstr,pstr);
     return  pButton;
};
void drawButton(struct button * pButton,int height,int width,unsigned int color)//画按钮
{
    setbkmode(0);//这个函数用于设置当前设备图案填充和文字输出时的背景模式。
    setfillcolor(color);//按钮填充颜色
    settextstyle(height,width,"隶书");//设置文字输出格式
    settextcolor(RED);
    setlinecolor(BLUE);
    setlinestyle(PS_SOLID,4);
    fillrectangle(pButton->x, pButton->y,pButton->width, pButton->height);
    outtextxy(pButton->x+20,pButton->y+17,pButton->pstr);
};
void freetem(struct button *pButton)//释放内存
{
    free(pButton->pstr);
    free(pButton);
}
