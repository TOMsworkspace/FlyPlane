#pragma once
#include"cocos2d.h"

USING_NS_CC;

class LoadScene :public Scene {
public:
	static Scene* createScene();
	bool init() override;
	CREATE_FUNC(LoadScene);
private:
};