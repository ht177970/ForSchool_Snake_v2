#include "Global.hpp"

namespace rg {
	CMode Global::C_now_mode = CMode::NONE;
	bool Global::C_rebuild = true;
	GMode Global::G_now_mode= GMode::NONE;

	void Global::C_changeCMode(CMode new_mode) {
		Global::C_now_mode = new_mode;
		Global::C_rebuild = true;
	}

	CMode Global::C_getMode() {
		return Global::C_now_mode;
	}

	void Global::C_afterRebuild() {
		Global::C_rebuild = false;
	}

	bool Global::C_Rebuild() {
		return Global::C_rebuild;
	}

	void Global::G_changeGMode(GMode new_mode) {
		Global::G_now_mode = new_mode;
	}

	GMode Global::G_getMode() {
		return Global::G_now_mode;
	}

}