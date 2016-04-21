#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "Entity.h"

// The top left point of the rectangle is where the sprite starts based off of its template
void Entity::setBoundingBox() {
	switch (type) {
	case EntityType::MARIO_SMALL_L: bbox = cv::Rect(-3, -1, 13, 16); break;
	case EntityType::MARIO_SMALL_R: bbox = cv::Rect(-3, -1, 13, 16); break;
	case EntityType::MARIO_BIG_L: bbox = cv::Rect(-4, -3, 16, 32); break;
	case EntityType::MARIO_BIG_R: bbox = cv::Rect(-3, -3, 16, 32); break;
	case EntityType::MARIO_FIRE_L: bbox = cv::Rect(-4, -3, 16, 32); break;
	case EntityType::MARIO_FIRE_R: bbox = cv::Rect(-3, -3, 16, 32); break;
	case EntityType::GOOMBA: bbox = cv::Rect(-3, -5, 16, 16); break;
	case EntityType::KOOPA_L: bbox = cv::Rect(-5, -13, 16, 24); break;
	case EntityType::KOOPA_R: bbox = cv::Rect(-2, -13, 16, 24); break;
	//case EntityType::KOOPA_RED_L: bbox = cv::Rect(-5, -13, 16, 24); break;
	//case EntityType::KOOPA_RED_R: bbox = cv::Rect(-2, -13, 16, 24); break;
	case EntityType::SHELL: bbox = cv::Rect(-4, -3, 16, 14); break;
	/*case EntityType::SHELL_RED: bbox = cv::Rect(-4, -3, 16, 14); break;
	case EntityType::PIRANHA: bbox = cv::Rect(); break;
	case EntityType::BRICK: bbox = cv::Rect(0, 0, 16, 16); break;
	case EntityType::QUESTION: bbox = cv::Rect(); break;
	case EntityType::ROCK: bbox = cv::Rect(); break;
	case EntityType::FLAGPOLE: bbox = cv::Rect(); break;
	case EntityType::BEAM: bbox = cv::Rect(); break;
	case EntityType::PIPE: bbox = cv::Rect(); break;
	case EntityType::MUSHROOM: bbox = cv::Rect(); break;
	case EntityType::FIREFLOWER: bbox = cv::Rect(); break;*/
	}
}

int Entity::getDetThresh(EntityType type) {
	int detThresh = 0;
	switch (type) {
	case EntityType::MARIO_SMALL_L: detThresh = 150000; break;
	case EntityType::MARIO_SMALL_R: detThresh = 150000; break;
	case EntityType::MARIO_BIG_L: detThresh = 150000; break;
	case EntityType::MARIO_BIG_R: detThresh = 150000; break;
	case EntityType::MARIO_FIRE_L: detThresh = 150000; break;
	case EntityType::MARIO_FIRE_R: detThresh = 150000; break;
	case EntityType::GOOMBA: detThresh = 780000; break;
	case EntityType::KOOPA_L: detThresh = 150000; break;
	case EntityType::KOOPA_R: detThresh = 150000; break;
	//case EntityType::KOOPA_RED_L: detThresh = 150000; break;
	//case EntityType::KOOPA_RED_R: detThresh = 150000; break;
	case EntityType::SHELL: detThresh = 150000; break;
	/*case EntityType::SHELL_RED: detThresh = 150000; break;
	case EntityType::PIRANHA: detThresh = 150000; break;
	case EntityType::BRICK: detThresh = 150000; break;
	case EntityType::QUESTION: detThresh = 150000; break;
	case EntityType::ROCK: detThresh = 150000; break;
	case EntityType::FLAGPOLE: detThresh = 150000; break;
	case EntityType::BEAM: detThresh = 150000; break;
	case EntityType::PIPE: detThresh = 150000; break;
	case EntityType::MUSHROOM: detThresh = 150000; break;
	case EntityType::FIREFLOWER: detThresh = 150000; break;*/
	}
	return detThresh;
}

