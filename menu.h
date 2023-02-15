#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
struct button *createButton(int x,int y,int width,int height,char *pstr);
void drawButton(struct button * pButton,int height,int width,unsigned int color);
void freetem(struct button *pButton);
#endif // MENU_H_INCLUDED
