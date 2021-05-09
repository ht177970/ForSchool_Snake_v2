#include "Global.hpp"

namespace rg {
	CMode Global::C_now_mode = CMode::NONE;
	bool Global::C_rebuild = true;
	GMode Global::G_now_mode= GMode::NONE;
	Settings Global::settings = Settings();

	const sf::Color Global::Gray = sf::Color(192, 192, 192);

	void Global::C_changeCMode(CMode new_mode) {
		Global::C_now_mode = new_mode;
		Global::C_rebuild = true;
	}

	CMode Global::C_getMode() {
		return Global::C_now_mode;
	}

	//return need rebuild
	//if need, vaule will change to false
	bool Global::C_Rebuild() {
		if (Global::C_rebuild) {
			Global::C_rebuild = false;
			return true;
		}
		return Global::C_rebuild;
	}

	void Global::G_changeGMode(GMode new_mode) {
		Global::G_now_mode = new_mode;
	}

	GMode Global::G_getMode() {
		return Global::G_now_mode;
	}

}