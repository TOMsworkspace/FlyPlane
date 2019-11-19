#include"GameScene.h"
#include"OverScene.h"
#include"constants.h"
#include"time.h"
#include"AudioEngine.h"

using namespace experimental;

Scene* GameScene::createScene() {
	return GameScene::create();
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	srand((unsigned int)time(NULL));

	AudioEngine::play2d("game_music.mp3", true, 0.4f);
	//auto size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//ͨ�����ؾ����ļ��������黺�浽���鼯������
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

	//����
	//auto bg = Sprite::create("shoot_background.png")
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");

	//bg->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
	//bg->setPosition(origin + size / 2);
	//���ö�λ������������½�
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//���������
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND, BACKGROUND1_TAG);

	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg2->setPositionY(bg1->getContentSize().height);
	bg2->getTexture()->setAliasTexParameters();
	addChild(bg2, BACKGROUND, BACKGROUND2_TAG);

	//�ɻ�
	auto plane = Sprite::createWithSpriteFrameName("hero1.png");
	plane->setPosition(origin.x + VISIBLE_SIZE.width / 2, origin.y + plane->getContentSize().height / 2);
	addChild(plane, PLANE, PLANE_TAG);
	//���ɻ���ӷ��ж���
	/*//��������
	//1.������������
	auto ani = Animation::create();
	//2.��Ӷ���֡,�Ӿ���֡�����и������ֲ��ҳ�����
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//3�����ö���ʱ��
	ani->setDelayPerUnit(CREATE_BACKGROUND_INTERVAL);
	//4.����ѭ������
	ani->setLoops(CC_REPEAT_FOREVER);*/
	//�ӻ����еõ�����
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//��������װ�ɶ���
	auto animate = Animate::create(ani);

	//�������ж���
	plane->runAction(animate);

	//�¼���������֪����ע�������������
	//�¼������̣��û�����������ꣻ������˫�����϶���
	//�¼��������ݣ�ע�ᣬ����ϵͳ�������Ѿ�����

	//��Ӵ����¼��Ĵ���
	//1.����һ���¼���������
	auto listener = EventListenerTouchOneByOne::create();
	//2.�ֽ��¼����������߼�

	//a.������ʼʱ
	//lambda���ʽ��[]���ֿ����ⲿ�����ķ��ʣ�����һ�����Ĵ��ݣ���ǰ�߼�&����ʾ�����ô���
	//Ҳ����дһ��=�ţ���ʾ�ⲿ���б�������ֵ���ݽ��������Է��ʵ������޸�
	//������дһ��&�ţ���ʾ���б������ݽ��������Է��ʺ��޸�ֵ
	listener->onTouchBegan = [=](Touch* t, Event* e) {
		//log("touch begin...");
		Vec2 touchPos = t->getLocation();
		//log("touch position is x: %f,y: %f", touchPos.x, touchPos.y);

		this->m_offset = plane->getPosition() - touchPos;

		//plane->setPosition(t->getLocation().x, t->getLocation().y);
		//auto move = MoveTo::create(0.5f,touchPos);
		//plane->runAction(move);

		bool isContains = plane->getBoundingBox().containsPoint(touchPos);
		return isContains && !Director::getInstance()->isPaused() && !this->m_isOver;
		//return true;
	};//[��ֵ�Ĵ���](//�����б�T) {};
	  //b.�����������ƶ�
	  //c.��������
	listener->onTouchMoved = [=](Touch* t, Event* e) {
		Vec2 touchPos = t->getLocation();
		if (this->m_isOver) return;
		//Vec2 deltaPos = t->getDelta;//��һ�δ���������һ�δ�����֮���������
		plane->setPosition(touchPos + m_offset);
		//bullet->setPosition(plane->getPositionX() , plane->getPositionY() + plane->getContentSize().height / 2 + zidan->getContentSize().height / 2);
		//log("touch move...");

		auto minX = plane->getContentSize().width / 2;
		auto minY = plane->getContentSize().height / 2;
		auto maxX = VISIBLE_SIZE.width - minX;
		auto maxY = VISIBLE_SIZE.height - minY;
		auto x = MAX(minX, MIN(maxX, plane->getPositionX()));
		auto y = MAX(minY, MIN(maxY, plane->getPositionY()));//ͬʱ�������±߽�
		plane->setPosition(x, y);

	};
	listener->onTouchEnded = [](Touch* t, Event* e) {
		//log("touch end...");
	};
	//3.ע��������ַ�����
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, plane);
	//���ݻ滭�����ȼ��õ��¼���˳��,�¼��Ľ�������������

	//��ʾ��ֵ UI  ������ת�����ַ��� �Ʒֱ�ǩ
	auto lb1Score = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_totalScore));
	lb1Score->setPosition(20, VISIBLE_SIZE.height - 15);

	this->addChild(lb1Score, UI, LABEL_SCORE_TAG);
	//��ɫ
	lb1Score->setColor(Color3B::BLACK);
	lb1Score->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//��ɫ

	//lb1Score->setAlignment(TextHAlignment::LEFT);



	//ը���˵�
	//�˵���ͼƬ�˵�����ǩ�˵�
	//�˵���˵�
	//ը������
	auto bomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto itemBomb = MenuItemSprite::create(bomb, bomb, [this, lb1Score](Ref*) {
		//�����Ϸ������ͣ״̬�Ͳ���Ӧ
		AudioEngine::play2d("button.mp3");
		if (Director::getInstance()->isPaused() || this->m_isOver) return;
		if (this->m_bombcount <= 0) {

			return;
		}
		for (auto enemy : m_enemys) {
			enemy->enemyBooming();
			this->m_totalScore += enemy->getScore();
		}
		lb1Score->setString(StringUtils::format("%d", this->m_totalScore));
		this->m_enemys.clear();
		this->m_bombcount--;
		this->changebombcount();
		AudioEngine::play2d("use_bomb.mp3");
	});
	itemBomb->setPosition(itemBomb->getContentSize());



	//ը��������ʾ
	auto lb2BombCount = Label::createWithBMFont("font.fnt", StringUtils::format("x%d", this->m_bombcount));
	lb2BombCount->setPosition(itemBomb->getPosition() + Vec2(bomb->getContentSize().width / 2 + 10, 0));

	//��ɫ
	lb2BombCount->setColor(Color3B::BLACK);
	lb2BombCount->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(lb2BombCount, UI, BOMB_LABEL_TAG);




	//��ͣ�˵�
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal, spPauseSelected);
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);

	auto toggle = MenuItemToggle::createWithCallback([listener, itemBomb](Ref* sender) {
		//��ȡ��ǰѡ������±�
		AudioEngine::play2d("button.mp3");
		int index = dynamic_cast<MenuItemToggle*> (sender)->getSelectedIndex();
		if (index) {
			//log("Pause");
			Director::getInstance()->pause();
		}
		else {
			//log("Resume");
			Director::getInstance()->resume();
		}
	}, itemPause, itemResume, nullptr);
	//��λ
	toggle->setPosition(VISIBLE_SIZE - toggle->getContentSize());

	auto menu = Menu::create();//����ģ��˵���Ҫ��ɲ˵�����
	menu->addChild(itemBomb, 1, ITEMBOMB_TAG);

	menu->addChild(toggle, 1, TOGGLE_PAUSE_TAG);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI, MENU_TAG);


	//��ʼ���˵��ͱ�ǩ����ʾ
	this->changebombcount();

	scheduleUpdate();
	schedule(schedule_selector(GameScene::createBullet), CREATE_BULLET_INTERVAL);
	schedule(schedule_selector(GameScene::createSmallEnemy), CREATE_SMALL_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_SMALL_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createMiddleEnemy), CREATE_MIDDLE_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_MIDDLE_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createBigEnemy), CREATE_BIG_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_BIG_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createUfo), CREATE_UFO_INTERVAL, CC_REPEAT_FOREVER, CREATE_UFO_DELAY);
	schedule(schedule_selector(GameScene::enemyunit), CREATE_ENEMYUNIT_INTERVAL, CC_REPEAT_FOREVER, CREATE_ENEMYUNIT_DELAY);
	return true;

}

