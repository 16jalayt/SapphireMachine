#include "Cursor.h"
#include <loguru.hpp>
#include <SDL2/SDL_image.h>
#include "Engine/Utils.h"

SDL_Cursor_ptr Cursor::cursors[30];
int Cursor::currentCursor = 0;
int Cursor::baseCursor = 0;
bool Cursor::CursorChanged = false;
SDL_Surface_ptr Cursor::spriteSheet;
std::string Cursor::spriteSheetPath;

SDL_Cursor_ptr Cursor::loadCursorFromSheet(std::string filename, SDL_Rect rect)
{
	//Cache sprite sheet to improve performance
	if (spriteSheetPath != filename)
	{
		spriteSheetPath = filename;
		spriteSheet = SDL_Surface_ptr(IMG_Load(filename.c_str()));
	}
	if (!spriteSheet)
	{
		LOG_F(ERROR, "Unable to load cursor Sprite Sheet: %s.\n", filename.c_str());
		return NULL;
	}

	SDL_Surface* sprite = SDL_CreateRGBSurface(spriteSheet->flags, rect.w, rect.h, spriteSheet->format->BitsPerPixel, spriteSheet->format->Rmask, spriteSheet->format->Gmask, spriteSheet->format->Bmask, spriteSheet->format->Amask);
	//SDL_Surface* sprite = IMG_Load(filename.c_str());
	if (!sprite)
	{
		LOG_F(ERROR, "Unable to load cursor.\n");
		return NULL;
	}

	SDL_BlitSurface(spriteSheet.get(), &rect, sprite, NULL);

	//SDL_FreeSurface(spriteSheet.get());

	//TODO: returns null on switch
	SDL_Cursor_ptr cursor = SDL_Cursor_ptr(SDL_CreateColorCursor(sprite, 10, 10));
	if (!cursor)
	{
		//Does not work on switch so suppress logging
#ifndef __SWITCH__
		LOG_F(ERROR, "Unable to create cursor.\n");
#endif
		return NULL;
	}

	return cursor;
}

int Cursor::Init()
{
	//set string if sprite sheet, otherwise specify on each load
	std::string sheetName = "DataFiles/cursors.png";

	//Default click
	cursors[0] = std::move(loadCursorFromSheet(sheetName, SDL_Rect{ 506, 3, 30, 40 }));

	return 0;
}

void Cursor::DrawCursor()
{
	if (!CursorChanged)
		currentCursor = baseCursor;

	if (!cursors[currentCursor])
	{
		//Does not work on switch so suppress logging
#ifndef __SWITCH__
		LOG_F(ERROR, "Invalid cursor id: %d\n", currentCursor);
#endif
		currentCursor = baseCursor;
	}

	SDL_SetCursor(cursors[currentCursor].get());
}

void Cursor::setCursor(int num)
{
	currentCursor = num;
	CursorChanged = true;
}

void Cursor::resetCursor()
{
	currentCursor = baseCursor;
}