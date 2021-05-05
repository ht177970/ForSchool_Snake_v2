#include "DataManager.hpp"
#include <fstream>
#include <sstream>

namespace rg {

	const char GameData::data_file[9] = "data.dat";
	const int GameData::res = 191;

	std::string GameData::LongToText(long value) {
		std::stringstream Converter;
		std::string text;
		Converter << value;
		Converter >> text;
		return text;
	}

	long GameData::TextToLong(std::string text) {
		std::stringstream Converter;
		long value;
		Converter << text;
		Converter >> value;
		return value;
	}

	long GameData::Encrypt(int score) {
		long raw_score;
		raw_score = score * 131L + 17;
		return raw_score;
	}

	int GameData::Decrypt(long raw_score) {
		if ((raw_score - 17) % 131)
			return 0;
		int score;
		score = static_cast<int>((raw_score - 17) / 131);
		return score;
	}

	void GameData::Encrypt(std::string& text) {
		for (auto& CHAR : text)
			CHAR += res;
	}

	void GameData::Decrypt(std::string& text) {
		for (auto& CHAR : text)
			CHAR -= res;
	}

	void GameData::saveHighestScore(int score) {
		std::fstream dataf;
		dataf.open(data_file, std::ios_base::out | std::ios_base::trunc);
		long raw_score = Encrypt(score);
		std::string text = LongToText(raw_score);
		Encrypt(text);
		dataf << text;
		dataf.close();
	}

	int GameData::getHighestScore() {
		std::fstream dataf;
		dataf.open(data_file, std::ios_base::in);
		std::string raw;
		dataf >> raw;
		dataf.close();
		Decrypt(raw);
		long raw_score = TextToLong(raw);
		int score = Decrypt(raw_score);
		return score;
	}

}