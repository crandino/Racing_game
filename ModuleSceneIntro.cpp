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

	App->camera->Move(vec3(0.0f, 40.0f, 0.0f));
	//App->camera->LookAt(vec3(170.0f, 0, 75.0f));
	vec3 cp0(175, 1, 0);
	vec3 cp1(25, 0, 0);

	current_checkpoint = 1;
	checkpoints.PushBack(cp0);
	checkpoints.PushBack(cp1);

	App->camera->Move(vec3(0.0f, 1.0f, 1.0f));
	App->camera->LookAt(vec3(0.0f, 0, 0.0f));

	createLinearSegmentCircuit({ -30, 0, 50 }, { -20, 0, 10 }, 20);
	createCircularSegmentCircuit({ -20, 0, 10 }, { 20, 0,20  }, -0.80f, 20);
	createRamp({ 10, 0, -10 }, { 30, 0, 20 });
	
	return ret;
}

void angleAndAxisFromEuler(float psi, float theta, float phi, float &angle, vec3 &axis)
{

	// https://www.udacity.com/course/viewer#!/c-cs291/l-91073092/m-123949249

	/*psi = psi * M_PI / 180.0f;
	theta = theta * M_PI / 180.0f;
	phi = phi * M_PI / 180.0f;*/

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

	axis = normalize(axis);

	angle = angle / M_PI * 180.0f;
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
	if (body1->is_sensor == true)
	{
		for (uint i = 0; i < sensors.Count(); i++)
		{
			if (body1 == sensors[i])
			{
				current_checkpoint = i;
				body1->is_sensor = false;
				break;
			}
		}
	}
}

void ModuleSceneIntro::createRamp(const vec3 i, const vec3 f)
{
	float height = 10;
	vec3 dir = normalize(f - i);
	
	// Angle theta about y axis
	

	// Angle psi about z axis
	vec3 f2 = { f.x, height, f.z };
	dir = normalize(f2 - i);

	/*float heading = 0.0f;
	if (i.z >= 0.0f && i.x < 0.0f)
		heading = acos(dot(dir, { 1, 0, 0 }));
	else if (i.z >= 0.0f && i.x >= 0.0f)
		heading = acos(dot(dir, { 1, 0, 0 }));
	else if (i.z < 0.0f && i.x >= 0.0f)
		heading = 2 * M_PI - acos(dot(dir, { 1, 0, 0 }));
	else if (i.z < 0.0f && i.x < 0.0f)
		heading = 2 * M_PI - acos(dot(dir, { 1, 0, 0 }));*/

	float heading = 0.0f;
	heading = acos(dot(dir, { 0, 1, 0 }));
	
	float psi = 0.0f;
	psi = acos(dot(dir, { 1, 0, 0 }));

	float phi = 0.0f;
	phi = acos(dot(dir, { 0, 0, 1 }));

	/*if (dir.x > 0.0f)


	if (i.y >= 0.0f && i.x < 0.0f)
		psi = acos(dot(dir, { 1, 0, 0 }));
	else if (i.y >= 0.0f && i.x >= 0.0f)
		psi = acos(dot(dir, { 1, 0, 0 }));
	else if (i.y < 0.0f && i.x >= 0.0f)
		psi = 2 * M_PI - acos(dot(dir, { 1, 0, 0 }));
	else if (i.y < 0.0f && i.x < 0.0f)
		psi = 2 * M_PI - acos(dot(dir, { 1, 0, 0 }));*/

	LOG("Phi = %f", phi * 180.0f / M_PI);
	
	float angle;
	vec3 axis;
	angleAndAxisFromEuler(psi, heading, phi, angle, axis);
	LOG("Angle is %f and vector is (%f, %f, %f)", angle, axis.x, axis.y, axis.z);

	Cube c;
	vec3 pos;
	c.color = Blue;
	c.size = { 1, 2, 1 };

	pos = i;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	pos = f2;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	c.size = (5, 0.25, 5);
	pos = i;
	c.SetPos(pos.x, pos.y, pos.z);
	c.SetRotation(angle, axis);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

}

