#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>

$on_mod(Loaded) {
	// You found the hidden Glungus :O
	log::info("Glungus...");
}

class $modify(AutoLevelRate, LevelInfoLayer) {
	void levelDownloadFinished(GJGameLevel* p0) {
		LevelInfoLayer::levelDownloadFinished(p0); // Run the original levelDownloadFinished code before running our custom code.

		// Make sure the user has the feature enabled.
		if (!Mod::get()->getSettingValue<bool>("enable-auto-rater")) return;

		std::string rateMode = Mod::get()->getSettingValue<std::string>("rate-mode-selection");
		int overrideDifficultyRate = Mod::get()->getSettingValue<int>("override-difficulty-rate");
		std::string naOverride = Mod::get()->getSettingValue<std::string>("na-override");
		bool overrideDemonRate = Mod::get()->getSettingValue<bool>("override-demon-rate");
		std::string overrideDifficultyDemonRate = Mod::get()->getSettingValue<std::string>("override-difficulty-demon-rate");

		// Rate demon difficulty if it is available.
		if (m_demonRateBtn && m_demonRateBtn->isEnabled()) {
			// no comments here because screw you :troll:
			std::string demonRated = "";
			RateDemonLayer* rateDemonLayer = RateDemonLayer::create(m_level->m_levelID);

			if (overrideDemonRate) {
				demonRated = overrideDifficultyDemonRate;
				if (overrideDifficultyDemonRate == "Easy Demon") {
					rateDemonLayer->m_demonRate = 1;
				} else if (overrideDifficultyDemonRate == "Medium Demon") {
					rateDemonLayer->m_demonRate = 2;
				} else if (overrideDifficultyDemonRate == "Hard Demon") {
					rateDemonLayer->m_demonRate = 3;
				} else if (overrideDifficultyDemonRate == "Insane Demon") {
					rateDemonLayer->m_demonRate = 4;
				} else {
					rateDemonLayer->m_demonRate = 5;
				}
			} else {
				switch (m_level->m_demonDifficulty) {
					case 3:
						rateDemonLayer->m_demonRate = 1;
						demonRated = "Easy Demon";
						break;
					case 4:
						rateDemonLayer->m_demonRate = 2;
						demonRated = "Medium Demon";
						break;
					case 0:
						rateDemonLayer->m_demonRate = 3;
						demonRated = "Hard Demon";
						break;
					case 5:
						rateDemonLayer->m_demonRate = 4;
						demonRated = "Insane Demon";
						break;
					case 6:
						rateDemonLayer->m_demonRate = 5;
						demonRated = "Extreme Demon";
						break;
				}
			}

			rateDemonLayer->onRate(rateDemonLayer->m_submitButton);

			log::info("Successfully rated the level {}!", demonRated);

			return;
		}
		
		// Make sure we haven't rated the level yet.
		if (!m_starRateBtn) return;
		if (!m_starRateBtn->isEnabled()) return;
		
		RateStarsLayer* rateStarsLayer = RateStarsLayer::create(m_level->m_levelID, p0->isPlatformer(), false);

		int starsRated = 0; // Used to log the amount of stars that the level got rated
		
		// Only rate the requested difficulty if the user chose it.
		if (rateMode == "Requested Difficulty"){
			if (p0->m_starsRequested == 0) { // If they never requested any stars (N/A)
				if (naOverride == "Requested Difficulty" || naOverride == "Override") {
					starsRated = overrideDifficultyRate;
				} else if (naOverride == "Current Difficulty") {
					starsRated = p0->getAverageDifficulty();

					switch (starsRated) {
						case 0:
							starsRated = overrideDifficultyRate;
							break;
						case 1:
							starsRated = 2;
							break;
						case 2:
							starsRated = 3;
							break;
						case 3:
							starsRated = 4;
							break;
						case 4:
							starsRated = 6;
							break;
						case 5:
							starsRated = 8;
							break;
						case 6:
							starsRated = 10;
							break;
						case 7:
							starsRated = 1;
							break;
					}
				}
			} else {
				starsRated = p0->m_starsRequested;
			}
		} else if(rateMode == "Current Difficulty") { // Only rate the current difficulty if the user chose it.
			starsRated = p0->getAverageDifficulty();

			if (starsRated == 0) {
				if (naOverride == "Current Difficulty" || naOverride == "Override") {
					starsRated = overrideDifficultyRate;
				} else {
					if (p0->m_starsRequested == 0) {
						starsRated = overrideDifficultyRate;
					} else {
						starsRated = p0->m_starsRequested;
					}
				}
			} else {
				switch (starsRated) {
					case 1:
						starsRated = 2;
						break;
					case 2:
						starsRated = 3;
						break;
					case 3:
						starsRated = 4;
						break;
					case 4:
						starsRated = 6;
						break;
					case 5:
						starsRated = 8;
						break;
					case 6:
						starsRated = 10;
						break;
					case 7:
						starsRated = 1;
						break;
				}
			}
		} else { // If the user chose override.
			starsRated = overrideDifficultyRate;
		}

		rateStarsLayer->m_starsRate = starsRated;
		rateStarsLayer->onRate(rateStarsLayer->m_submitButton);

		log::info("Successfully rated the level {}*!", starsRated);
	}
};