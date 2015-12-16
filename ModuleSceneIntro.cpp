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

	App->camera->Move(vec3(0.0f, 400.0f, 0.0f));
	App->camera->LookAt(vec3(-130.0f, 0, 55.0f));
	vec3 cp2(58.0, 0, -71.0f);
	vec3 cp3(85.0, 0, 86.0f);

	traffic_light1.radius = traffic_light2.radius = 0.4f;
	traffic_light1.color = traffic_light2.color = Red;

	current_checkpoint = 0;

	// Circuit creation
	createLinearSegmentCircuit({ -100, 0, -175.0f }, { 30, 0, -175.0f }, 30);
	createCheckPoint({-19.0f, 0.0f, -175.0f}, 0.0f);
	createStart({ -19.0f, 0.0f, -175.0f });
	createCircularSegmentCircuit({ 30, 0, -175.0f }, { 70, 0, -160.0f }, -0.1, 16);
	createLinearSegmentCircuit({ 70, 0, -160.0f }, { 75, 0, -156.6f }, 2);
	createRamp({ 75, 0, -156.6f }, { 90, 4.0f, -146.0f });
	createRamp({ 120, 0.0f, -126.66f }, {105, 4.0f, -136.66f });
	createLinearSegmentCircuit({ 120, 0.0f, -126.66f }, { 150, 0, -106.6f }, 8);
	createCircularSegmentCircuit({ 150, 0, -106.6f }, { 185.0f, 0, -130.0f }, 0.6f, 12);
	createCheckPoint({ 171.0f, 0.0f, -107.0f }, 30.0f);
	createLinearSegmentCircuit({ 185.0f, 0, -130.0f }, { 186.0f, 0.0f, -163.0f }, 8);
	createCircularSegmentCircuit({ 185.0f, 0, -160.0f }, { 145.0f, 0, -175.0f }, 0.80f, 14);
	createLinearSegmentCircuit({ 145.0f, 0, -175.0f }, { 70.0f, 0.0f, -110.0f }, 16);
	createCircularSegmentCircuit({ 70.0f, 0.0f, -110.0f }, { 60.0f, 0, -80.0f }, 0.2f, 10);
	createLinearSegmentCircuit({ 60.0f, 0, -80.0f }, { 60.0f, 0, -20.0f }, 18);
	createCheckPoint({ 60.0f, 0, -80.0f }, -90.0f);
	createCircularSegmentCircuit({ 60.0f, 0, -20.0f }, { 90.0f, 0, -20.0f }, 0.99f, 16);
	createCircularSegmentCircuit({ 90.0f, 0, -20.0f }, { 120.0f, 0, -20.0f }, -0.99f, 16);
	createCircularSegmentCircuit({ 120.0f, 0, -20.0f }, { 10.0f, 0, 110.0f }, -0.45f, 24);
	createCheckPoint({ 108.0f, 0, 49.0f }, 230.0f);
	createLinearSegmentCircuit({ 10.0f, 0, 110.0f }, { -10.0f, 0, 110.0f }, 4);
	createRamp({ -10.0f, 0, 110.0f }, { -40.0f, 4.0f, 110.0f });
	createRamp({ -105.0f, 0, 110.0f }, { -75.0f, 4.0f, 110.0f });
	createLinearSegmentCircuit({ -105.0f, 0, 110.0f }, { -145.0f, 0, 110.0f }, 12);
	createCircularSegmentCircuit({ -145.0f, 0, 110.0f }, { -145.0f, 0, 80.0f }, -0.95f, 20);
	createCheckPoint({ -159.0f, 0, 96.0f }, 90.0f);
	createCircularSegmentCircuit({ -145.0f, 0, 80.0f }, { -125.0f, 0, 85.0f }, -0.15f, 6);
	createCircularSegmentCircuit({ -125.0f, 0, 85.0f }, { -105.0f, 0, 85.0f }, 0.20f, 12);
	createCircularSegmentCircuit({ -105.0f, 0, 85.0f }, { -40.0f, 0, 30.0f }, 0.20f, 24);
	createRamp({ -42.0f, 0, 35.0f }, { -32.0f, 4.0f, 15.0f });
	createRamp({ -15.0f, 0, -19.0f }, { -25.0f, 4.0f, -1.0f });
	createLinearSegmentCircuit({ -15.0f, 0, -19.0f }, { 0.0f, 0, -49.0f }, 8);
	createCircularSegmentCircuit({ 0.0f, 0, -49.0f }, { 20.0f, 0, -60.0f }, -0.4f, 12);
	createCheckPoint({ 33.0f, 0, -74.0f }, 90.0f);
	createCircularSegmentCircuit({ 20.0f, 0, -60.0f }, { 20.0f, 0, -90.0f }, 0.95f, 12);
	createCircularSegmentCircuit({ 20.0f, 0, -90.0f }, { -60.0f, 0, -75.0f }, 0.15f, 12);
	createRamp({ -60.0f, 0, -75.0f }, { -70.0f, 3.0f, -70.0f });
	createRamp({ -95.0f, 0, -57.5f }, { -85.0f, 3.0f, -62.5f });
	createCircularSegmentCircuit({ -95.0f, 0, -57.5f }, { -185.0f, 0, 110.0f }, 0.25f, 24);
	createCircularSegmentCircuit({ -185.0f, 0, 110.0f }, { -80.0f, 0, 185.0f }, 0.40f, 24);
	createLinearSegmentCircuit({ -80.0f, 0, 185.0f }, { 160.0f, 0, 185.0f }, 24);
	createCheckPoint({ -80.0f, 0, 185.0f }, 0.0f);
	createCircularSegmentCircuit({ 160.0f, 0, 185.0f }, { 185.0f, 0, 160.0f}, 0.45f, 12);
	createLinearSegmentCircuit({ 185.0f, 0, 160.0f }, { 175.0f, 0, 120.0f }, 8);
	createCircularSegmentCircuit({ 175.0f, 0, 120.0f }, { 175.0f, 0, 100.0f }, -0.1f, 6);
	createLinearSegmentCircuit({ 175.0f, 0, 100.0f }, { 185.0f, 0, 80.0f }, 6);
	createCircularSegmentCircuit({ 185.0f, 0, 80.0f }, { 150.0f, 0, 70.0f }, 0.99f, 12);
	createCircularSegmentCircuit({ 150.0f, 0, 70.0f }, { 25.0f, 0, 140.0f }, -0.35f, 16);
	createLinearSegmentCircuit({ 25.0f, 0, 140.0f }, { -35.0f, 0, 140.0f }, 8);
	createCircularSegmentCircuit({ -35.0f, 0, 140.0f }, { -60.0f, 0, 120.0f }, -0.45f, 12);
	createCircularSegmentCircuit({ -60.0f, 0, 120.0f }, { -30.0f, 0, 75.0f }, -0.3f, 12);
	createCircularSegmentCircuit({ -30.0f, 0, 75.0f }, { 10.0f, 0, 75.0f }, -0.2f, 12);
	createCircularSegmentCircuit({ 10.0f, 0, 75.0f }, { 15.0f, 0, 45.0f }, 0.99f, 12);
	createCheckPoint({ 25.0f, 0, 69.0f }, 50.0f);
	createLinearSegmentCircuit({ 15.0f, 0, 45.0f }, { 5.0f, 0, 45.0f }, 2);
	createCircularSegmentCircuit({ 5.0f, 0, 45.0f }, { -10.0f, 0, 25.0f }, -0.40f, 10);
	createCircularSegmentCircuit({ -10.0f, 0, 25.0f }, { -20.0f, 0, 10.0f }, 0.2f, 10);
	createLinearSegmentCircuit({ -20.0f, 0, 10.0f }, { -60.0f, 0, -10.0f }, 10);
	createCircularSegmentCircuit({ -60.0f, 0, -10.0f }, { -75.0f, 0, -40.0f }, -0.51f, 10);
	createCircularSegmentCircuit({ -75.0f, 0, -40.0f }, { -80.0f, 0, -70.0f }, 0.25f, 10); 	
	createLinearSegmentCircuit({ -80.0f, 0, -70.0f }, { -100.0f, 0, -90.0f }, 8);
	createCircularSegmentCircuit({ -100.0f, 0, -90.0f }, { -135, 0.0f, -90.0f }, 0.49f, 10);
	createCheckPoint({ -118.0f, 0, -97.0f }, 180.0f);
	createLinearSegmentCircuit({ -135, 0.0f, -90.0f }, { -150, 0, -75.0f }, 8);
	createCircularSegmentCircuit({ -150, 0, -75.0f }, { -184, 0.0f, -101.0f }, -0.68f, 12);
	createLinearSegmentCircuit({ -185, 0.0f, -100.0f }, { -185, 0, -155.0f }, 12);
	createCircularSegmentCircuit({ -185, 0, -155.0f }, { -145, 0, -155.0f }, -0.99f, 12);
	createCircularSegmentCircuit({ -145, 0, -155.0f }, { -120, 0, -155.0f }, 0.99f, 12);
	createCircularSegmentCircuit({ -120, 0, -155.0f }, { -100, 0, -175.0f }, -0.42f, 12);

	return ret;

}