void GameScene::update(float delta) {
	auto bg1 = this->getChildByTag(BACKGROUND1_TAG);
	auto bg2 = getChildByTag(BACKGROUND2_TAG);
	auto plane = getChildByTag(PLANE_TAG);


	bg1->setPositionY(bg1->getPositionY() - BACKGROUND_SPEED);
	//�ñ���2���汳��1�ƶ���ȡ����1��λ�ü��ϱ���1�ĸ߶ȣ�
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
	//������2������߳�����Ļ�ײ��������ͼ1��λ��Y��0
	if (bg2->getPositionY() <= 0)
	{
		bg1->setPositionY(0);
	}

	//��ʱ����
	Vector<Enemy*> removeableenemy;
	Vector<Sprite*> removeablebullet;
	Vector<Sprite*> removeableufo;

	for (auto bullet : m_bullets)
	{
		bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		//����ӵ��Ƿ񳬳���Ļ�Ϸ�
		if (bullet->getPositionY() >= VISIBLE_SIZE.height + bullet->getContentSize().height / 2) {
			//�����ڱ�������ʱ�޸ļ��ϣ����Խ���ֻ�ǰ����ŵ������У��ȱ����������ٴӼ������Ƴ�
			//	this->m_bombs.eraseObject(bomb);
			this->removeChild(bullet);
			removeablebullet.pushBack(bullet);
		}
	}

	for (auto enemy : m_enemys)
	{
		enemy->move();
		//���л��Ƿ񳬳���Ļ�·�
		if (enemy->getPositionY() <= -enemy->getContentSize().height / 2) {
			//�����ڱ�������ʱ�޸ļ��ϣ����Խ���ֻ�ǰ����ŵ������У��ȱ����������ٴӼ������Ƴ�
			//	this->m_bombs.eraseObject(bomb);
			this->removeChild(enemy);
			removeableenemy.pushBack(enemy);
		}
	}

	for (auto ufo : m_ufoes)
	{
		//ufo->setPositionY(ufo->getPositionY() - UFO_SPEED);
		//���ɻ��Ƿ�����ufo
		if (ufo && plane->getBoundingBox().intersectsRect(ufo->getBoundingBox())) {
			//�����ڱ�������ʱ�޸ļ��ϣ����Խ���ֻ�ǰ����ŵ������У��ȱ����������ٴӼ������Ƴ�
			//	this->m_bombs.eraseObject(bomb);

			this->removeChild(ufo);
			removeableufo.pushBack(ufo);
			if (this->m_ufoType) {
				//˫�ӵ�
				AudioEngine::play2d("get_double_laser.mp3");
				this->m_doubleBulletCount += FULL_DOUBLE_BULLET_COUNT;
			}
			else {
				if (this->m_bombcount < 3) {
					//����
					AudioEngine::play2d("get_bomb.mp3");
					this->m_bombcount++;
					this->changebombcount();
					//this->(this->m_enemys);
					/*auto nodeCount = getChildByTag(BOMB_COUNT_TAG);
					auto lb1Score = dynamic_cast<Label *>(nodeCount);
					std::string strCount = StringUtils::format(" X %d", m_BombCount);
					lb1Score->setString(strCount);*/
				}
			}

		}
		//���ufo�Ƿ񳬳���Ļ�·�
		/*if (ufo->getPositionY() <= - ufo->getContentSize().height / 2) {
		//�����ڱ�������ʱ�޸ļ��ϣ����Խ���ֻ�ǰ����ŵ������У��ȱ����������ٴӼ������Ƴ�
		//	this->m_bombs.eraseObject(bomb);
		this->removeChild(ufo);
		removeableufo.pushBack(ufo);
		}*/
	}

	//��ײ���
	//��ѭ���л�,��ѭ���ӵ�
	for (auto enemy : m_enemys) {
		for (auto bullet : m_bullets) {
			if (enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox())) {
				//�л���ը
				if (enemy->hit()) {
					if (enemy->getHp() == 0 && enemy->getType() == EnemyType::BIG_ENEMY) {
						auto xy = enemy->getPosition();
						auto size = enemy->getContentSize();
						for (int i = 0; i < 3; i++) {
							auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
							enemy->setPosition(xy.x-size.width/2+size.width/2*i,xy.y);
							//m_enemys.pushBack(enemy);
						}
					}
					removeableenemy.pushBack(enemy);
					//���»���
					//log("Score:%d", m_totalScore);
					m_totalScore += enemy->getScore();

					auto nodeScore = getChildByTag(LABEL_SCORE_TAG);
					auto lb1Score = dynamic_cast<Label *>(nodeScore);
					std::string strScore = StringUtils::format("%d", m_totalScore);
					lb1Score->setString(strScore);

				}
				/*auto ani = Animation::create();
				for (auto i = 0; i < 4; i++) {
				auto frameName = StringUtils::format("enemy1_down%d.png",i+1);
				ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
				}
				ani->setDelayPerUnit(0.2f);
				auto animate = Animate::create(ani);
				//Ҫ�õл��ڱ�ը֮��ɾ��
				//Sequence��� �����ж�������ݲ������Ⱥ�˳��ִ�У�ע����������һ��ҪΪNULL��������־
				//NULL ���﷨  nullptr ���﷨
				//CallFuncN��䣺
				//����һ
				/*auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node) {
				//removeableemeny.pushBack(dynamic_cast<Sprite>);
				this->removeChild(node);
				}),NULL);*/
				//������
				/*auto seq = Sequence::create(animate, CallFuncN::create([](Node *node) {
				node->removeFromParentAndCleanup(true);
				}), NULL);*/
				//������
				/*auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
				//auto seq = Sequence::create(animate, removeChild(enemy), nullptr);
				enemy->runAction(seq);*/
				removeablebullet.pushBack(bullet);
				this->removeChild(bullet);
				//this->removeChild(enemy);
			}
		}
		//��ɻ���ײ���
		if (enemy->getBoundingBox().intersectsRect(plane->getBoundingBox())) {
			//1���л��ͷɻ�����ը
			//AudioEngine::play2d("game_over.mp3");
			this->gameover();
			//5.���ٵл�
			removeableenemy.pushBack(enemy);
			enemy->enemyBooming();
			/*auto ani = Animation::create();
			for (auto i = 0; i < 4; i++) {
			auto frameName = StringUtils::format("hero_blowup_n%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
			}
			ani->setDelayPerUnit(0.2f);
			auto animate = Animate::create(ani);
			this->m_isOver = true;
			//������
			auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node) {
			//removeableemeny.pushBack(dynamic_cast<Sprite>);
			//this->removeChild(node);
			this->unscheduleAllCallbacks();
			//4.��ת����
			auto scene = OverScene::createScene(this->m_totalScore);
			Director::getInstance()->replaceScene(scene);
			}), nullptr);
			//auto seq = Sequence::create(animate, removeChild(enemy), nullptr);
			plane->stopAllActions();
			plane->runAction(seq);
			//������ͣ�˵�

			auto menu = this->getChildByTag(MENU_TAG);
			auto toggle = menu->getChildByTag(TOGGLE_PAUSE_TAG);
			dynamic_cast<MenuItem*> (toggle)->setEnabled(false);*/
			//removeablebullet.pushBack(bullet);


			//	this->removeChild();
			//this->removeChild(enemy);
			//2.�޸����ó�Ա����m_isoverΪtrue
			//3.ֹͣ���ж�ʱ��
			//this->unscheduleAllSelectors();
			//4.��ת����
			//auto scene = OverScene::createScene(this->m_totalScore);
			//Director::getInstance()->replaceScene(scene);
		}
	}

	for (auto bullet : removeablebullet) {
		m_bullets.eraseObject(bullet);
	}

	removeablebullet.clear();

	for (auto enemy : removeableenemy) {
		m_enemys.eraseObject(enemy);
	}
	removeableenemy.clear();

	for (auto ufo : removeableufo) {
		m_ufoes.eraseObject(ufo);
	}
	removeableufo.clear();
}

