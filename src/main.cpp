
#include "ClassDefs.h"
#include <time.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SDL_Texture* grass_image = NULL;
	SDL_Texture* duder = NULL;
	SDL_Rect dudeRect;
	int counter = 0;
	int x,y;
	srand(time(0));
	gsystem mySys(0);

	if (mySys.boned())
	{
		dbglog("early bonedness");
		goto cleanup;
	}

	grass_image = mySys.loadImage("background.bmp");
	duder = mySys.loadImage("dude.bmp");

	if (!grass_image || !duder)
	{
		dbglog("images failed to load %p %p", grass_image, duder);
		goto cleanup;
	}
	
	x = 320;
	y = 240;

	while (!mySys.boned())
	{
		mySys.prep();
		mySys.drawBG(grass_image);
		
		//mySys.drawImage(duder, x, y, true, true);
		mySys.update();
	}

	dbglog("Done, cleaning up");

cleanup:
	if (grass_image)
	{
		SDL_DestroyTexture(grass_image);
	}
	if (duder)
	{
		SDL_DestroyTexture(duder);
	}

	return 0;
}