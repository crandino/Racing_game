#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include <math.h>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	following_camera = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties 
	car.chassis_size.Set(2.0f, 0.5f, 4.0f);
	car.chassis_offset.Set(0.0f, 0.5f, 0.0f);
	car.mass = 1000.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 5.0f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.3f;					// 0.6f
	float wheel_width = 0.5f;					// 0.5f
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.6;  // 0.5f
	float half_length = car.chassis_size.z * 0.6;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(-19.0f, 0, -175.0f);
	vehicle->orient(M_PI/2);

	state = GO;
	crono.Start();
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	// CRZ -> implementation of camera. Let's do it!
	switch (state)
	{
		case READY:
		{
			vec3 p = vehicle->GetPos();
			App->camera->LookAt(p);
			acceleration = 500.0f;
			break;
		}
		case STEADY:
		{
			float speed_cam = 0.09;
			vec3 p = vehicle->GetPos();
			vec3 f = vehicle->GetForwardVector();

			vec3 dist_to_car = { -8.0f, 5.0f, -8.0f };
			vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
			vec3 speed_camera = camera_new_position - App->camera->Position;

			App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			break;
		}
		case GO:
		{
			if (following_camera)
			{
				float speed_cam = 0.09;
				vec3 p = vehicle->GetPos();
				vec3 f = vehicle->GetForwardVector();

				vec3 dist_to_car = { -8.0f, 5.0f, -8.0f };
				vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 speed_camera = camera_new_position - App->camera->Position;

				App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			}			

			turn = acceleration = brake = 0.0f;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				acceleration = MAX_ACCELERATION;
			}

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				if (turn < TURN_DEGREES)
					turn += TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				if (turn > -TURN_DEGREES)
					turn -= TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() > 0.0f)
					brake = BRAKE_POWER;

				if (vehicle->GetKmh() <= 0.0f)
					acceleration = -(MAX_ACCELERATION / 2);
			}
			vehicle->Turn(turn);
			vehicle->Brake(brake);
			break;
		}
		case FINISH:
		{
			turn = acceleration = brake = 0.0f;
			break;
		}
	}

	vehicle->ApplyEngineForce(acceleration);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		respawn(App->scene_intro->check_points[App->scene_intro->current_checkpoint]);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		following_camera = !following_camera;
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		state = READY;
	}

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		state = STEADY;
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		state = GO;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		state = FINISH;
	}

	vehicle->Render();

	char title[80];
	vec3 v = vehicle->GetPos();
	//sprintf_s(title, "%.1f Km/h   X:%.2f Y:%.2f Z:%.2f", vehicle->GetKmh(), v.x, v.y, v.z );
	//App->window->SetTitle(title);

	showCrono();

	return UPDATE_CONTINUE;
}

void ModulePlayer::respawn(const PhysBody3D* respawn_point)
{
	vec3 pos = respawn_point->GetPos();
	vehicle->SetPos(pos.x, pos.y, pos.z);
	vehicle->orient(respawn_point->rotation + M_PI/2);
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
}

void ModulePlayer::showCrono()
{
	uint miliseconds = crono.Read() % 1000;
	uint seconds = (crono.Read() / 1000) % 60;
	uint minutes = (crono.Read() / 1000) / 60;

	char title[80];
	sprintf_s(title, "Total time --  %02d:%d:%03d  --", minutes, seconds, miliseconds);
	App->window->SetTitle(title);

}