void GameScene::createBullet(float t) {
	AudioEngine::play2d("bullet");
	auto plane = getChildByTag(PLANE_TAG);
	if (m_doubleBulletCount > 0) {
		//����˫�ӵ�
		auto bulletL = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletL->setPosition(plane->getPosition() + Vec2(-plane->getContentSize().width / 3, plane->getContentSize().width / 3));
		addChild(bulletL, BULLET);
		m_bullets.pushBack(bulletL);

		auto bulletR = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletR->setPosition(plane->getPosition() + Vec2(plane->getContentSize().width / 3, plane->getContentSize().width / 3));
		addChild(bulletR, BULLET);
		m_bullets.pushBack(bulletR);

		//�ӵ�����
		log("%d", this->m_doubleBulletCount);
		this->m_doubleBulletCount--;

	}
	else {
		//���������ӵ�
		auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
		bullet->setPosition(plane->getPosition() + Vec2(0, plane->getContentSize().height / 2 + bullet->getContentSize().height / 2));
		addChild(bullet, BULLET);
		m_bullets.pushBack(bullet);
	}
}

void GameScene::createenemy(const EnemyType& type) {
	//c++ö�٣�����ȫ�֣�Ҫ�������޶�

	auto enemy = Enemy::create(type);
	float minX = enemy->getContentSize().width / 2;
	float maxX = VISIBLE_SIZE.width - minX;
	float x = rand() % (int)(maxX - minX + 1) + minX;
	//log("x:%f", x);
	enemy->setPosition(x, VISIBLE_SIZE.height + enemy->getContentSize().height / 2);
	addChild(enemy, EMENY);
	m_enemys.pushBack(enemy);
}

