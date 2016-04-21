#include <iostream>
#include <algorithm>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>

#include "Entity.h"
#include "ScreenCapture.h"

std::vector<cv::Mat> getSpriteList(WorldType world);
cv::Mat getHue(std::string loc);
void findEnemyTemplateInFrame(cv::Mat image, cv::Mat enemyTemplate, std::vector<cv::Rect> boundingBoxes, cv::Scalar drawColor, int match_method, double threshold);

// Global Window handle
HWND emulator_window;

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		emulator_window = hwnd;
		return FALSE;
	}
	return TRUE;
}

int main(int argc, char** argv) {
	cv::VideoCapture cap;
	cv::Mat input;
	cv::Mat inputCh[3];
	cv::Mat recon;
	cv::Mat output;
	cv::Rect marioBoundingRect;
	Entity mario(EntityType::MARIO_SMALL_R);
	bool foundMario = false;
	int marioState = 0;
	DWORD start, end;
	int fps;
	std::vector<Entity> known;
	const int PURGE_TIME = 1000;

	// std::vector<cv::Mat> marioTemplates = loadMarioTemplates();
	int marioThresholds[] = {150000, 150000, 150000, 150000, 150000, 150000};
	// std::vector<cv::Mat> spriteList; // = getSpriteList(WorldType::OVERWORLD);
	Entity::fillSpriteTable(WorldType::OVERWORLD);
	//mario.fillSpriteTable();

	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);

	/* cap.open(argv[1]);

	if (!cap.isOpened()) {
		std::cout << "Cannot open " << argv[1] << std::endl;
		return -1;
	} */

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (strcmp(entry.szExeFile, "fceux.exe") == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				DWORD id = GetProcessId(hProcess);
				EnumWindows((WNDENUMPROC)EnumWindowsProcMy, id);
				CloseHandle(hProcess);
			}
		}
	}

	foundMario = false;

	while (1) {
		start = GetTickCount();

		/*if (newWorldType) {
			// Determine type of world
			spriteList = getSpriteList(world);
		}*/

		// cap >> input;
		input = hwnd2mat(emulator_window);
		if (input.empty()) {
			std::cout << "Empty input" << std::endl;
			continue;
		}

		// Remove alpha component for template matching - I think this is why it works?
		cv::cvtColor(input, input, CV_RGBA2RGB);

		mario.updateState(input, start);
		cv::rectangle(input, mario.getBBox(), cv::Scalar::all(255), 2);
		
		//cv::rectangle(input, mario.getBBox(), cv::Scalar(127,127,0), 2);

		for (int i = 0; i < known.size(); i++) {
			known[i].updateState(input, start);
		}

		std::vector<Entity> newEntities = Entity::watch(input, known, start);

		// Add newEntities to known
		for (int i = 0; i < newEntities.size(); i++) {
			known.push_back(newEntities[i]);
		}

		for (int i = 0; i < known.size(); i++) {
			if (known[i].inFrame()) {
				cv::rectangle(input, known[i].getBBox(), cv::Scalar::all(255), 2);
			}
			else if (start - known[i].timeLastSeen() > PURGE_TIME) {
				known.erase(known.begin() + i);
			}
		}

		// 780000
		std::cout << std::endl;

		int startX = -1;
		int endX = -1;

		for (int i = 0; i < input.cols; i++) {
			while (input.at<Vec3b>(205, i)[0] == 252 && input.at<Vec3b>(205, i)[1] == 148 && input.at<Vec3b>(205, i)[2] == 92) {
				if (startX > -1) {
					endX = i;
				}
				else {
					startX = i;
				}

				if (++i >= input.cols) {
					break;
				}
			}

			if (startX > -1) {
				cv::rectangle(input, cv::Rect(startX, 205, endX - startX, 5), cv::Scalar(255, 0, 0), 2);
				startX = -1;
				endX = -1;
			}
		}

		// cv::cvtColor(input, input, CV_8UC4);
		end = GetTickCount();
		if (end != start) {
			fps = 1000 / (end - start);
		}

		// Put fps on the screen. Maybe make it a toggle option
		std::ostringstream strs;
		strs << fps;
		std::string str = strs.str();
		cv::putText(input, str, cv::Point(15, 30), FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 255), 2);

		cv::imshow("Image", input);

		if (cv::waitKey(1) == 27) {
			break;
		}
	}

	return 0;
}

