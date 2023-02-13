#pragma once

class c_spec_mgr
{
public:
	void start(int ent_index)
	{
		std::lock_guard<std::mutex> _(mtx_);

		if (locked_ == true)
			return;

		ent_index_ = ent_index;
		locked_ = true;
	}

	void stop()
	{
		std::lock_guard<std::mutex> _(mtx_);

		if (locked_ == false)
			return;

		locked_ = false;
	}

	static auto single()
	{
		static c_spec_mgr inst_;
		return &inst_;
	}

	void on_view_render(respawn::c_view_render* inst);
	void on_paint();

	const auto is_spectating() { std::lock_guard<std::mutex> _(mtx_); return locked_; }
private:
	std::mutex mtx_;
	bool locked_;
	cache::object_t ent_obj_;
	int ent_index_;
};