void ModuleSceneIntro::createLinearSegmentCircuit(const vec3 i, const vec3 f, uint intervals)
{
	
	float distance = sqrt(pow(f.x - i.x, 2) + pow(f.y - i.y, 2) + pow(f.z - i.z, 2));
	float dist_segment = distance / intervals;

	vec3 dir_v = f - i;
	float dir_v_mod = sqrt((dir_v.x*dir_v.x) + (dir_v.y*dir_v.y) + (dir_v.z * dir_v.z));
	dir_v /= dir_v_mod;
	vec3 perp_v = { -dir_v.z, 0, dir_v.x };
	float perp_v_mod = sqrt((perp_v.x*perp_v.x) + (perp_v.y*perp_v.y) + (perp_v.z * perp_v.z));
	perp_v /= perp_v_mod;

	vec3 pos;
	vec3 dim(1, 2, 1);

	Cube c;
	c.color = White;
	c.size = { dim.x, dim.y, dim.z };

	for (uint j = 0; j < intervals; j++)
	{
		c.color = (j % 2 == 0) ? White : Red;

		pos = (i + (dir_v * j * dist_segment)) + ((TRACK_WIDTH / 2) * perp_v);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		cube_circuit_pieces.prim_bodies.PushBack(c);
		cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

		pos = (i + (dir_v * j * dist_segment)) + ((TRACK_WIDTH / 2) * -perp_v);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		cube_circuit_pieces.prim_bodies.PushBack(c);
		cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));
	}
}

void ModuleSceneIntro::createCircularSegmentCircuit(const vec3 i, const vec3 f, float factor, uint intervals)
{
	assert(factor < 1.0f && factor > -1.0f);
	float distance = length(f - i);
	vec3 mid_point = (f - i) / 2.0f + i;

	vec3 dir_v = f - i;
	dir_v = normalize(dir_v);
	vec3 perp_v = { -dir_v.z, 0, dir_v.x };
	perp_v = normalize(perp_v);

	float max_radi = distance / 2.0f;
	float seg_to_high_point = factor * max_radi;

	vec3 h = mid_point + (seg_to_high_point * perp_v);

	float mFH = (h.z - f.z) / (h.x - f.x);
	float mIH = (i.z - h.z) / (i.x - h.x);

	vec3 center_circle = { 0, 0, 0 };
	center_circle.x = (mFH*mIH*(i.z - f.z) + mFH*(h.x + i.x) - mIH*(f.x + h.x)) / (2.0f * (mFH - mIH));
	center_circle.z = (-1 / mFH)*(center_circle.x - ((f.x + h.x) / 2.0f)) + ((f.z + h.z) / 2.0f);

	vec3 c_to_i = normalize(i - center_circle);
	vec3 c_to_f = normalize(f - center_circle);
	float theta = acos(dot(c_to_f, c_to_i));
	float radius = length(f - center_circle);

	float angle_ref = 0.0f;
	if (i.z >= center_circle.z && i.x < center_circle.x)
		angle_ref = acos(dot(c_to_i, { 1, 0, 0 }));
	else if (i.z >= center_circle.z && i.x >= center_circle.x)
		angle_ref = acos(dot(c_to_i, { 1, 0, 0 }));	
	else if (i.z < center_circle.z && i.x >= center_circle.x)
		angle_ref = 2 * M_PI - acos(dot(c_to_i, { 1, 0, 0 }));	
	else if (i.z < center_circle.z && i.x < center_circle.x)
		angle_ref = 2 * M_PI - acos(dot(c_to_i, { 1, 0, 0 }));
		
	Cube c;
	vec3 dim(1, 2, 1);
	vec3 pos;
	c.size = { dim.x, dim.y, dim.z };
	c.color = Orange;

	vec3 central_pos;
	for (uint j = 0; j < intervals; j++)
	{
		c.color = (j % 2 == 0) ? White : Red;
		float sub_angle = (factor > 0.0f) ? -(float)j / intervals * theta : (float)j / intervals * theta;

		central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
		central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);	

		vec3 to_center = normalize(central_pos - center_circle);
		pos = central_pos + ((TRACK_WIDTH / 2.0f) * to_center);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		cube_circuit_pieces.prim_bodies.PushBack(c);
		cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c,this, 0.0f));

		pos = central_pos + ((TRACK_WIDTH / 2.0f) * -to_center);
		c.SetPos(pos.x, pos.y + 1, pos.z);
		cube_circuit_pieces.prim_bodies.PushBack(c);
		cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c,this, 0.0f));

	}

}


