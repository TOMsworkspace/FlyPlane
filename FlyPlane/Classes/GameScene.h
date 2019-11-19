#pragma once
#include"cocos2d.h"
#include"enemy.h"

USING_NS_CC;

class GameScene :public Scene {
public:
	//��ࣻÿ����������һ��ָ���ַ�����еľֲ�����������ջ�У�
	//��ַ��ת�ļĴ�������������ʾָ����׵�ַ����һ�ֲ���ϵͳ���ɵ������ַ
	//����һ����������һ��ָ��ȷ����ڴ棨�ֲ������������ڴ��н������㣬��ָ���ֵ����ڴ棬����ջ�����������������
	//
	GameScene() : m_offset(Vec2::ZERO), m_totalScore(0), m_doubleBulletCount(0), m_ufoType(0), m_bombcount(0), m_isOver(false) {}
	static Scene* createScene();
	bool init() override;
	CREATE_FUNC(GameScene);
	void update(float) override;
private:
	bool m_isOver;
	int m_doubleBulletCount;//˫�ӵ�����
	Vec2 m_offset;          //
	int m_ufoType;          //�������� 
	int m_totalScore;       //�÷�
	int m_bombcount;		//ը������
	void createBullet(float);   //�����ӵ�
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
