#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(AutoLevelRate, LevelInfoLayer) {
	void levelDownloadFinished(GJGameLevel* p0) {
		LevelInfoLayer::levelDownloadFinished(p0); // Run the original levelDownloadFinished code before running our custom code.
		
		// Make sure we haven't rated the level yet.
		if (!m_starRateBtn) return;
		if (!m_starRateBtn->isEnabled()) return;

		// Make sure the user has the feature enabled.
		if (!Mod::get()->getSettingValue<bool>("enable-auto-rater")) return;
		
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
			return;
		}
	
		// We can just use the first object because it is the only object.
		auto mainLayer = static_cast<CCNode*>(rateLayer->getChildren()->objectAtIndex(0));

		if (!mainLayer) {
			log::warn("Main Layer not found inside RateStarsLayer!");
			return;
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
			return;
		}

		CCMenuItemSpriteExtra* button;

		int starsRated = 0; // Used to log the amount of stars that the level got rated
		
		// Only rate the requested difficulty if the user doesn't have a difficulty override.
		if (!Mod::get()->getSettingValue<bool>("toggle-override-difficulty-rate")){
			if (p0->m_starsRequested == 0) { // If they never requested any stars (N/A)
				button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(4));
				starsRated = 5;
			} else {
				button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(p0->m_starsRequested));
				starsRated = p0->m_starsRequested;
			}
		} else { // This is if the user specified a difficulty override.
			button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(Mod::get()->getSettingValue<int>("override-difficulty-rate") - 1));
			starsRated = Mod::get()->getSettingValue<int>("override-difficulty-rate");
		}

		// Check if the difficulty selection button exists
		if (!button) {
			log::warn("Difficulty selection button not found!");
			return;
		}

		button->activate();
				
		button = static_cast<CCMenuItemSpriteExtra*>(menuLayer->getChildren()->objectAtIndex(11));

		// Check if the submit rating button exists
		if (!button) {
			log::warn("Submit rating button not found!");
			return;
		}

		button->activate();

		CCSprite* disableButton = CCSprite::createWithSpriteFrameName("GJ_starBtn2_001.png");

		// Check if the disable rate button sprite exists
		if (!disableButton) {
			log::warn("Disabled Rate Button sprite not found!");
			return;
		}

		m_starRateBtn->setSprite(disableButton);
		m_starRateBtn->m_bEnabled = false;

		log::info("Successfully rated the level {}*!", starsRated);

		// yay we done :D
	}
};