void GameScene::createSmallEnemy(float) {
	this->createenemy(EnemyType::SMALL_ENEMY);
}

void GameScene::createMiddleEnemy(float) {
	this->createenemy(EnemyType::MIDDLE_ENEMY);
}

void GameScene::createBigEnemy(float) {
	this->createenemy(EnemyType::BIG_ENEMY);
}

void GameScene::createUfo(float) {
	AudioEngine::play2d("out_porp.mp3");
	//��������
	this->m_ufoType = rand() % 2;
	auto frameName = this->m_ufoType ? "ufo1.png" : "ufo2.png";
	//auto frameName = StringUtils::format("ufo%d.png", rand() % 2 + 1);
	auto ufo = Sprite::createWithSpriteFrameName(frameName);
	float minX = ufo->getContentSize().width / 2;
	float maxX = VISIBLE_SIZE.width - minX;
	float x = rand() % (int)(maxX - minX + 1) + minX;
	ufo->setPosition(x, VISIBLE_SIZE.height + ufo->getContentSize().height / 2);
	addChild(ufo, UFO, UFO_TAG);
	m_ufoes.pushBack(ufo);
	//���߶���
	auto move1 = MoveBy::create(2, Vec2(0, -200));
	auto move2 = MoveTo::create(1.0f, Vec2(ufo->getPositionX(), -ufo->getContentSize().height));

	ufo->runAction(Sequence::create(move1, move1->reverse(), move2, RemoveSelf::create(), NULL));
}

