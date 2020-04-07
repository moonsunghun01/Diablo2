#pragma once
class camera
{
private:
	int _cameraX;
	int _cameraY;

public:

	HRESULT init();

	int getCameraX() { return _cameraX; }
	void setCameraX(int cameraX) { _cameraX = cameraX; }

	int getCameraY() { return _cameraY; }
	void setCameraY(int cameraY) { _cameraY = cameraY; }

};

