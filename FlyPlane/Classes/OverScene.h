
#pragma once
#include"cocos2d.h"

USING_NS_CC;

class  OverScene :public Scene
{
public:
	static Scene* createScene(int Score);
	bool init(int score);
	//CREATE_FUNC(LoadScene);
};