//�����л�����
void GameScene::enemyunit(float) {
	this->createenemyunit(Vec2 (30,0)); 
	this->createenemyunit(Vec2(200, 0));
	this->createenemyunit(Vec2(370, 0));
}
void GameScene::createenemyunit(Vec2 vec) {
	//��ʱ�л�������
	Vector<Enemy*> m_enemyunit;
	//�����л���װ������
	auto xy = Vec2(0, 0);
	for (int i = 0; i <= 2; i++) {
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		//enemy->setAnchorPoint(enemy->getContentSize().width*i,VISIBLE_SIZE.height);

		enemy->setPosition(enemy->getContentSize().width*i , VISIBLE_SIZE.height - enemy->getContentSize().height);
		xy = enemy->getPosition();
		m_enemyunit.pushBack(enemy);
	}
	for (int i = 0; i <= 1; i++) {
		auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);
		//enemy->setAnchorPoint(enemy->getContentSize().width*i,VISIBLE_SIZE.height);
		enemy->setPosition(xy.x - enemy->getContentSize().width / 2 - i * enemy->getContentSize().width, xy.y + enemy->getContentSize().height);
		//xy = enemy->getPosition();
		m_enemyunit.pushBack(enemy);
	}
	auto enemy = Enemy::create(EnemyType::SMALL_ENEMY);

	enemy->setPosition(xy + Vec2(-enemy->getContentSize().width , 2*enemy->getContentSize().height));
	xy = enemy->getPosition();
	m_enemyunit.pushBack(enemy);

	//����л����ϣ������л���
	for (auto enemy : m_enemyunit) {
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemys.pushBack(enemy);
	}

	//�����ʱ����
	m_enemyunit.clear();
}

