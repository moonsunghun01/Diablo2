#pragma once
#include"gameNode.h"
class camel :  public gameNode
{
private:
	image * _camel;
	/*animation* _ani1;
	animation* _ani2;
	animation* _ani3;*/

	animation* _ani;



public:
	camel();
	~camel();

	HRESULT init();
	void release();
	void update();
	void render();
};

