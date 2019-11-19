#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_  //只包含一次


//优先级
#define BACKGROUND -1
#define PLANE 1
#define BULLET 1
#define EMENY 1
#define UFO 1
#define UI  3
#define BOMB 4
//Tag
#define BACKGROUND1_TAG 1
#define BACKGROUND2_TAG 2
#define PLANE_TAG 3
#define BULLET_TAG 4
#define EMENY_TAG 5
#define UFO_TAG 6
#define LABEL_SCORE_TAG 10
#define BOMB_LABEL_TAG  11
#define ITEMBOMB_TAG 12
#define TOGGLE_PAUSE_TAG 13
#define MENU_TAG 14

//sizes
#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define VISIBLE_ORIGIN Director::getInstance()->getVisibleOrigin()
//速度
#define BACKGROUND_SPEED 2
#define BULLET_SPEED 9
#define SMALL_ENEMY_SPEED 4.5f
#define MIDDLE_ENEMY_SPEED 1.2f
#define BIG_ENEMY_SPEED 0.5f
#define UFO_SPEED 3

//定时器相关
#define CREATE_BULLET_INTERVAL 0.1f
#define CREATE_BACKGROUND_INTERVAL 0.2f
#define CREATE_SMALL_ENEMY_INTERVAL 5.5f
#define CREATE_MIDDLE_ENEMY_INTERVAL 10.0f
#define CREATE_BIG_ENEMY_INTERVAL 25.5f
#define CREATE_UFO_INTERVAL  5
#define CREATE_ENEMYUNIT_INTERVAL 5

#define CREATE_SMALL_ENEMY_DELAY 3
#define CREATE_MIDDLE_ENEMY_DELAY 10
#define CREATE_BIG_ENEMY_DELAY 30
#define CREATE_UFO_DELAY 7.5f
#define CREATE_ENEMYUNIT_DELAY 5

#define FULL_DOUBLE_BULLET_COUNT 60 //代表满了的双子弹的次数
//动画缓存键
#define HERO_FLY_ANIMATION "HEROFLY"
#define HERO_DOWN_ANIMATION "HERODOWN"

#define HIGH_SCORE_KEY "HIGHSCORE"
#endif // !_CONSTANTS_H_
