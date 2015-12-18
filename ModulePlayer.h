#pragma once
#include "Module.h"
#include "Globals.h"
#include "Timer.h"
#include "p2Point.h"

struct PhysVehicle3D;
class vec3;

#define MAX_ACCELERATION 6000.0f	   // Previous 6000.0f
#define TURN_DEGREES 20.0f * DEGTORAD  // Previous 20.0.0f
#define BRAKE_POWER 250.0f			   // Previous 250.0f

enum PLAYER_STATE
{
	PREPARATION,
	READY,
	STEADY,
	GO,
	FINISH
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void respawn(const PhysBody3D* respawn_point);
	void showInfo();
	bool checkDrift();
	void showVeredict();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	bool  following_camera;
	PLAYER_STATE state;

	uint lap;

	Timer crono;
	uint countdown_fx;
	uint go_fx;
	uint applause_fx;
	uint oh_no_fx;
	uint great_race_fx;
	uint jeering_fx;

	uint best_time;
	bool countdown_played;
};