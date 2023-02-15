#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#define INTERVAL 44
#define CHESS_SIZE   80
#define ROW  10
#define COL  9
void GameInit(void);
void MouseControl(void);
void ChessMove(void);
void GameDraw(void);
bool Guideline();
void  GameOver(void);
int main_chess(void);
#endif // CHESS_H_INCLUDED
