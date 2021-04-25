#ifndef SNAKE_RG_GLOBAL_H_
#define SNAKE_RG_GLOBAL_H_

namespace rg {
	static enum class CMode {
		MAIN_MENU, GAMING, SETTINGS, GAMEOVER, NONE
	};
	static enum class GMode {
		Gaming, A1, A2, NONE
	};

	class Global {
	public:
		static void C_changeCMode(CMode new_mode);
		static CMode C_getMode();
		static void C_afterRebuild();
		static bool C_Rebuild();
		static void G_changeGMode(GMode new_mode);
		static GMode G_getMode();
	private:
		static CMode C_now_mode;
		static bool C_rebuild;
		static GMode G_now_mode;
	};
}

#endif