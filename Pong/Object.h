#pragma once
#include <SDL.h>

class Object
{
public:
	void virtual init()=0;
	void virtual Move() = 0;
	void virtual Render(SDL_Renderer* renderer);

	bool virtual IsColliding(const Object& o);

	inline float GetX() const;
	inline float GetY() const;

	inline float GetW() const;
	inline float GetH() const;
public:
	float posX,posY;
	float w,h;
};

inline float Object::GetX() const
{
	return posX;
}

inline float Object::GetY() const
{
	return posY;
}

inline float Object::GetW() const
{
	return w;
}

inline float Object::GetH() const
{
	return h;
}