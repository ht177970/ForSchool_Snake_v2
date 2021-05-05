#ifndef SNAKE_RG_DATA_H_
#define NAKE_RG_MENU_H_

#include <string>

namespace rg {
	class GameData {
	public:
		static void saveHighestScore(int score);
		static int getHighestScore();
	private:
		static const char data_file[9];
		static const int res;

		static std::string LongToText(long value);
		static long TextToLong(std::string text);

		static long Encrypt(int score);
		static int Decrypt(long raw_score);

		static void Encrypt(std::string& text);
		static void Decrypt(std::string& raw);
	};
}

#endif