void angleAndAxisFromEuler(float psi, float theta, float phi, float &angle, vec3 &axis)
{

	// https://www.udacity.com/course/viewer#!/c-cs291/l-91073092/m-123949249

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
}

void angleAndAxisFromRotMat(mat4x4 m, float &angle, vec3 &axis)
{
	angle = acos((m[0] + m[5] + m[10] - 1) / 2.0f);

	axis.x = (m[9] - m[6]) / (2 * sin(angle));
	axis.y = (m[2] - m[8]) / (2 * sin(angle));
	axis.z = (m[4] - m[1]) / (2 * sin(angle));

	axis = normalize(axis);
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

	for (int i = 0; i < prim_check_points.Count(); i++)
		prim_check_points[i].Render();

	traffic_light1.Render();
	traffic_light2.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->is_sensor == true)
	{
		for (uint i = 0; i < check_points.Count(); i++)
		{
			if (body1 == check_points[i])
			{
				if (i > current_checkpoint + 1)
				{
					App->player->respawn(check_points[current_checkpoint]);
					break;
				}
				else
				{
					current_checkpoint = i;
					prim_check_points[i].color = Green;
					body1->is_sensor = false;
					if (body1 == check_points[check_points.Count() - 1])
						changeAllCheckpoints();
					break;
				}
			}
		}
	}
}