//��ը����Ϊ��ʱ���˵���ͱ�ǩ������ʾ
//��ը����Ϊ1ʱ��ֻ��ʾ�˵���
//��ը��������1ʱ����ʾ�˵���ͱ�ǩ���Ҹ��±�ǩ��ʾ����
void GameScene::changebombcount() {
	auto menu = this->getChildByTag(MENU_TAG);
	auto itemBomb = menu->getChildByTag(ITEMBOMB_TAG);
	auto lbbomb = this->getChildByTag(BOMB_LABEL_TAG);

	if (this->m_bombcount <= 0) {
		itemBomb->setVisible(false);
		lbbomb->setVisible(false);
	}
	else if (this->m_bombcount == 1) {
		itemBomb->setVisible(true);
		lbbomb->setVisible(false);
	}
	else {
		itemBomb->setVisible(true);
		lbbomb->setVisible(true);
		dynamic_cast<Label*>(lbbomb)->setString(StringUtils::format("X%d", this->m_bombcount));
	}

}

void GameScene::gameover() {
	auto plane = this->getChildByTag(PLANE_TAG);
	//1���л��ͷɻ�����ը,���ó�Ա����is_OverΪ1
	this->m_isOver = true;
	//6.����������
	for (auto node : this->getChildren()) {
		node->stopAllActions();
	}
	//2.ִ�б�ը����

	auto ani = AnimationCache::getInstance()->getAnimation(HERO_DOWN_ANIMATION);
	auto seq = Sequence::create(Animate::create(ani), CallFuncN::create([this](Node*node) {
		//4����ת����
		auto scene = OverScene::createScene(this->m_totalScore);
		Director::getInstance()->replaceScene(scene);
	}), nullptr);
	plane->runAction(seq);
	//3.ֹͣ���ж�ʱ��
	this->unscheduleAllCallbacks();
	//8.ֹͣ��ͣ�˵�
	auto menu = this->getChildByTag(MENU_TAG);
	auto toggle = menu->getChildByTag(TOGGLE_PAUSE_TAG);
	dynamic_cast<MenuItem*> (toggle)->setEnabled(false);

}