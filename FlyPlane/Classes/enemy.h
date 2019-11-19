#pragma once
#include"cocos2d.h"
USING_NS_CC;
//#include"constants.h"

//C���Ե�ö�٣�ȫ���Ե�ö��
/*enum EnemyType {
SMALL_ENEMY,
MIDDLE_ENEMY,
BIG_ENEMY
};*/
//C++��ö�٣������ƣ����ͣ��޶���ö����
enum class EnemyType {
	SMALL_ENEMY,
	MIDDLE_ENEMY,
	BIG_ENEMY
};

class Enemy :public Sprite {
public:
	Enemy() :m_type(EnemyType::SMALL_ENEMY), m_speed(0), m_hp(0), m_score(0) {}
	bool initWithEnemyType(const EnemyType&);
	//ɾ���ַ���������ͨ��ö��ֱ��֪���������������ͼƬ
	//��֤��װ��
	static Enemy* create(const EnemyType&);

	void move();
	bool hit();
	//�ƶ��ٶ�
	CC_SYNTHESIZE(float, m_speed, Speed);
	//����ֵ
	CC_SYNTHESIZE_READONLY(float, m_hp, Hp);
	//������ֵ
	CC_SYNTHESIZE_READONLY(int, m_score, Score);

	CC_SYNTHESIZE_READONLY(EnemyType ,m_type, Type);
	void enemyBooming();
private:
	//EnemyType m_type;
};