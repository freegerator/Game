#include <ClassDefs.h>

gameObject::gameObject()
{
	objImg = NULL;
}

gameObject::~gameObject()
{
	if (objImg)
	{
		SDL_DestroyTexture(objImg);
		objImg = NULL;
	}
}
























