#include"LoadScene.h"
#include"constants.h"
#include"GameScene.h"
#include"AudioEngine.h"

using namespace experimental;

Scene* LoadScene::createScene() {
	return LoadScene::create();
}

bool LoadScene::init()
{
	//先调用父类的初始化函数
	do
	{
		CC_BREAK_IF(!Scene::init());
		//加载精灵集文件，将精灵都缓存到精灵集缓存中
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

		//页面内容
		auto bg = Sprite::createWithSpriteFrameName("background.png");
		bg->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2);
		this->addChild(bg);
		auto title = Sprite::createWithSpriteFrameName("shoot_copyright.png");
		title->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2 + Vec2(0, 100));
		this->addChild(title);

		auto loading = Sprite::create();
		loading->setPosition(VISIBLE_ORIGIN + VISIBLE_SIZE / 2 + Vec2(0, -200));
		this->addChild(loading);
		auto ani = Animation::create();
		for (auto i = 0; i < 4; i++)
		{
			auto png = StringUtils::format("game_loading%d.png", i + 1);
			ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(png));
		}
		ani->setDelayPerUnit(0.2f);
		ani->setLoops(2);
		auto seq = Sequence::create(Animate::create(ani), CallFunc::create([]() {
			//场景跳转
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionSplitRows::create(1, scene));
		}), nullptr);
		loading->runAction(seq);

		//创建动画并放入缓存
		//创建动画对象
		auto aniHeroFly = Animation::create();
		//添加动画帧(从精灵帧缓存中根据名字查找出来的)
		aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
		aniHeroFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
		//设置动画切换时长
		aniHeroFly->setDelayPerUnit(0.2f);
		//设置循环次数
		aniHeroFly->setLoops(CC_REPEAT_FOREVER);//无限循环
												//放入动画缓存
		AnimationCache::getInstance()->addAnimation(aniHeroFly, HERO_FLY_ANIMATION);

		//爆炸动画
		Vector<SpriteFrame* > frames;
		for (auto i = 0; i < 4; i++) {
			auto png = StringUtils::format("hero_blowup_n%d.png", i + 1);
			frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(png));

		}
		auto aniHeroDown = Animation::createWithSpriteFrames(frames, 0.2f);
		AnimationCache::getInstance()->addAnimation(aniHeroDown, HERO_DOWN_ANIMATION);

		//预加载声音文件
		AudioEngine::preload("achievement.mp3");
		AudioEngine::preload("big_spaceship_flying.mp3");
		AudioEngine::preload("bullet.mp3");
		AudioEngine::preload("botton.mp3");
		AudioEngine::preload("enemy1_down.mp3");
		AudioEngine::preload("enemy2_down.mp3");
		AudioEngine::preload("enemy3_down.mp3");
		AudioEngine::preload("game_music.mp3");
		AudioEngine::preload("game_over.mp3");
		AudioEngine::preload("get_bomb.mp3");
		AudioEngine::preload("get_double_laser.mp3");
		AudioEngine::preload("out_porp.mp3");
		AudioEngine::preload("use_bomb.mp3");
		return true;

	} while (0);
	return false;
}

//创建LoadScene类及成员函数