cv::Mat getHue(std::string loc) {
	cv::Mat tmp = cv::imread(loc, CV_LOAD_IMAGE_COLOR);
	cv::cvtColor(tmp, tmp, cv::COLOR_BGR2HSV);
	cv::Mat ch[3];
	cv::split(tmp, ch);
	return ch[0];
}

// CV_TM_SQDIFF for match method presently.
// TODO - Optimization
void findEnemyTemplateInFrame(cv::Mat image, cv::Mat enemyTemplate, std::vector<cv::Rect> boundingBoxes, cv::Scalar drawColor, int match_method, double threshold) {
	
	cv::Mat result;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	double minVal;
	double maxVal;

	// Create the result matrix
	int result_cols = image.cols - enemyTemplate.cols + 1;
	int result_rows = image.rows - enemyTemplate.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	cv::matchTemplate(image, enemyTemplate, result, match_method);

	// Try and find the first enemy template
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	std::cout << minVal << std::endl;

	// TODO - allow for max match technique
	while (minVal < threshold) {
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
		{
			matchLoc = minLoc;
		}
		else
		{
			matchLoc = maxLoc;
		}

		// Populate our enemy bounding boxes
		// TODO - Extrapolate enemy size
		boundingBoxes.push_back(cv::Rect(matchLoc, cv::Point(matchLoc.x + enemyTemplate.cols, matchLoc.y + enemyTemplate.rows)));

		// Draw what we see
		cv::rectangle(image, matchLoc, cv::Point(matchLoc.x + enemyTemplate.cols, matchLoc.y + enemyTemplate.rows), drawColor, 2, 8, 0);

		// Fill in our result to remove previously tracked objects
		cv::floodFill(result, matchLoc, cv::Scalar(threshold), 0);

		// Find the next template
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	}
}

std::vector<cv::Mat> getSpriteList(WorldType world) {
	std::vector<cv::Mat> list;

	std::string worldStr;
	if (world == WorldType::OVERWORLD) {
		worldStr = "overworld";
	}
	else {
		worldStr = "underworld";
	}

	// It works for overworld
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/goomba-template.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/koopa-left-template.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/goomba1.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/goomba2.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/goomba-flat.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/koopa1.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/koopa2.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/koopa3.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/koopa4.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/koopa-shell.png"));
	// list.push_back(cv::imread("sprites/enemies/shared/koopa1.png"));
	// list.push_back(cv::imread("sprites/enemies/shared/koopa2.png"));
	// list.push_back(cv::imread("sprites/enemies/shared/koopa3.png"));
	// list.push_back(cv::imread("sprites/enemies/shared/koopa4.png"));
	// list.push_back(cv::imread("sprites/enemies/shared/koopa-shell.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/shell.png"));
	// list.push_back(cv::imread("sprites/enemies/shared/shell.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/piranha1.png"));
	// list.push_back(cv::imread("sprites/enemies/" + worldStr + "/piranha2.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/brick1.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/brick2.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/question1.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/question2.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/question3.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/rock.png"));
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/block-chiseled.png"));
	/* list.push_back(cv::imread("sprites/misc/" + worldStr + "/used-block.png")); */
	// list.push_back(cv::imread("sprites/misc/" + worldStr + "/flagpole.png"));
	// list.push_back(cv::imread("sprites/misc/shared/beam-short.png"));
	// list.push_back(cv::imread("sprites/misc/shared/beam-medium.png"));
	// list.push_back(cv::imread("sprites/misc/shared/beam-long.png"));
	// list.push_back(cv::imread("sprites/misc/shared/pipe-up.png"));
	// list.push_back(cv::imread("sprites/misc/shared/pipe-down.png"));
	// list.push_back(cv::imread("sprites/misc/shared/pipe-left.png"));
	// list.push_back(cv::imread("sprites/misc/shared/pipe-t.png"));
	// list.push_back(cv::imread("sprites/misc/shared/pipe-tess.png"));
	// list.push_back(cv::imread("sprites/powerups/shared/mushroom.png"));
	if (world == WorldType::OVERWORLD) {
		// list.push_back(getHue("sprites/misc/overworld/flagpole.png"));
	}

	return list;
}
