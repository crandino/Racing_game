#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "p2Point.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	/*App->camera->Move(vec3(170.0f, 40.0f, 75.0f));
	App->camera->LookAt(vec3(170.0f, 0, 75.0f));*/

	Cube border_block;
	border_block.color.Set(0.67,0.18,0.18);
	border_block.size = { 50, 2, 1 };	

	border_block.SetPos(185, 1, 0);
	border_block.SetRotation(90, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	border_block.SetPos(170, 1, 0);
	border_block.SetRotation(90, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	border_block.SetPos(183, 1, 50);
	border_block.SetRotation(85, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	border_block.SetPos(167, 1, 50);
	border_block.SetRotation(83, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	border_block.SetPos(163, 1, 92);
	border_block.SetRotation(45, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	border_block.SetPos(146, 1, 92);
	border_block.SetRotation(45, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	border_block.size = { 10, 2, 1 };
	border_block.SetPos(141, 1, 114);
	border_block.SetRotation(45, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, 0.0f));

	Cube ramp;
	ramp.color = { 0.5, 0.5, 0.5 };
	ramp.size.Set(6, 0.25, 6);
	ramp.SetPos(136, 1, 111);
	ramp.SetRotation(45, { -0.5f, 0.90f, -0.5f });
	cube_circuit_pieces.prim_bodies.PushBack(ramp);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(ramp, 0.0f));	

	App->camera->Move(vec3(141.0f, 40.0f, 114.0f));
	App->camera->LookAt(vec3(141.0f, 0, 114.0f));

	//createSpiralRamp({5, 0, 0});

	return ret;
}

void angleAndAxisFromEuler(float psi, float theta, float phi, float &angle, vec3 &axis)
{

	// https://www.udacity.com/course/viewer#!/c-cs291/l-91073092/m-123949249

	psi = psi * M_PI / 180.0f;
	theta = theta * M_PI / 180.0f;
	phi = phi * M_PI / 180.0f;

	float m[3][3];

	m[0][0] = cos(theta)*cos(psi);
	m[0][1] = cos(psi)*sin(theta)*sin(phi) - cos(phi)*sin(psi);
	m[0][2] = cos(psi)*cos(psi)*sin(theta) + sin(psi)*sin(phi);
	m[1][0] = cos(theta)*sin(psi);
	m[1][1] = sin(psi)*sin(theta)*sin(phi) + cos(phi)*cos(psi);
	m[1][2] = cos(phi)*sin(psi)*sin(theta) - cos(psi)*sin(phi);
	m[2][0] = -sin(theta);
	m[2][1] = cos(theta)*sin(phi);
	m[2][2] = cos(theta)*cos(phi);

	angle = acos((m[0][0] + m[1][1] + m[2][2] - 1) / 2.0f);

	axis.x = (m[2][1] - m[1][2]) / (2 * sin(angle));
	axis.y = (m[0][2] - m[2][0]) / (2 * sin(angle));
	axis.z = (m[1][0] - m[0][1]) / (2 * sin(angle));

	angle = angle / M_PI * 180.0f;
}

void ModuleSceneIntro::createSpiralRamp(vec3 initial_pos)
{
	vec3 pos(initial_pos);
	vec3 dim(5, 0.25, 5);
	
	float angle_d = 15.0f;
	float angle_r = angle_d * M_PI / 180.0f;

	Cube c;
	c.color = Green;
	c.size = { dim.x, dim.y, dim.z };
	
	c.SetPos(pos.x, pos.y, pos.z);
	c.SetRotation(angle_d, { 0, 0, 1 });		
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, 0.0f));

	pos += {dim.x * cos(angle_r), dim.x * sin(angle_r), 0};
	c.SetPos(pos.x, pos.y, pos.z);
	c.SetRotation(angle_d, { 0.50f, 0, 1 });
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, 0.0f));

	pos += {dim.x * cos(angle_r), dim.x * sin(angle_r), 0};
	c.SetPos(pos.x, pos.y, pos.z);
	c.SetRotation(angle_d, { 1.0f, 0, 1 });
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, 0.0f));
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < cube_circuit_pieces.prim_bodies.Count(); i++)
		cube_circuit_pieces.prim_bodies[i].Render();		

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

