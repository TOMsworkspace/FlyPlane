#pragma once
#include"cocos2d.h"
#include"enemy.h"

USING_NS_CC;

class GameScene :public Scene {
public:
	//汇编；每个函数都有一个指令地址，所有的局部变量都放在栈中，
	//地址跳转的寄存器，函数名表示指令的首地址，是一种操作系统生成的虚拟地址
	//运行一个函数，即一个指令，先分配内存（局部变量），在内存中进行运算，把指令返回值存回内存，拉下栈顶，会产生垃圾数据
	//
	GameScene() : m_offset(Vec2::ZERO), m_totalScore(0), m_doubleBulletCount(0), m_ufoType(0), m_bombcount(0), m_isOver(false) {}
	static Scene* createScene();
	bool init() override;
	CREATE_FUNC(GameScene);
	void update(float) override;
private:
	bool m_isOver;
	int m_doubleBulletCount;//双子弹数量
	Vec2 m_offset;          //
	int m_ufoType;          //道具类型 
	int m_totalScore;       //得分
	int m_bombcount;		//炸弹数量
	void createBullet(float);   //产生子弹
	void createenemy(const EnemyType & type);
	void createSmallEnemy(float);
	void createMiddleEnemy(float);
	void createBigEnemy(float);
	void createUfo(float);
	void createenemyunit(Vec2);
	void enemyunit(float);
	void changebombcount();
	void gameover();
	Vector <Sprite *> m_bullets;
	Vector <Enemy *> m_enemys;
	Vector<Sprite*> m_ufoes;
};
