#pragma once
#include"cocos2d.h"
USING_NS_CC;
//#include"constants.h"

//C语言的枚举，全局性的枚举
/*enum EnemyType {
SMALL_ENEMY,
MIDDLE_ENEMY,
BIG_ENEMY
};*/
//C++的枚举，有名称（类型）限定，枚举类
enum class EnemyType {
	SMALL_ENEMY,
	MIDDLE_ENEMY,
	BIG_ENEMY
};

class Enemy :public Sprite {
public:
	Enemy() :m_type(EnemyType::SMALL_ENEMY), m_speed(0), m_hp(0), m_score(0) {}
	bool initWithEnemyType(const EnemyType&);
	//删掉字符串，可以通过枚举直接知道创建精灵所需的图片
	//保证封装性
	static Enemy* create(const EnemyType&);

	void move();
	bool hit();
	//移动速度
	CC_SYNTHESIZE(float, m_speed, Speed);
	//生命值
	CC_SYNTHESIZE_READONLY(float, m_hp, Hp);
	//奖励分值
	CC_SYNTHESIZE_READONLY(int, m_score, Score);

	CC_SYNTHESIZE_READONLY(EnemyType ,m_type, Type);
	void enemyBooming();
private:
	//EnemyType m_type;
};