void ModuleSceneIntro::createRamp(const vec3 i_pos, const vec3 f_pos)
{
	vec3 final_projection = { f_pos.x, 0, f_pos.z };
	vec3 direction = normalize(f_pos - i_pos);						
	vec3 planar_direction = normalize(final_projection - i_pos);
	float distance = length(f_pos - i_pos);
	float planar_distance = length(final_projection - i_pos);

	float heading = 0.0f;
	if (f_pos.z >= i_pos.z && f_pos.x < i_pos.x)
		heading = acos(dot(planar_direction, { 1, 0, 0 }));
	else if (f_pos.z >= i_pos.z && f_pos.x >= i_pos.x)
		heading = acos(dot(planar_direction, { 1, 0, 0 }));
	else if (f_pos.z < i_pos.z && f_pos.x >= i_pos.x)
		heading = 2 * M_PI - acos(dot(planar_direction, { 1, 0, 0 }));
	else if (f_pos.z < i_pos.z && f_pos.x < i_pos.x)
		heading = 2 * M_PI - acos(dot(planar_direction, { 1, 0, 0 }));

	float slope = asin(f_pos.y / distance);

	Cube c;
	/*vec3 pos;
	c.color = Blue;
	c.size = { 1, 2, 1 };

	pos = i_pos;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));

	pos = f_pos;
	c.SetPos(pos.x, pos.y + 1, pos.z);
	cube_circuit_pieces.prim_bodies.PushBack(c);
	cube_circuit_pieces.phys_bodies.PushBack(App->physics->AddBody(c, this, 0.0f));
*/
	c.size = { distance, 0.25f, TRACK_WIDTH - 2 };
	c.color = White;

	c.SetPos(i_pos.x + (direction.x * distance / 2.0f), i_pos.y + (sin(slope)* (c.size.x / 2.0f)), i_pos.z + (direction.z * distance / 2.0f));
	
	c.SetRotation(slope * 180.0f / M_PI, { 0, 0, 1 });
	mat4x4 R1;
	for(uint i = 0; i < 16; i++)
		R1[i] = c.transform.M[i];
	c.SetRotation(-heading * 180.0f / M_PI, { 0, 1, 0});
	mat4x4 R2;
	for (uint i = 0; i < 16; i++)
		R2[i] = c.transform.M[i];
	mat4x4 R3 = R2*R1;

	float angle;
	vec3 axis;
	angleAndAxisFromRotMat(R3, angle, axis);
	c.SetRotation(-angle * 180.0f / M_PI, { axis.x, axis.y, axis.z });

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

void ModuleSceneIntro::createCheckPoint(const vec3 pos, float direction)
{
	Cube sensor;
	vec3 dim(2.0f, 1.0f, TRACK_WIDTH);
	sensor.size = { dim.x, dim.y, dim.z };
	sensor.SetPos(pos.x, pos.y + 1, pos.z);
	sensor.SetRotation(direction, { 0, 1, 0 });
	sensor.color = White;
	PhysBody3D* pb_sensor = App->physics->AddBody(sensor, this, 0.0f, true);
	pb_sensor->rotation = direction * M_PI / 180;
	check_points.PushBack(pb_sensor);
	prim_check_points.PushBack(sensor);
}


void ModuleSceneIntro::createStart(const vec3 pos)
{
	Cube cube;
	cube.color = Blue;
	cube.size = { 1.0f, 1.0f, 1.0f };

	// Columns
	for (uint height = 0; height < 6; height++)
	{
		cube.color = (height % 2 == 0) ? Black : White;
		cube.SetPos(pos.x, height + 1.0f, pos.z + ((TRACK_WIDTH + 1.0f) / 2.0f));
		cube_circuit_pieces.prim_bodies.PushBack(cube);

		cube.SetPos(pos.x, height + 1.0f, pos.z - ((TRACK_WIDTH + 1.0f) / 2.0f));
		cube_circuit_pieces.prim_bodies.PushBack(cube);
	}

	// Up bar
	for (uint disp = 0; disp < 11; disp++)
	{
		cube.color = (disp % 2 == 0) ? White : Black;
		cube.SetPos(pos.x, 6.0f, pos.z + ((TRACK_WIDTH + 1.0f) / 2.0f) - disp);
		cube_circuit_pieces.prim_bodies.PushBack(cube);

		cube.SetPos(pos.x, 6.0f, pos.z - ((TRACK_WIDTH + 1.0f) / 2.0f) + disp);
		cube_circuit_pieces.prim_bodies.PushBack(cube);
	}

	// Traffic light	
	traffic_light1.SetPos(pos.x - 0.4f, 6.0f, pos.z + ((TRACK_WIDTH + 1.0f) / 2.0f) - 10);
	traffic_light2.SetPos(pos.x - 0.4f, 6.0f, pos.z - ((TRACK_WIDTH + 1.0f) / 2.0f) + 10);
}

void ModuleSceneIntro::changeAllCheckpoints()
{
	uint count = 0;
	for (uint i = 0; i < check_points.Count() - 1; i++)
	{
		if (check_points[i]->is_sensor == false)
			count++;
	}

	if (count == check_points.Count() - 1)
	{
		for (uint i = 0; i < check_points.Count() - 1; i++)
		{
				check_points[i]->is_sensor = true;
				prim_check_points[i].color = White;
		}
	}
}

