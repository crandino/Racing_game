#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct cubePieces
{
	p2DynArray<PhysBody3D*>		phys_bodies;
	p2DynArray<Cube>			prim_bodies;
};

#define TRACK_WIDTH 20.0f

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	p2DynArray<PhysBody3D*> check_points;
	p2DynArray<Cube> prim_check_points;
	uint current_checkpoint;

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void createLinearSegmentCircuit(const vec3 initial_pos, const vec3 final_pos, uint intervals);
	void createCircularSegmentCircuit(const vec3 i, const vec3 f, float factor, uint left_intervals, uint right_intervals = 6);
	void createRamp(const vec3 i, const vec3 f);
	void createCheckPoint(const vec3 pos, float direction);
	void createStart(const vec3 pos);
	void changeAllCheckpoints();


public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/
	
	cubePieces cube_circuit_pieces;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Sphere traffic_light1;
	Sphere traffic_light2;
};
