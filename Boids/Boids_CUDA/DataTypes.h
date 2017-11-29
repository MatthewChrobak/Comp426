#pragma once

typedef struct
{
	float X;
	float Y;
} Vector2f;

typedef struct {
	Vector2f NewVelocity;
	Vector2f Position;
	Vector2f Velocity;
} Boid;