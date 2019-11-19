#include"enemy.h"
#include"constants.h"
#include"AudioEngine.h"

using namespace experimental;

bool Enemy::initWithEnemyType(const EnemyType& type) {
	//��Ҫ����creatWithSpriteFrameNmae
	//switch����ֵ�Ƚϣ��õ�Enemy�����ͼ�������Դ
	//�ó�Ա�����������ͣ���ס֮��������Է���
	m_type = type;
	std::string frameName = "";
	switch (m_type)
	{
	case EnemyType::SMALL_ENEMY:
		frameName = "enemy1.png";
		this->m_speed = -SMALL_ENEMY_SPEED;
		this->m_hp = 1;
		this->m_score = 2;
		break;
	case EnemyType::MIDDLE_ENEMY:
		frameName = "enemy2.png";
		this->m_speed = -MIDDLE_ENEMY_SPEED;
		this->m_hp = 6;
		this->m_score = 10;
		break;
	case EnemyType::BIG_ENEMY:

		frameName = "enemy3_n1.png";
		this->m_speed = -BIG_ENEMY_SPEED;
		this->m_hp = 20;
		this->m_score = 100;
		break;
	default:
		break;
	}
	if (!Sprite::initWithSpriteFrameName(frameName)) {

		return false;
	}
	//��ִ�з��ж���ʱ����ֹͣ��������
	if (this->m_type == EnemyType::BIG_ENEMY) {
		AudioEngine::play2d("big_spaceship.mp3");
		auto ani = Animation::create();
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
		ani->setDelayPerUnit(0.2f);
		ani->setLoops(CC_REPEAT_FOREVER);
		this->runAction(Animate::create(ani));
	}
	return true;
}

Enemy* Enemy::create(const EnemyType& type) {
	auto enemy = new Enemy();
	if (enemy&&enemy->initWithEnemyType(type)) {
		enemy->autorelease();
		return enemy;
	}
	delete enemy;
	enemy = nullptr;
}

void Enemy::enemyBooming() {
	//����Ǩ�ƣ���װ
	auto ani = Animation::create();

	switch (m_type)
	{
	case EnemyType::SMALL_ENEMY:
		for (auto i = 0; i < 4; i++) {
			auto frameName = StringUtils::format("enemy1_down%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}
		AudioEngine::play2d("enemy1_down.mp3");
		break;
	case EnemyType::MIDDLE_ENEMY:
		for (auto i = 0; i < 4; i++) {
			auto frameName = StringUtils::format("enemy2_down%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}
		AudioEngine::play2d("enemy2_down.mp3");
		break;
	case EnemyType::BIG_ENEMY:
		
		for (auto i = 0; i < 6; i++) {
			auto frameName = StringUtils::format("enemy3_down%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		}
		AudioEngine::play2d("enemy3_down.mp3");
		break;
	default:
		break;
	}

	/*for (auto i = 0; i < 4; i++) {
	auto frameName = StringUtils::format("enemy1_down%d.png",i+1);
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}*/

	ani->setDelayPerUnit(0.2f);
	auto animate = Animate::create(ani);
	//Ҫ�õл��ڱ�ը֮��ɾ��
	//Sequence��� �����ж�������ݲ������Ⱥ�˳��ִ�У�ע����������һ��ҪΪNULL��������־
	//NULL ���﷨  nullptr ���﷨
	//CallFuncN��䣺
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
	this->stopAllActions();
	this->runAction(seq);
}

void Enemy::move() {
	this->setPositionY(this->getPositionY() + this->getSpeed());
}
/*
@return ��Ѫ���Ƿ�����
*/
//һ��Ĭ��һ�μ���Ѫ
bool Enemy::hit() {
	//�Ƿ��Ѿ�����
	if (this->m_hp <= 0) return true;
	this->m_hp--;
	//�˿��Ƿ�����
	if (this->m_hp <= 0) {
		//��ը����
		this->enemyBooming();
		return true;
	}
	else {
		//���˶���
		switch (m_type) {
		case EnemyType::MIDDLE_ENEMY:
		{
			auto ani = Animation::create();
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_hit.png"));
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
			ani->setDelayPerUnit(0.2f);
			runAction(Animate::create(ani));
		}
		break;

		case EnemyType::BIG_ENEMY:
		{
			auto ani = Animation::create();
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png"));
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
			ani->setDelayPerUnit(0.2f);
			runAction(Animate::create(ani));
		}
		break;
		default:
			break;
		}
	}
	return false;
}