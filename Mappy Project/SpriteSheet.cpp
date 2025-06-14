#include "SpriteSheet.h"

Sprite::Sprite()
{
	image=NULL;
	isJumping = false;

}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{

	isJumping = false;

	x = 80;
	y = 64;


	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if(dir == 1){ //right key
		animationDirection = 1; 
		x+=2; 
		if (!isJumping && ++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 1;
		}
	} else if (dir == 0){ //left key
		animationDirection = 0; 
		x-=2; 
		if (!isJumping && ++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame)
				curFrame = 1;
		}
	}else //represent that they hit the space bar and that mean direction = 0
		animationDirection = dir;

	//check for collided with foreground tiles
	if (animationDirection==0)
	{ 
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx; 
			y= oldy;
		}

	}
	else if (animationDirection ==1)
	{ 
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx; 
			y= oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy;


	if (isJumping) {
		fy = frameHeight; // second row
	}
	else {
		fy = 0; // first row
	}


	if (animationDirection==1){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, 0);
	}else if (animationDirection == 0 ){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}else if (animationDirection == 2 ){
		al_draw_bitmap_region(image,0,0,frameWidth,frameHeight,  x-xoffset, y-yoffset, 0);

	}
}

int Sprite::jumping(int jump, const int JUMPIT)
{
	if (jump == JUMPIT) {
		if (!collided(x + frameWidth / 2, y + frameHeight + 5))
			jump = 0;
	}
	else {
		if (!isJumping) {
			isJumping = true;
		}


		if (jump > 0 && collided(x + frameWidth / 2, y)) {
			jump = -1; // Start descending immediately
		}

		y -= jump / 3;
		jump--;

		// Frame assignment based on phase
		if (jump == JUMPIT - 1) {
			curFrame = 0; // crouch
		}
		else if (jump > 0) {
			curFrame = 1; // ascending
		}
		else if (jump < 0) {
			curFrame = 2; // descending
		}
	}

	if (jump < 0) {
		if (collided(x + frameWidth / 2, y + frameHeight)) {
			jump = JUMPIT;

			while (collided(x + frameWidth / 2, y + frameHeight)) {
				y -= 3;
			}

			curFrame = 3; // landing
		}
	}

	if (jump == JUMPIT)
		isJumping = false;

	return jump;
}
