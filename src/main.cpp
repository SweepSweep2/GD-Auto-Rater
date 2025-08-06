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
		
		// Make sure we haven't rated the level yet.
		if (!m_starRateBtn) return;
		if (!m_starRateBtn->isEnabled()) return;

		// Make sure the user has the feature enabled.
		if (!Mod::get()->getSettingValue<bool>("enable-auto-rater")) return;

		std::string rateMode = Mod::get()->getSettingValue<std::string>("rate-mode-selection");
		int overrideDifficultyRate = Mod::get()->getSettingValue<int>("override-difficulty-rate");
		std::string naOverride = Mod::get()->getSettingValue<std::string>("na-override");
		
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