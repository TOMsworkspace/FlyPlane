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
	//通过加载精灵文件，将精灵缓存到精灵集缓存中
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

	//背景
	//auto bg = Sprite::create("shoot_background.png")
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");

	//bg->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
	//bg->setPosition(origin + size / 2);
	//设置定位点在自身的左下角
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//开启抗锯齿
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND, BACKGROUND1_TAG);

	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg2->setPositionY(bg1->getContentSize().height);
	bg2->getTexture()->setAliasTexParameters();
	addChild(bg2, BACKGROUND, BACKGROUND2_TAG);

	//飞机
	auto plane = Sprite::createWithSpriteFrameName("hero1.png");
	plane->setPosition(origin.x + VISIBLE_SIZE.width / 2, origin.y + plane->getContentSize().height / 2);
	addChild(plane, PLANE, PLANE_TAG);
	//给飞机添加飞行动画
	/*//创建动画
	//1.创建动画对象
	auto ani = Animation::create();
	//2.添加动画帧,从精灵帧缓存中根据名字查找出来的
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//3，设置动画时长
	ani->setDelayPerUnit(CREATE_BACKGROUND_INTERVAL);
	//4.设置循环次数
	ani->setLoops(CC_REPEAT_FOREVER);*/
	//从缓存中得到动画
	auto ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//将动画封装成动作
	auto animate = Animate::create(ani);

	//精灵运行动作
	plane->runAction(animate);

	//事件：发生，知道（注册监听），处理；
	//事件：键盘：敲击，触摸；鼠标；单击，双击，拖动；
	//事件发生传递，注册，操作系统，引擎已经做好

	//添加触摸事件的处理
	//1.创建一个事件监听对象
	auto listener = EventListenerTouchOneByOne::create();
	//2.分解事件按，处理逻辑

	//a.触摸开始时
	//lambda表达式的[]部分控制外部变量的访问，可以一个个的传递，在前边加&，表示按引用传递
	//也可以写一个=号，表示外部所有变量都按值传递进来，可以访问但不能修改
	//还可以写一个&号，表示所有变量传递进来，可以访问和修改值
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
	};//[做值的传递](//参数列表T) {};
	  //b.持续触摸并移动
	  //c.触摸结束
	listener->onTouchMoved = [=](Touch* t, Event* e) {
		Vec2 touchPos = t->getLocation();
		if (this->m_isOver) return;
		//Vec2 deltaPos = t->getDelta;//上一次触摸点与这一次触摸点之间的向量差
		plane->setPosition(touchPos + m_offset);
		//bullet->setPosition(plane->getPositionX() , plane->getPositionY() + plane->getContentSize().height / 2 + zidan->getContentSize().height / 2);
		//log("touch move...");

		auto minX = plane->getContentSize().width / 2;
		auto minY = plane->getContentSize().height / 2;
		auto maxX = VISIBLE_SIZE.width - minX;
		auto maxY = VISIBLE_SIZE.height - minY;
		auto x = MAX(minX, MIN(maxX, plane->getPositionX()));
		auto y = MAX(minY, MIN(maxY, plane->getPositionY()));//同时限制上下边界
		plane->setPosition(x, y);

	};
	listener->onTouchEnded = [](Touch* t, Event* e) {
		//log("touch end...");
	};
	//3.注册监听到分发器上
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, plane);
	//根据绘画的优先级得到事件的顺序,事件的接收与对象相关联

	//显示分值 UI  把整形转化成字符串 计分标签
	auto lb1Score = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_totalScore));
	lb1Score->setPosition(20, VISIBLE_SIZE.height - 15);

	this->addChild(lb1Score, UI, LABEL_SCORE_TAG);
	//上色
	lb1Score->setColor(Color3B::BLACK);
	lb1Score->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//上色

	//lb1Score->setAlignment(TextHAlignment::LEFT);



	//炸弹菜单
	//菜单：图片菜单，标签菜单
	//菜单项，菜单
	//炸弹数量
	auto bomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto itemBomb = MenuItemSprite::create(bomb, bomb, [this, lb1Score](Ref*) {
		//如果游戏处于暂停状态就不响应
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



	//炸弹数量显示
	auto lb2BombCount = Label::createWithBMFont("font.fnt", StringUtils::format("x%d", this->m_bombcount));
	lb2BombCount->setPosition(itemBomb->getPosition() + Vec2(bomb->getContentSize().width / 2 + 10, 0));

	//上色
	lb2BombCount->setColor(Color3B::BLACK);
	lb2BombCount->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(lb2BombCount, UI, BOMB_LABEL_TAG);




	//暂停菜单
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal, spPauseSelected);
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);

	auto toggle = MenuItemToggle::createWithCallback([listener, itemBomb](Ref* sender) {
		//获取当前选择项的下标
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
	//定位
	toggle->setPosition(VISIBLE_SIZE - toggle->getContentSize());

	auto menu = Menu::create();//必须的，菜单项要变成菜单必须
	menu->addChild(itemBomb, 1, ITEMBOMB_TAG);

	menu->addChild(toggle, 1, TOGGLE_PAUSE_TAG);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI, MENU_TAG);


	//初始化菜单和标签的显示
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
	//让背景2跟随背景1移动（取背景1的位置加上背景1的高度）
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
	//当背景2到达或者超出屏幕底部，则更新图1的位置Y到0
	if (bg2->getPositionY() <= 0)
	{
		bg1->setPositionY(0);
	}

	//临时容器
	Vector<Enemy*> removeableenemy;
	Vector<Sprite*> removeablebullet;
	Vector<Sprite*> removeableufo;

	for (auto bullet : m_bullets)
	{
		bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		//检查子弹是否超出屏幕上方
		if (bullet->getPositionY() >= VISIBLE_SIZE.height + bullet->getContentSize().height / 2) {
			//不能在遍历集合时修改集合，所以仅仅只是把它放到集合中，等遍历结束后，再从集合中移除
			//	this->m_bombs.eraseObject(bomb);
			this->removeChild(bullet);
			removeablebullet.pushBack(bullet);
		}
	}

	for (auto enemy : m_enemys)
	{
		enemy->move();
		//检查敌机是否超出屏幕下方
		if (enemy->getPositionY() <= -enemy->getContentSize().height / 2) {
			//不能在遍历集合时修改集合，所以仅仅只是把它放到集合中，等遍历结束后，再从集合中移除
			//	this->m_bombs.eraseObject(bomb);
			this->removeChild(enemy);
			removeableenemy.pushBack(enemy);
		}
	}

	for (auto ufo : m_ufoes)
	{
		//ufo->setPositionY(ufo->getPositionY() - UFO_SPEED);
		//检查飞机是否碰到ufo
		if (ufo && plane->getBoundingBox().intersectsRect(ufo->getBoundingBox())) {
			//不能在遍历集合时修改集合，所以仅仅只是把它放到集合中，等遍历结束后，再从集合中移除
			//	this->m_bombs.eraseObject(bomb);

			this->removeChild(ufo);
			removeableufo.pushBack(ufo);
			if (this->m_ufoType) {
				//双子弹
				AudioEngine::play2d("get_double_laser.mp3");
				this->m_doubleBulletCount += FULL_DOUBLE_BULLET_COUNT;
			}
			else {
				if (this->m_bombcount < 3) {
					//导弹
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
		//检查ufo是否超出屏幕下方
		/*if (ufo->getPositionY() <= - ufo->getContentSize().height / 2) {
		//不能在遍历集合时修改集合，所以仅仅只是把它放到集合中，等遍历结束后，再从集合中移除
		//	this->m_bombs.eraseObject(bomb);
		this->removeChild(ufo);
		removeableufo.pushBack(ufo);
		}*/
	}

	//碰撞检测
	//外循环敌机,内循环子弹
	for (auto enemy : m_enemys) {
		for (auto bullet : m_bullets) {
			if (enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox())) {
				//敌机爆炸
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
					//更新积分
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
				//要让敌机在爆炸之后被删除
				//Sequence语句 ，序列动作会根据参数的先后顺序执行，注意参数的最后一个要为NULL，结束标志
				//NULL 老语法  nullptr 新语法
				//CallFuncN语句：
				//方法一
				/*auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node) {
				//removeableemeny.pushBack(dynamic_cast<Sprite>);
				this->removeChild(node);
				}),NULL);*/
				//方法二
				/*auto seq = Sequence::create(animate, CallFuncN::create([](Node *node) {
				node->removeFromParentAndCleanup(true);
				}), NULL);*/
				//方法三
				/*auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
				//auto seq = Sequence::create(animate, removeChild(enemy), nullptr);
				enemy->runAction(seq);*/
				removeablebullet.pushBack(bullet);
				this->removeChild(bullet);
				//this->removeChild(enemy);
			}
		}
		//与飞机碰撞检测
		if (enemy->getBoundingBox().intersectsRect(plane->getBoundingBox())) {
			//1，敌机和飞机都爆炸
			//AudioEngine::play2d("game_over.mp3");
			this->gameover();
			//5.销毁敌机
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
			//方法三
			auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node) {
			//removeableemeny.pushBack(dynamic_cast<Sprite>);
			//this->removeChild(node);
			this->unscheduleAllCallbacks();
			//4.跳转场景
			auto scene = OverScene::createScene(this->m_totalScore);
			Director::getInstance()->replaceScene(scene);
			}), nullptr);
			//auto seq = Sequence::create(animate, removeChild(enemy), nullptr);
			plane->stopAllActions();
			plane->runAction(seq);
			//禁用暂停菜单

			auto menu = this->getChildByTag(MENU_TAG);
			auto toggle = menu->getChildByTag(TOGGLE_PAUSE_TAG);
			dynamic_cast<MenuItem*> (toggle)->setEnabled(false);*/
			//removeablebullet.pushBack(bullet);


			//	this->removeChild();
			//this->removeChild(enemy);
			//2.修改设置成员变量m_isover为true
			//3.停止所有定时器
			//this->unscheduleAllSelectors();
			//4.跳转场景
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
		//创建双子弹
		auto bulletL = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletL->setPosition(plane->getPosition() + Vec2(-plane->getContentSize().width / 3, plane->getContentSize().width / 3));
		addChild(bulletL, BULLET);
		m_bullets.pushBack(bulletL);

		auto bulletR = Sprite::createWithSpriteFrameName("bullet2.png");
		bulletR->setPosition(plane->getPosition() + Vec2(plane->getContentSize().width / 3, plane->getContentSize().width / 3));
		addChild(bulletR, BULLET);
		m_bullets.pushBack(bulletR);

		//子弹减少
		log("%d", this->m_doubleBulletCount);
		this->m_doubleBulletCount--;

	}
	else {
		//创建单发子弹
		auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
		bullet->setPosition(plane->getPosition() + Vec2(0, plane->getContentSize().height / 2 + bullet->getContentSize().height / 2));
		addChild(bullet, BULLET);
		m_bullets.pushBack(bullet);
	}
}

