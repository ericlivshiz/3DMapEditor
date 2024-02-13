#pragma once

#include "../ObjectMgr/ObjectMgr.h"
// #include <SFML/Graphics.hpp>

class Keyboard {
public:
	void handleMovementKeys(vec3D& vCamera, Math& math, vec3D& vForward, bool isDrawMode) {
		handleUpPress(vCamera, isDrawMode);
		handleDownPress(vCamera, isDrawMode);
		handleRightPress(isDrawMode);
		handleLeftPress(isDrawMode);
		handleDPress(vCamera, isDrawMode);
		handleAPress(vCamera, isDrawMode);
		handleWPress(vCamera, math, vForward, isDrawMode);
		handleSPress(vCamera, math, vForward, isDrawMode);
	}

	void handleKeyBinds(bool& drawWireFrames) {
		PKeyBind(drawWireFrames);
		OKeyBind(drawWireFrames);
	}

	float Speed = 0.5f;
	float fYaw = 0;
	float fYawInc = 0.05f;

private:
	void handleUpPress(vec3D& vCamera, bool& idm){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (!idm)
				vCamera.y += Speed;

			if (idm)
				vCamera.y -= Speed;
		}
	}

	void handleDownPress(vec3D& vCamera, bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
		{
			if (!idm)
				vCamera.y -= Speed;

			if (idm)
				vCamera.y += Speed;
		}

	}

	void handleDPress(vec3D& vCamera, bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (!idm)
				vCamera.x -= Speed;

			if (idm)
				vCamera.x += Speed;
		}
	}

	void handleAPress(vec3D& vCamera, bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (!idm)
				vCamera.x += Speed;

			if (idm)
				vCamera.x -= Speed;
		}
	}

	void handleWPress(vec3D& vCamera, Math& math, vec3D& vForward, bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
		{
			if (!idm)
				vCamera = math.Vector_Sub(vCamera, vForward);

			if (idm)
				vCamera = math.Vector_Add(vCamera, vForward);

		}
	}
	void handleSPress(vec3D& vCamera, Math& math, vec3D& vForward, bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (!idm)
				vCamera = math.Vector_Add(vCamera, vForward);


			if (idm)
				vCamera = math.Vector_Sub(vCamera, vForward);


		}
	}

	void handleRightPress(bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			fYaw += fYawInc;
	}

	void handleLeftPress(bool& idm) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			fYaw -= fYawInc;
	}

	void PKeyBind(bool& wf) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			wf = true;
	}

	void OKeyBind(bool& wf) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			wf = false;
	}
};