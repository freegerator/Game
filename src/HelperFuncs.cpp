
#include <ClassDefs.h>

void setRect(struct SDL_Rect *rect, int x, int y, int w, int h)
{
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

bool isRectCollision(SDL_Rect *rect1, SDL_Rect *rect2)
{
	int xx1 = rect1->x + rect1->w;
	int yy1 = rect1->y + rect1->h;
	int xx2 = rect2->x + rect2->w;
	int yy2 = rect2->y + rect2->h;

	return ((rect1->x <= rect2->x && rect2->x <= xx1) || 
		    (rect1->x <= xx2 && xx2 <= xx1)) &&
	       ((rect1->y <= rect2->y && rect2->y <= yy1)|| 
			(rect1->y <= yy2 && yy2 <= yy1));
}
