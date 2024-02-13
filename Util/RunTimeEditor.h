#pragma once

#include "../Util/Program.h"
#include "../Util/Math.h"


// EDITOR_STATE 
// 0 = no editor ||| 1 = editor ||| 2 = drawing mode

class RunTimeEditor {
public:
	const int EDITOR_STATE = 1;


	bool drawWireFrames = false;
	bool isDrawMode = false;
	bool moveObject = false;
	bool setObjectPos = false;
	bool saveDrawing = false;
	bool clearScreen = false;

	// Like a bool but with 3 states:
	// 1. Uninitialized - 0
	// 2. True (Save the drawing) - 1
	// 3. False - 2
	int drawingModeInput = 0;


	RunTimeEditor(){
		if (EDITOR_STATE == 1) {
			introMessage();
		}
	}

	// Fly Mode should let you hold the vecTriToRaster right by the camera at all times, while you have
	// full range to move the camera and objects such as drawing are rendered in perspective mode
	void takeInput() {
		int userInput = 0;
		std::cout << "1. Move object" << std::endl;
		std::cout << "2. Set Object Pos" << std::endl;
		std::cout << "3. Select object" << std::endl;
		std::cout << "4. Drawing Mode" << std::endl;
		std::cout << "5. Fly Mode" << std::endl;
		std::cout << "5. quit" << std::endl;
		std::cin >> userInput;
		if (userInput == 1)
			moveObject = true;

		if (userInput == 2)
			setObjectPos = true;

		if (userInput == 4) {
			int dmi = 0;
			isDrawMode = true;
			std::cout << "PRESS 1 TO SAVE & EXIT DRAWING MODE || PRESS 2 TO EXIT DRAWING MODE" << std::endl << std::endl;
			std::cin >> dmi;
			drawingModeInput = dmi;

			if (drawingModeInput == 1) {
				// Find a way to save vecToRaster vector at this point
				saveDrawing = true;
			}

			if (drawingModeInput == 2) {
				clearScreen = true;
				isDrawMode = false;
			}
		}
	}

	//bool newVecMade() {
	//	// instead of 0,0,0 it needs to hold
	//	if (newVec != sf::Vector3f(0, 0, 0))
	//		return true;

	//	return false;
	//}

	sf::Vector3f newObjPos() {
		sf::Vector3f newPos;
		std::cout << "How much would you like to move X by: " << std::endl;
		std::cin >> newPos.x;
		std::cout << std::endl;
		std::cout << "How much would you like to move Y by: " << std::endl;
		std::cin >> newPos.y;
		std::cout << std::endl;
		std::cout << "How much would you like to move Z by: " << std::endl;
		std::cin >> newPos.z;
		std::cout << std::endl;
		return newPos;
	}

private:
	void introMessage() {
		std::cout << "Welcome to the SHVIZIL Engine!" << std::endl << std::endl;
		std::cout << "You currently have the real-time-editor turned on: " << std::endl;
		std::cout << "---------------------------------------------------------" << std::endl << std::endl;
	}

	
};