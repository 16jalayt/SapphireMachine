#include "Engine/Sprite.h"
#include "Engine/AnimatedSprite.h"

class Movie : public Engine::Sprite
{
public:
	Movie(std::string name, int x, int y, bool isLooped = false, RenderParent parent = RenderParent::canvas, Scaled_Rect partial = Scaled_Rect());
	~Movie();
	void Draw();
	AnimatedSprite_ptr ASprite;

private:
};

using Movie_ptr = std::shared_ptr<Movie>;