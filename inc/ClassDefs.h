#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "SDL.h"
#include "SDL_Image.h"

#define dbglog(...) {char dbgstr[MAX_PATH]; sprintf(dbgstr, ## __VA_ARGS__); OutputDebugStringA(dbgstr);}

#define IMAGE_LOC "..\\..\\images\\"
#define IMAGE_LOC_LEN 13

/* ERROR CODES */
#define SUCCESS 0
#define E_INIT 1

/* FLAGS */
#define GSYSTEM_FULLSCREEN 1

/* Helper function prototypes */
void setRect(struct SDL_Rect *rect, int x, int y, int w, int h);

bool isRectCollision(SDL_Rect *rect1, SDL_Rect *rect2);

/* Game System Prototype */

class gsystem
{
public:
	gsystem(DWORD flags);
	~gsystem();
	void drawImage(SDL_Texture*, SDL_Rect*);
	void drawImage(SDL_Texture *img, int x, int y, bool overlap, bool centered);
	void drawBG(SDL_Texture*);
	bool boned();
	SDL_Texture *loadImage(char*);
	void prep();
	void update();
	int width();
	int height();
private:
	DWORD flagrantSystemError;
	SDL_Window *gameWindow;
	SDL_Renderer *gameRenderer;
	SDL_Event *gameEvent;
	int screenWidth, screenHeight;
	void refreshSizes();
};

class gameObject
{
public:
	gameObject();
	~gameObject();
private:
	SDL_Texture *objImg;
	DWORD code;
};