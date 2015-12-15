#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;
class vec3;

#define MAX_ACCELERATION 5000.0f	   // 1000.0f
#define TURN_DEGREES 10.0f * DEGTORAD  // 15.0f
#define BRAKE_POWER 250.0f			   // 1000.0f

enum PLAYER_STATE
{
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

	void respawn(const vec3& respawn_point);

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	bool  following_camera;
	PLAYER_STATE state;
};