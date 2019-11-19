#include"OverScene.h"
#include"constants.h"
#include"GameScene.h"
#include"AudioEngine.h"

using namespace experimental;

Scene* OverScene::createScene(int Score) {
	auto scene = new OverScene();
	if (scene&&scene->init(Score)) {
		scene->autorelease();
		return scene;
	}
	delete scene;
	scene = nullptr;
	return nullptr;
	//return OverScene::create();
}

bool OverScene::init(int score) {
	do
	{
		CC_BREAK_IF(!Scene::init());
		AudioEngine::play2d("game_over.mp3");
		auto bg = Sprite::createWithSpriteFrameName("gameover.png");
		bg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		bg->setPosition(0, VISIBLE_ORIGIN.y + VISIBLE_SIZE.height);
		this->addChild(bg, BACKGROUND);

		auto highScore = UserDefault::getInstance()->getIntegerForKey(HIGH_SCORE_KEY, 0);
		//历史最高分

		auto lbHighScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", highScore));
		lbHighScore->setPosition(VISIBLE_ORIGIN + Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 5 * 4));
		this->addChild(lbHighScore, UI);
		lbHighScore->setColor(Color3B::BLACK);
		if (highScore < score) {

			highScore = score;
			UserDefault::getInstance()->setIntegerForKey(HIGH_SCORE_KEY, highScore);
			auto seq = Sequence::create(
				ScaleTo::create(1, 5),
				CallFuncN::create([highScore](Node* node) {
				dynamic_cast<Label*>(node)->setString(StringUtils::format("%d", highScore));
			}),
				ScaleTo::create(1, 1),
				nullptr);
			lbHighScore->runAction(seq);
		}
		//当前成绩
		AudioEngine::play2d("achieventment.mp3");
		auto lbScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", score));
		lbScore->setPosition(VISIBLE_ORIGIN + Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 3));
		this->addChild(lbScore, UI);
		lbScore->setColor(Color3B::BLACK);
		lbScore->setBMFontSize(68);

		//回到游戏
		auto itemRstart = Sprite::createWithSpriteFrameName("btn_finish.png");
		auto itemRestart = MenuItemSprite::create(itemRstart, itemRstart, [](Ref*) {
			AudioEngine::play2d("button.mp3");
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(scene);
		});
		itemRestart->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		itemRestart->setPosition(VISIBLE_SIZE.width - 20, 20);

		auto menu = Menu::create(itemRestart, nullptr);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1, MENU_TAG);

		return true;
	} while (0);
	return false;
}