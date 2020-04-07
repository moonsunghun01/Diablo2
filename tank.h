#pragma once
#include"gameNode.h"
#include"tankMap.h"
#include"bullet.h"

enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_UP,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_DOWN
};
class tank :  public gameNode
{
public:
	tank();
	~tank();
	
	int _frameY;
	float _speed;
	float _x, _y;
	int tileX, tileY;//탱크가 밟고 있는 타일 번호

	TANKDIRECTION _direction;
	tankMap* _tankMap;
	bullet* _bullet;

	image* _image;

	RECT _rc;

	HRESULT init(const char* imageName);
	void release();
	void update();
	void render();


	void setTankPosition(RECT rc);
	void tankMove();
	void fire();
	void bulletCollision();

	void setTankMapMemoryAdress(tankMap* tm) { _tankMap = tm; }




};

