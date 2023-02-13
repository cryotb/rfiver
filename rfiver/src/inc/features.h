#pragma once

class c_feature_base
{
public:
	virtual void move(respawn::c_player* me, respawn::user_cmd_t* cmd) { }
	virtual void move_unsafe(respawn::c_player* me, respawn::user_cmd_t* cmd) { }
	virtual void paint() { }
private:
};

class c_feature_mgr
{
public:
	void add(c_feature_base* ft) { _list.push_back(ft); }

	void on_move(respawn::c_player* me, respawn::user_cmd_t* cmd)
	{
		for (auto pft : _list)
		{
			if (!pft) continue;
			pft->move(me, cmd);
		}
	}

	void on_move_unsafe(respawn::c_player* me, respawn::user_cmd_t* cmd)
	{
		for (auto pft : _list)
		{
			if (!pft) continue;
			pft->move_unsafe(me, cmd);
		}
	}

	void on_paint()
	{
		for (auto pft : _list)
		{
			if (!pft) continue;
			pft->paint();
		}
	}
private:
	std::vector<c_feature_base*> _list;
};

namespace ft
{
	class c_esp : public c_feature_base
	{
	public:
		void process_player(const cache::object_t& obj, bool pl);
		void paint() override;
		void move(respawn::c_player* me, respawn::user_cmd_t* cmd) override;
	private:
	};

	struct context_t
	{
		math::vec3_t my_shoot_pos;
		math::ang3_t my_angles;
		int bt_tick;
	};

	class c_aim : public c_feature_base
	{
	public:
		enum target_sel_mode
		{
			dist = 0,
			fov
		};

		float get_max_fov();
		float get_max_dist();
		bool can_activate(respawn::user_cmd_t* cmd);
		bool get_target_pos(const cache::object_t& obj, cache::hitbox_t& out);

		void paint() override;
		bool think(respawn::c_player* me, respawn::user_cmd_t* cmd);
		void move(respawn::c_player* me, respawn::user_cmd_t* cmd) override;

		bool is_silent() { return g_cfg.aim_silent; }

		bool m_aiming;
		math::vec3_t m_aim_pos;
	private:
		struct
		{
			bool waiting_for_release;
			bool aiming;
			int aim_ticks;
		} ctx_arc;
	private:
		respawn::weapons m_wpn_def;
		respawn::weapon_types m_wpn_type;
		context_t m_ctx;
	};

	class c_trigger : public c_feature_base
	{
	public:
		void move(respawn::c_player* me, respawn::user_cmd_t* cmd) override;
	public:
		bool m_target_found;
	};

	class c_rapidfire : public c_feature_base
	{
	public:
		void paint() override;
		void move(respawn::c_player* me, respawn::user_cmd_t* cmd) override;

		int _boost_ticks;
		int _last_boost_stop_tick;
		bool _boosting;

		bool can_boost(respawn::user_cmd_t* cmd);

		void start_boosting();
		void stop_boosting();
	private:
	};

	class c_misc : public c_feature_base
	{
	public:
		void paint() override;
		void move(respawn::c_player* me, respawn::user_cmd_t* cmd) override;
		void move_unsafe(respawn::c_player* me, respawn::user_cmd_t* cmd) override;
	private:
	};

	class c_spectators : public c_feature_base
	{
	public:
		void paint() override;
	private:
	};
}

namespace f
{
	extern c_feature_mgr* mgr;

	inline ft::c_esp* esp{ };
	inline ft::c_aim* aim{ };
	inline ft::c_trigger* trigger{ };
	inline ft::c_rapidfire* rapidfire{ };
	inline ft::c_misc* misc{ };
	inline ft::c_spectators* specs{ };

	extern void start();
}