void GameScene::createenemy(const EnemyType& type) {
	//c++枚举，不是全局，要加类型限定

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
	//创建道具
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
	//道具动作
	auto move1 = MoveBy::create(2, Vec2(0, -200));
	auto move2 = MoveTo::create(1.0f, Vec2(ufo->getPositionX(), -ufo->getContentSize().height));

	ufo->runAction(Sequence::create(move1, move1->reverse(), move2, RemoveSelf::create(), NULL));
}

//产生敌机阵函数
void GameScene::enemyunit(float) {
	this->createenemyunit(Vec2 (30,0)); 
	this->createenemyunit(Vec2(200, 0));
	this->createenemyunit(Vec2(370, 0));
}
void GameScene::createenemyunit(Vec2 vec) {
	//临时敌机阵容器
	Vector<Enemy*> m_enemyunit;
	//产生敌机并装入容器
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

	//加入敌机集合，产生敌机阵
	for (auto enemy : m_enemyunit) {
		enemy->setPosition(enemy->getPosition() + vec);
		this->addChild(enemy);
		m_enemys.pushBack(enemy);
	}

	//清除临时集合
	m_enemyunit.clear();
}

//当炸弹数为零时，菜单项和标签都不显示
//当炸弹数为1时，只显示菜单项
//当炸弹数大于1时，显示菜单项和标签，且更新标签显示内容
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
	//1，敌机和飞机都爆炸,设置成员变量is_Over为1
	this->m_isOver = true;
	//6.动作还在跑
	for (auto node : this->getChildren()) {
		node->stopAllActions();
	}
	//2.执行爆炸动画

	auto ani = AnimationCache::getInstance()->getAnimation(HERO_DOWN_ANIMATION);
	auto seq = Sequence::create(Animate::create(ani), CallFuncN::create([this](Node*node) {
		//4。跳转场景
		auto scene = OverScene::createScene(this->m_totalScore);
		Director::getInstance()->replaceScene(scene);
	}), nullptr);
	plane->runAction(seq);
	//3.停止所有定时器
	this->unscheduleAllCallbacks();
	//8.停止暂停菜单
	auto menu = this->getChildByTag(MENU_TAG);
	auto toggle = menu->getChildByTag(TOGGLE_PAUSE_TAG);
	dynamic_cast<MenuItem*> (toggle)->setEnabled(false);

}