#include"enemy.h"
#include"constants.h"
#include"AudioEngine.h"

using namespace experimental;

bool Enemy::initWithEnemyType(const EnemyType& type) {
	//不要调用creatWithSpriteFrameNmae
	//switch语句等值比较，得到Enemy的类型及引用资源
	//用成员变量保存类型，记住之后后续可以访问
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
	//在执行飞行动画时，先停止其他动画
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
	//代码迁移，封装
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
	//要让敌机在爆炸之后被删除
	//Sequence语句 ，序列动作会根据参数的先后顺序执行，注意参数的最后一个要为NULL，结束标志
	//NULL 老语法  nullptr 新语法
	//CallFuncN语句：
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
	this->stopAllActions();
	this->runAction(seq);
}

void Enemy::move() {
	this->setPositionY(this->getPositionY() + this->getSpeed());
}
/*
@return 减血后是否死亡
*/
//一次默认一次减滴血
bool Enemy::hit() {
	//是否已经死亡
	if (this->m_hp <= 0) return true;
	this->m_hp--;
	//此刻是否死亡
	if (this->m_hp <= 0) {
		//爆炸动画
		this->enemyBooming();
		return true;
	}
	else {
		//受伤动画
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