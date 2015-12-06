#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// CRZ Montmeló circuit ... or something similiar... or a little aproximation... or a circle... or a straight line.

	Cube c1;
	c1.SetPos(1.0, 0.0, 20.0);
	c1.size = { 5, 2, 20 };
	c1.SetRotation(20, { 1, 0, 0 });
	c1.color.Set(1, 0, 0);
	circuit_pieces_primitives.add(c1);
	circuit_pieces_ph3d.add(App->physics->AddBody(c1, 0.0f));

	return ret;
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

	Cube s(2, 2, 2);
	s.SetPos(4, 3, 6);
	s.SetRotation(35, { 1, 0, 0 });
	s.axis = true;
	s.color = Green;
	s.Render();

	p2List_item<PhysBody3D*> *item_ph3d = circuit_pieces_ph3d.getFirst();
	p2List_item<Primitive> *item_primitive = circuit_pieces_primitives.getFirst();
	while (item_primitive != NULL)
	{
		item_ph3d->data->GetTransform(&(item_primitive->data.transform));
		item_primitive->data.Render();

		item_primitive = item_primitive->next;
		item_ph3d = item_ph3d->next;
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

