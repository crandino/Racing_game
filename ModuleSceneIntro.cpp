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
	vec3 cp0(175, 1, 0);
	vec3 cp1(0, 0, 0);

	current_checkpoint = 1;
	checkpoints.PushBack(cp0);
	checkpoints.PushBack(cp1);

	Cube border_block;
	border_block.color.Set(0.67,0.18,0.18);
	border_block.size = { 50, 2, 1 };	

	border_block.SetPos(185, 1, 0);
	border_block.SetRotation(90, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	border_block.SetPos(170, 1, 0);
	border_block.SetRotation(90, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	border_block.SetPos(183, 1, 50);
	border_block.SetRotation(85, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	border_block.SetPos(167, 1, 50);
	border_block.SetRotation(83, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	border_block.SetPos(163, 1, 92);
	border_block.SetRotation(45, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	border_block.SetPos(146, 1, 92);
	border_block.SetRotation(45, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	border_block.size = { 10, 2, 1 };
	border_block.SetPos(141, 1, 114);
	border_block.SetRotation(45, { 0, 1, 0 });
	cube_circuit_pieces.prim_bodies.PushBack(border_block);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(border_block, this, 0.0f));

	Cube ramp;
	ramp.color = { 0.5, 0.5, 0.5 };
	ramp.size.Set(6, 0.25, 6);
	ramp.SetPos(136, 1, 111);
	ramp.SetRotation(45, { -0.5f, 0.90f, -0.5f });
	cube_circuit_pieces.prim_bodies.PushBack(ramp);
	// Should be a local variable (physbody)
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(ramp, this, 0.0f, true));

	App->camera->Move(vec3(0.0f, 1.0f, 1.0f));
	App->camera->LookAt(vec3(0.0f, 0, 0.0f));

	//createLinearSegmentCircuit({ 12, 0, -24 }, { 13, 0, 2 }, 10);
	createCircularSegmentCircuit({ 23, 0, -20}, { -1, 0, 30 }, 0.77f);

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
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	pos += {dim.x * cos(angle_r), dim.x * sin(angle_r), 0};
	c.SetPos(pos.x, pos.y, pos.z);
	c.SetRotation(angle_d, { 0.50f, 0, 1 });
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	pos += {dim.x * cos(angle_r), dim.x * sin(angle_r), 0};
	c.SetPos(pos.x, pos.y, pos.z);
	c.SetRotation(angle_d, { 1.0f, 0, 1 });
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));
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

void ModuleSceneIntro::createCircularSegmentCircuit(const vec3 i, const vec3 f, float factor)
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
	float radius = length(f - center_circle);
	
	float theta = acos(dot(c_to_f, c_to_i));
	float angle_ref = acos(dot(c_to_i, { 1, 0, 0 }));
	angle_ref = (i.z >= 0.0f) ? angle_ref : -angle_ref;

	LOG("%f", theta * 180.0f / M_PI);
	LOG("%f", angle_ref * 180.0f / M_PI);

	Cube c;
	vec3 dim(1, 2, 1);
	vec3 pos;
	c.size = { dim.x, dim.y, dim.z };
	c.color = Orange;
	
	vec3 central_pos;
	uint interval = 10;
	for (uint j = 0; j < interval; j++)
	{
		
		float sub_angle =  ((float)j / 10) * theta;
		central_pos.x = center_circle.x + radius * cos(sub_angle + angle_ref);
		central_pos.z = center_circle.z + radius * sin(sub_angle + angle_ref);

		vec3 to_center = normalize(central_pos - center_circle);
		pos = central_pos + (TRACK_WIDTH / 2.0f) * to_center;
		c.SetPos(pos.x, pos.y + 1, pos.z);
		cube_circuit_pieces.prim_bodies.PushBack(c);
		cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

		pos = central_pos + (TRACK_WIDTH / 2.0f) * -to_center;
		c.SetPos(pos.x, pos.y + 1, pos.z);
		cube_circuit_pieces.prim_bodies.PushBack(c);
		cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

		c.color.g += 0.03;
		c.color.b += 0.25;
	}

	c.color = Black;
	pos = i;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	c.color = White;
	pos = f;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	c.color = Blue;
	pos = mid_point;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	c.color = Red;
	pos = h;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	c.color = Green;
	pos = center_circle;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

}


