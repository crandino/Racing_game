#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "ModuleAudio.h"
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

	bool countdown_played = false;
	countdown_fx = App->audio->LoadFx("Sounds/Countdown.ogg");
	go_fx = App->audio->LoadFx("Sounds/Go.ogg");
	great_race_fx = App->audio->LoadFx("Sounds/Great_race.ogg");
	oh_no_fx = App->audio->LoadFx("Sounds/Lost_game-oh_no.ogg");
	applause_fx = App->audio->LoadFx("Sounds/win_game-applause.ogg");
	jeering_fx = App->audio->LoadFx("Sounds/Lost_game-jeering.ogg");

	best_time = 240000.0f;

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
	vehicle->SetPos(0.0f ,0.0f, 0.0f);
	state = PREPARATION;
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");	
	delete[] vehicle->info.wheels;
	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	switch (state)
	{
		case PREPARATION:
		{
			state = READY;
			vehicle->Brake(brake);
			crono.Stop();
			App->audio->stopMusic();
			App->scene_intro->changeAllCheckpoints();

			countdown_played = false;
			following_camera = false;
			App->scene_intro->traffic_light1.color = App->scene_intro->traffic_light2.color = Red;
			lap = 0;
			
			vehicle->SetPos(-30.0f, 0, -175.0f);
			vehicle->orient(M_PI / 2);
			vehicle->SetLinearVelocity(0.0f, 0.0f, 0.0f);
			vehicle->ApplyEngineForce(0.0f);
			vehicle->Turn(0.0f);			
			
			turn = acceleration = brake = 0.0f;
			
			vec3 p = vehicle->GetPos();
			App->camera->Look(vec3(-30.0f, 50.0f, -175.0f), p);
			
			break;
		}
		case READY:
		{
			float speed_cam = 0.08f;

			vec3 p = vehicle->GetPos();
			vec3 f = vehicle->GetForwardVector();

			vec3 dist_to_car = { -8.0f, 2.0f, -8.0f };
			vec3 new_camera_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
			vec3 camera_disp_vec = new_camera_position - App->camera->Position;

			if (abs(camera_disp_vec.x) < 1.0f && abs(camera_disp_vec.y) < 1.0f && abs(camera_disp_vec.z) < 1.0f)
			{
				state = STEADY;
				crono.Start();
			}				
			else
				App->camera->Look(App->camera->Position + (speed_cam * camera_disp_vec), p);			

			break;
		}
		case STEADY:
		{
			if (!countdown_played)
			{
				App->audio->PlayFx(countdown_fx);
				countdown_played = true;
			}

			if (crono.Read() > 3000.0f)
			{
				state = GO;
				App->audio->PlayFx(go_fx);
				App->audio->PlayMusic("Music/Nickelback-Animals.ogg");
				App->scene_intro->traffic_light1.color = App->scene_intro->traffic_light2.color = Green;
				following_camera = true;
				crono.Stop();
				crono.Start();
			}				
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
			vehicle->ApplyEngineForce(acceleration);

			if (lap > 2)
			{
				state = FINISH;
				App->audio->stopMusic(0.5f);
				crono.Stop();
				showVeredict();

			}
			break;
		}
		case FINISH:
		{
			turn = acceleration = 0.0f;
			brake = BRAKE_POWER;
			vehicle->Brake(brake);

			App->scene_intro->traffic_light1.color = App->scene_intro->traffic_light2.color = Red;
			break;
		}
	}

	// New game
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		state = PREPARATION;
	}
	
	// Activate/deactivate camera on car.
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		following_camera = !following_camera;
	}

	// Reset position to previous active checkpoint
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		respawn(App->scene_intro->check_points[App->scene_intro->current_checkpoint]);
	}	

	vehicle->Render();
	checkDrift();
	showInfo();

	return UPDATE_CONTINUE;
}

void ModulePlayer::respawn(const PhysBody3D* respawn_point)
{
	vec3 pos = respawn_point->GetPos();
	vehicle->SetPos(pos.x, pos.y, pos.z);
	vehicle->orient(respawn_point->rotation + M_PI/2);
	vehicle->SetLinearVelocity(0, 0, 0);
}

void ModulePlayer::showInfo()
{
	char title[80];

	uint miliseconds_b = best_time % 1000;
	uint seconds_b = (best_time / 1000) % 60;
	uint minutes_b = (best_time / 1000) / 60;

	switch (state)
	{
	case PREPARATION:
	case READY:
	case STEADY:
		sprintf_s(title, "Total time: 00:00:000 | Best time: %02d:%02d:%03d | Speed: 0.0 Km/h | Lap: 0/2 ", minutes_b, seconds_b, miliseconds_b);
		break;
	case GO:
	{
		uint miliseconds = crono.Read() % 1000;
		uint seconds = (crono.Read() / 1000) % 60;
		uint minutes = (crono.Read() / 1000) / 60;
		
		sprintf_s(title, "Total time: %02d:%02d:%03d | Best time: %02d:%02d:%03d | Speed: %.1f Km/h | Lap: %d/2",
						  minutes, seconds, miliseconds, minutes_b, seconds_b, miliseconds_b, vehicle->GetKmh(), lap);
		break;
	}
	case FINISH:
		uint miliseconds = crono.Read() % 1000;
		uint seconds = (crono.Read() / 1000) % 60;
		uint minutes = (crono.Read() / 1000) / 60;
		sprintf_s(title, "Total time: %02d:%02d:%03d | Best time: %02d:%02d:%03d | Speed: 0.0 Km/h | Lap: 2/2",
						 minutes, seconds, miliseconds, minutes_b, seconds_b, miliseconds_b, vehicle->GetKmh());
		break;
	}

	App->window->SetTitle(title);
}

bool ModulePlayer::checkDrift()
{
	vec3 for_dir = vehicle->GetForwardVector();
	vec3 vel_dir = vehicle->GetLinearVelocity();
	
	vec2 for_dir2(for_dir.x, for_dir.z);
	for_dir2 = normalize(for_dir2);
	vec2 vel_dir2(vel_dir.x, vel_dir.z);
	vel_dir2 = normalize(vel_dir2);
	float angle = acos(dot(for_dir2, vel_dir2));

	return true;
}

void ModulePlayer::showVeredict()
{
	if (crono.Read() < best_time)
	{
		App->audio->PlayFx(applause_fx);
		App->audio->PlayFx(great_race_fx);
		best_time = crono.Read();
	}
	else
	{
		App->audio->PlayFx(oh_no_fx);
		App->audio->PlayFx(jeering_fx);
	}
}

