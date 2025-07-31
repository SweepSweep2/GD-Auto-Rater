#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(AutoLevelRate, LevelInfoLayer) {
	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;

		// Make sure we haven't rated the level yet.
		if (!m_starRateBtn) return false;
		if (!m_starRateBtn->isEnabled()) return false;

		// Make sure the user has the feature enabled.
		if (!Mod::get()->getSettingValue<bool>("enable-auto-rater")) return false;
		
		// Open the rate menu.
		m_starRateBtn->activate();

		RateStarsLayer* rateLayer = nullptr;

		auto& children = *getParent()->getChildren();

		// Loop through the root of the game tree until we find the rate menu.
		for (int i = 0; i < children.count(); i++) {
			auto node = static_cast<CCNode*>(children.objectAtIndex(i));

			if (auto rate = dynamic_cast<RateStarsLayer*>(node)) {
				rateLayer = rate;
				break;
			}
		}

		if (!rateLayer) {
			log::warn("RateStarsLayer could not be found!");
			return false;
		}
	
		// We can just use the first object because it is the only object.
		auto mainLayer = static_cast<CCNode*>(rateLayer->getChildren()->objectAtIndex(0));

		if (!mainLayer) {
			log::warn("Main Layer not found inside RateStarsLayer!");
			return false;
		}

		CCMenu* menuLayer = nullptr;

		auto& mainLayerChildren = *mainLayer->getChildren();

		// Loop through the root of the game tree until we find the main layer.
		for (int i = 0; i < mainLayerChildren.count(); i++) {
			auto node = static_cast<CCNode*>(mainLayerChildren.objectAtIndex(i));

			if (auto menu = dynamic_cast<CCMenu*>(node)) {
				menuLayer = menu;
				break;
			}
		}

		if (!menuLayer) {
			log::warn("Main CCMenu Layer could not be found!");
			return false;
		}

		CCMenuItemSpriteExtra* button;

		int starsRated = 0; // Used to log the amount of stars that the level got rated
		
		// Only rate the requested difficulty if the user doesn't have a difficulty override.
		if (!Mod::get()->getSettingValue<bool>("toggle-override-difficulty-rate")){
			if (level->m_starsRequested == 0) { // If they never requested any stars (N/A)
				button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(4));
				starsRated = 5;
			} else {
				button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(level->m_starsRequested));
				starsRated = level->m_starsRequested;
			}
		} else { // This is if the user specified a difficulty override.
			button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(Mod::get()->getSettingValue<int>("override-difficulty-rate") - 1));
			starsRated = Mod::get()->getSettingValue<int>("override-difficulty-rate");
		}

		// Only rate the requested difficulty if the user chose it.
		if (Mod::get()->getSettingValue<std::string>("rate-mode-selection") == "Requested Difficulty"){
			if (level->m_starsRequested == 0) { // If they never requested any stars (N/A)
				button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(4));
				starsRated = 5;
			} else {
				button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(level->m_starsRequested));
				starsRated = level->m_starsRequested;
			}
		} else if(Mod::get()->getSettingValue<std::string>("rate-mode-selection") == "Current Difficulty") { // Only rate the current difficulty if the user chose it.
			starsRated = level->getAverageDifficulty();

			switch (starsRated) {
				case 0:
					starsRated = 5;
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

			button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(starsRated - 1));
		} else { // If the user chose override.
			button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(Mod::get()->getSettingValue<int>("override-difficulty-rate") - 1));
			starsRated = Mod::get()->getSettingValue<int>("override-difficulty-rate");
		}

		// Check if the difficulty selection button exists
		if (!button) {
			log::warn("Difficulty selection button not found!");
			return false;
		}

		button->activate();
				
		button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(11));

		// Check if the submit rating button exists
		if (!button) {
			log::warn("Submit rating button not found!");
			return false;
		}

		button->activate();

		// Do it all again because robtop

		// Open the rate menu.
		m_starRateBtn->activate();

		RateStarsLayer* newRateLayer = nullptr;

		auto& newChildren = *getParent()->getChildren();

		// Loop through the root of the game tree until we find the rate menu.
		for (int i = 0; i < newChildren.count(); i++) {
			auto node = static_cast<CCNode*>(newChildren.objectAtIndex(i));

			if (auto rate = dynamic_cast<RateStarsLayer*>(node)) {
				newRateLayer = rate;
				break;
			}
		}

		if (!newRateLayer) {
			log::warn("RateStarsLayer could not be found!");
			return false;
		}

		// We can just use the first object because it is the only object.
		auto newMainLayer = static_cast<CCNode*>(newRateLayer->getChildren()->objectAtIndex(0));

		if (!newMainLayer) {
			log::warn("Main Layer not found inside RateStarsLayer!");
			return false;
		}

		CCMenu* newMenuLayer = nullptr;

		auto& newMainLayerChildren = *newMainLayer->getChildren();

		// Loop through the root of the game tree until we find the main layer.
		for (int i = 0; i < newMainLayerChildren.count(); i++) {
			auto node = static_cast<CCNode*>(newMainLayerChildren.objectAtIndex(i));

			if (auto menu = dynamic_cast<CCMenu*>(node)) {
				newMenuLayer = menu;
				break;
			}
		}

		if (!newMenuLayer) {
			log::warn("Main CCMenu Layer could not be found!");
			return false;
		}

		CCMenuItemSpriteExtra* newButton;

		int newStarsRated = 0; // Used to log the amount of stars that the level got rated
		
		// Only rate the requested difficulty if the user chose it.
		if (Mod::get()->getSettingValue<std::string>("rate-mode-selection") == "Requested Difficulty"){
			if (level->m_starsRequested == 0) { // If they never requested any stars (N/A)
				newButton = static_cast<CCMenuItemSpriteExtra*>(newMenuLayer->getChildren()->objectAtIndex(4));
				newStarsRated = 5;
			} else {
				newButton = static_cast<CCMenuItemSpriteExtra*>(newMenuLayer->getChildren()->objectAtIndex(level->m_starsRequested));
				newStarsRated = level->m_starsRequested;
			}
		} else if(Mod::get()->getSettingValue<std::string>("rate-mode-selection") == "Current Difficulty") { // Only rate the current difficulty if the user chose it.
			newStarsRated = level->getAverageDifficulty();

			switch (newStarsRated) {
				case 0:
					newStarsRated = 5;
					break;
				case 1:
					newStarsRated = 2;
					break;
				case 2:
					newStarsRated = 3;
					break;
				case 3:
					newStarsRated = 4;
					break;
				case 4:
					newStarsRated = 6;
					break;
				case 5:
					newStarsRated = 8;
					break;
				case 6:
					newStarsRated = 10;
					break;
				case 7:
					newStarsRated = 1;
					break;
			}

			newButton = static_cast<CCMenuItemSpriteExtra*>(newMenuLayer->getChildren()->objectAtIndex(newStarsRated - 1));
		} else { // If the user chose override.
			newButton = static_cast<CCMenuItemSpriteExtra*>(newMenuLayer->getChildren()->objectAtIndex(Mod::get()->getSettingValue<int>("override-difficulty-rate") - 1));
			newStarsRated = Mod::get()->getSettingValue<int>("override-difficulty-rate");
		}

		// Check if the difficulty selection button exists
		if (!newButton) {
			log::warn("Difficulty selection button not found!");
			return false;
		}

		newButton->activate();

		newButton = static_cast<CCMenuItemSpriteExtra*>(newMenuLayer->getChildren()->objectAtIndex(11));

		// Check if the submit rating button exists
		if (!newButton) {
			log::warn("Submit rating button not found!");
			return false;
		}

		newButton->activate();

		log::info("Successfully rated the level {}*", newStarsRated);

		// yay we done :D
		return true;
	}
};