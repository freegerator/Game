#include <ClassDefs.h>
#include <tr1/unordered_map>

std::tr1::unordered_map<std::string, int> my_map;

int gsystem::width()
{
	refreshSizes();
	return screenWidth;
}

int gsystem::height()
{
	refreshSizes();
	return screenHeight;
}

void gsystem::drawImage(SDL_Texture *img, int x, int y, bool overlap, bool centered)
{
	int imagew, imageh, xpos, ypos;
	SDL_Rect rct;

	SDL_QueryTexture(img, NULL, NULL, &imagew, &imageh);

	refreshSizes();


	if (centered)
	{
		xpos = x - imagew / 2;
		ypos = y - imageh / 2;
	}
	else
	{
		xpos = x;
		ypos = y;
	}

	if (xpos < 0)
	{
		xpos = (xpos % screenWidth) + screenWidth;
	}
	else
	{
		xpos = xpos % screenWidth;
	}

	if (ypos < 0)
	{
		ypos = (ypos % screenHeight) + screenHeight;
	}
	else
	{
		ypos = ypos % screenHeight;
	}

	setRect(&rct, xpos, ypos, imagew, imageh);
	drawImage(img, &rct);

	if (overlap)
	{
		if (xpos > screenWidth - imagew)
		{
			setRect(&rct, xpos - screenWidth, ypos, imagew, imageh);
			drawImage(img, &rct);
		}

		if (ypos > screenHeight - imageh)
		{
			setRect(&rct, xpos, ypos - screenHeight, imagew, imageh);
			drawImage(img, &rct);
		}

		if (ypos > screenHeight - imageh && xpos > screenWidth - imagew)
		{
			setRect(&rct, xpos - screenWidth, ypos - screenHeight, imagew, imageh);
			drawImage(img, &rct);
		}
	}
}


gsystem::gsystem(DWORD flags)
{
	int retCode = 0;

	dbglog("init'ing vars");
	flagrantSystemError = 0;
	gameWindow = NULL;
	gameRenderer = NULL;
	gameEvent = NULL;
	screenWidth = screenHeight = 0;

	dbglog("Init SDL");
	retCode = SDL_Init(SDL_INIT_EVERYTHING);

	if (retCode != 0)
	{
		dbglog("Flagrant System Error: %x", retCode);
		flagrantSystemError = true;
		return;
	}

	if (flags & GSYSTEM_FULLSCREEN)
	{
		dbglog("creating fullscreen window");
		gameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		dbglog("creating small window");
		gameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	}
	if (gameWindow == NULL)
	{
		dbglog("Flagrant System Error: Could not open window");
		flagrantSystemError = true;
		return;
	}

	dbglog("creating renderer");
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

	if (gameRenderer == NULL)
	{
		dbglog("Flagrant System Error: Could not create renderer");
		flagrantSystemError = true;
		return;
	}

	dbglog("creating game event");
	gameEvent = new SDL_Event;

	if (NULL == gameEvent)
	{
		dbglog("Flagrant System Error: Could not create event");
		flagrantSystemError = true;
		return;
	}
}

gsystem::~gsystem()
{
	if (gameEvent)
	{
		delete gameEvent;
		gameEvent = NULL;
	}
	if (gameRenderer)
	{
		SDL_DestroyRenderer(gameRenderer);
		gameRenderer = NULL;
	}

	if (gameWindow)
	{
		SDL_DestroyWindow(gameWindow);
		gameWindow = NULL;
	}
	SDL_Quit();
}

bool gsystem::boned()
{
	if (flagrantSystemError)
	{
		dbglog("Declaring boned due to flagrant system error");
		return true;
	}
	if (gameEvent->type == SDL_QUIT)
	{
		return true;
	}
	return false;
}

void gsystem::prep()
{
	SDL_RenderClear(gameRenderer);
}

void gsystem::update()
{
	SDL_RenderPresent(gameRenderer);
	SDL_PollEvent(gameEvent);
}

void gsystem::drawImage(SDL_Texture *img, SDL_Rect *rct)
{
	SDL_RenderCopy(gameRenderer, img, NULL, rct);
}

void gsystem::drawBG(SDL_Texture *bgImg)
{
	refreshSizes();
	SDL_Rect wholeScreen;

	setRect(&wholeScreen, 0, 0, screenWidth, screenHeight);
	drawImage(bgImg, &wholeScreen);
}

void gsystem::refreshSizes()
{
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);
}

SDL_Texture* gsystem::loadImage(char *str)
{
	char *filename = NULL;
	SDL_Texture* retVal = NULL;
	filename = (char*)malloc(strlen(str) + IMAGE_LOC_LEN + 1);
	if (filename == NULL)
	{
		flagrantSystemError = true;
		dbglog("Bad malloc");
		return NULL;
	}
	strcpy(filename, IMAGE_LOC);
	strcat(filename, str);

	retVal = IMG_LoadTexture(gameRenderer, filename);
	free(filename);
	filename = NULL;
	if (retVal == NULL)
	{
		dbglog("Could not load image");
	}
	return retVal; /*might be NULL if image failed to load.  Do not consider catastrophic*/
}
