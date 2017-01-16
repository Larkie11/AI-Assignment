#include "Bullet.h"

Bullet::Bullet()
{
	gravity.Set(0, -9.8, 0);
}

Bullet::~Bullet()
{

}
void Bullet::SetPosition(Vector3 pos, Vector3 distance)
{
	vel.SetZero();
	vel = distance;
	this->pos = pos;
}
Vector3 Bullet::GetPosition()
{
	return pos;
}
void Bullet::SetTarget(Vector3 pos)
{
}
void Bullet::Update(double dt)
{
	vel += gravity * dt;
	pos += vel * dt;
}