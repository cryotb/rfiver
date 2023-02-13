#include <include.h>

void c_spec_mgr::on_view_render(respawn::c_view_render* inst)
{
	if (!locked_)
		return;

	ent_obj_ = g_ent_cache.objects()[ent_index_];

	const auto& target = ent_obj_;
	const auto& data = target.ds->dt.player;

	if (!target.hdr.m_used || !data.alive)
	{
		stop();
		return;
	}

	auto angles = data.eye_angles;

	inst->eye_angles = angles;
	inst->camera_pos = target.ds->m_hitboxes.front().pos;

	if (data.zooming)
		inst->fov /= 2.f;
}

void c_spec_mgr::on_paint()
{
	/*float pad_x = 400.f, pad_y = 400.f;
	float push_y = 25.f;

	render::text({ pad_x, 5.f + pad_y + push_y }, im::color::green, "spectating: %i", locked_); push_y += 20.f;

	if (locked_)
	{
		render::text({ pad_x, 5.f + pad_y + push_y }, im::color::green, "angles: (%f, %f, %f)", 
			ent_obj_.dt.player.eye_angles.x, 
			ent_obj_.dt.player.eye_angles.y,
			ent_obj_.dt.player.eye_angles.z); 
		push_y += 20.f;
	}*/
}