void Entity::fillSpriteTable(WorldType world) {
	std::string worldStr;
	if (world == WorldType::OVERWORLD) {
		worldStr = "overworld";
	}
	else {
		worldStr = "underworld";
	}

	// Fill spriteTable
	spriteTable[EntityType::MARIO_SMALL_L] = cv::imread("sprites/mario/mario-small-left.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::MARIO_SMALL_R] = cv::imread("sprites/mario/mario-small-right.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::MARIO_BIG_L] = cv::imread("sprites/mario/mario-big-left.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::MARIO_BIG_R] = cv::imread("sprites/mario/mario-big-right.png", CV_LOAD_IMAGE_COLOR);;
	spriteTable[EntityType::MARIO_FIRE_L] = cv::imread("sprites/mario/mario-fire-left.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::MARIO_FIRE_R] = cv::imread("sprites/mario/mario-fire-right.png", CV_LOAD_IMAGE_COLOR);
	Entity::spriteTable[EntityType::GOOMBA] = cv::imread("sprites/enemies/" + worldStr + "/goomba-template.png", CV_LOAD_IMAGE_COLOR);
	Entity::spriteTable[EntityType::KOOPA_L] = cv::imread("sprites/enemies/" + worldStr + "/koopa-l-template.png", CV_LOAD_IMAGE_COLOR);
	Entity::spriteTable[EntityType::KOOPA_R] = cv::imread("sprites/enemies/" + worldStr + "/koopa-r-template.png", CV_LOAD_IMAGE_COLOR);
	//Entity::spriteTable[EntityType::KOOPA_RED_L] = cv::imread("sprites/enemies/shared/koopa-l-template.png", CV_LOAD_IMAGE_COLOR);
	//Entity::spriteTable[EntityType::KOOPA_RED_R] = cv::imread("sprites/enemies/shared/koopa-r-template.png", CV_LOAD_IMAGE_COLOR);
	Entity::spriteTable[EntityType::SHELL] = cv::imread("sprites/enemies/" + worldStr + "/shell-template.png", CV_LOAD_IMAGE_COLOR);
	/*Entity::spriteTable[EntityType::SHELL_RED] = cv::imread("sprites/enemies/shared/shell-template.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::PIRANHA] = cv::imread("sprites/enemies/" + worldStr + "/goomba-template.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::BRICK] = cv::imread("sprites/misc/" + worldStr + "/brick1.png", CV_LOAD_IMAGE_COLOR);
	spriteTable[EntityType::QUESTION]
	spriteTable[EntityType::ROCK]
	spriteTable[EntityType::FLAGPOLE]
	spriteTable[EntityType::BEAM]
	spriteTable[EntityType::PIPE]
	spriteTable[EntityType::MUSHROOM]
	spriteTable[EntityType::FIREFLOWER]*/
}

Entity::Entity(cv::Point loc, EntityType type, int timeMS) {
	this->loc = loc;
	this->type = type;
	msLastSeen = timeMS;
	isInFrame = true;

	setBoundingBox();
}

Entity::Entity(EntityType type) {
	this->loc = cv::Point(0, 0);
	this->type = type;
	msLastSeen = 0;
	isInFrame = false;

	setBoundingBox();
}

cv::Point Entity::getLoc() {
	return loc;
}

cv::Rect Entity::getBBox() {
	return cv::Rect(loc.x + bbox.x, loc.y + bbox.y, bbox.width, bbox.height);
}

cv::Mat Entity::getSprite() {
	return spriteTable[type];
}

EntityType Entity::getType() {
	return type;
}

void Entity::setLoc(cv::Point loc) {
	this->loc = loc;
}

void Entity::setType(EntityType type) {
	this->type = type;
}

bool Entity::isPassable() { // How to handle breakable bricks?
	switch (type) {
	//case EntityType::MARIO:
	case EntityType::SHELL:
	/*case EntityType::FLAGPOLE:
	case EntityType::MUSHROOM:
	case EntityType::FIREFLOWER:*/
		return true;
	default:
		return false;
	}
}

bool Entity::isHostile() { // Technically should also return true for a moving shell
	switch (type) {
	case EntityType::GOOMBA:
	case EntityType::KOOPA_R:
	case EntityType::KOOPA_L:
	//case EntityType::PIRANHA:
		return true;
	default:
		return false;
	}
}

bool Entity::updateState(cv::Mat image, int timeMS) {
	cv::Mat result;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	double minVal;
	double maxVal;
	int method = cv::TM_SQDIFF;

	const int MARGIN = 5;
	int x, y, width, height;
	if (!isInFrame && (
		type == EntityType::MARIO_SMALL_L ||
		type == EntityType::MARIO_SMALL_R ||
		type == EntityType::MARIO_BIG_L   ||
		type == EntityType::MARIO_BIG_R   ||
		type == EntityType::MARIO_FIRE_L  ||
		type == EntityType::MARIO_FIRE_R)) {
		x = 0;
		y = 0;
		width = image.cols;
		height = image.rows;
	}
	else {
		x = std::max(0, bbox.x + loc.x - MARGIN);
		y = std::max(0, bbox.y + loc.y - MARGIN);
		width = std::min(bbox.width + MARGIN * 2, image.cols - x);
		height = std::min(bbox.height + MARGIN * 2, image.rows - y);
	}
	cv::Mat roi = image(cv::Rect(x, y, width, height));
	std::vector<EntityType> possStates = nextStates();

	for (EntityType tmpType : possStates) {
		// Create the result matrix
		int result_cols = roi.cols - spriteTable[tmpType].cols + 1;
		int result_rows = roi.rows - spriteTable[tmpType].rows + 1;
		result.create(result_rows, result_cols, CV_32FC1);

		// Do the Matching and Normalize
		cv::matchTemplate(roi, spriteTable[tmpType], result, method);

		// Try and find the first enemy template
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

		if (minVal < getDetThresh(tmpType)) { // We found it
			if (!isInFrame && (
				type == EntityType::MARIO_SMALL_L ||
				type == EntityType::MARIO_SMALL_R ||
				type == EntityType::MARIO_BIG_L ||
				type == EntityType::MARIO_BIG_R ||
				type == EntityType::MARIO_FIRE_L ||
				type == EntityType::MARIO_FIRE_R)) {
				loc = minLoc;
			}
			else {
				loc = cv::Point(minLoc.x + loc.x + bbox.x - MARGIN, minLoc.y + loc.y + bbox.y - MARGIN);
			}
			type = tmpType;

			setBoundingBox();
			isInFrame = true;
			msLastSeen = timeMS;

			return true;
		}
	}

	isInFrame = false;
	return false;
}


bool Entity::inFrame() {
	return isInFrame;
}

std::vector<Entity> Entity::watch(cv::Mat image, std::vector<Entity> known, int timeMS) {
	std::vector<Entity> ret;
	
	// Only look at the right
	int origWidth = image.size().width;
	image = image(cv::Rect(image.size().width - 40, 0, 40, image.size().height));
	
	cv::Mat result;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	double minVal;
	double maxVal;
	int method = cv::TM_SQDIFF;

	for (EntityType t = EntityType::GOOMBA; t != EntityType::SIZE_ENTITY_TYPE; t = static_cast<EntityType>(t + 1)) {
		// Create the result matrix
		int result_cols = image.cols - spriteTable[t].cols + 1;
		int result_rows = image.rows - spriteTable[t].rows + 1;
		result.create(result_rows, result_cols, CV_32FC1);

		cv::Mat maskedImage = image.clone();
		for (int i = 0; i < known.size(); i++) {
			if (known[i].getType() == t) {
				cv::Rect bbox = known[i].getBBox();
				bbox.x -= (origWidth - maskedImage.cols);
				std::cout << bbox.x << std::endl;
				cv::rectangle(maskedImage, bbox, cv::Scalar::all(255), CV_FILLED);
			}
		}

		cv::imshow("Masked Watch", maskedImage);

		// Do the Matching and Normalize
		cv::matchTemplate(maskedImage, spriteTable[t], result, method);

		// White out the Entities we know about of this type
		/* for (int i = 0; i < known.size(); i++) {
			if (known[i].getType() == t) {
				for (int m = std::max(0, bbox.y); m < std::min(result.size().height, bbox.y + bbox.height); m++) {
					for (int n = std::max(0, bbox.x); m < std::min(result.size().width, bbox.x + bbox.width); n++) {
						result.at<float>(cv::Point(n, m)) = std::numeric_limits<float>::max();
					}
				}
			}
		} */

		// Try and find the first enemy template
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
		// minLoc = cv::Point(minLoc.x + spriteTable[t].cols - 1, minLoc.y + spriteTable[t].rows - 1);

		if (minVal < getDetThresh(t)) { // We found one
			minLoc.x += (origWidth - maskedImage.cols);
			ret.push_back(Entity(minLoc, t, timeMS));
		}
	}

	return ret;
}

int Entity::timeLastSeen() {
	return msLastSeen;
}

cv::Mat Entity::spriteTable[EntityType::SIZE_ENTITY_TYPE];

std::vector<EntityType> Entity::nextStates() {
	std::vector<EntityType> ret;
	switch (type) {
	case EntityType::MARIO_SMALL_L:
		ret.push_back(EntityType::MARIO_SMALL_L);
		ret.push_back(EntityType::MARIO_SMALL_R);
		ret.push_back(EntityType::MARIO_BIG_L);
		break;
	case EntityType::MARIO_SMALL_R:
		ret.push_back(EntityType::MARIO_SMALL_R);
		ret.push_back(EntityType::MARIO_SMALL_L);
		ret.push_back(EntityType::MARIO_BIG_R);
		break;
	case EntityType::MARIO_BIG_L:
		ret.push_back(EntityType::MARIO_BIG_L);
		ret.push_back(EntityType::MARIO_BIG_R);
		ret.push_back(EntityType::MARIO_SMALL_L);
		ret.push_back(EntityType::MARIO_FIRE_L);
		break;
	case EntityType::MARIO_BIG_R:
		ret.push_back(EntityType::MARIO_BIG_R);
		ret.push_back(EntityType::MARIO_BIG_L);
		ret.push_back(EntityType::MARIO_SMALL_R);
		ret.push_back(EntityType::MARIO_FIRE_R);
		break;
	case EntityType::MARIO_FIRE_L:
		ret.push_back(EntityType::MARIO_FIRE_L);
		ret.push_back(EntityType::MARIO_FIRE_R);
		ret.push_back(EntityType::MARIO_SMALL_L);
		break;
	case EntityType::MARIO_FIRE_R:
		ret.push_back(EntityType::MARIO_FIRE_R);
		ret.push_back(EntityType::MARIO_FIRE_L);
		ret.push_back(EntityType::MARIO_SMALL_R);
		break;
	case EntityType::GOOMBA:
		ret.push_back(EntityType::GOOMBA);
		break;
	case EntityType::KOOPA_L:
		ret.push_back(EntityType::KOOPA_L);
		ret.push_back(EntityType::KOOPA_R);
		ret.push_back(EntityType::SHELL);
		break;
	case EntityType::KOOPA_R:
		ret.push_back(EntityType::KOOPA_R);
		ret.push_back(EntityType::KOOPA_L);
		ret.push_back(EntityType::SHELL);
		break;
	case EntityType::SHELL:
		ret.push_back(EntityType::SHELL);
		break;
	}
	return ret;
}
