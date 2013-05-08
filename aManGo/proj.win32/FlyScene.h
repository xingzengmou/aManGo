#ifndef __FLY_SCENE_H__
#define __FLY_SCENE_H__

#include "cocos2d.h"
#include <vector>

class FlyScene : public cocos2d::CCLayer {
public:
	virtual bool init();

	static cocos2d::CCScene *scene();

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	CREATE_FUNC(FlyScene);

private:
	void menuReturnCallback(cocos2d::CCObject *pSender);
	void createBullet(float f); //(cocos2d::CCTime dt);
	void flying(float f);//(cocos2d::CCTime dt);
	void checkBullet(float f);//(cocos2d::CCTime dt);
	void saveTime(float f);//(cocos2d::CCTime dt);
	void explosionEndDid();
	bool triangleContainPoint(float x1,float y1,float x2,float y2,float x3,float y3,float px,float py);
	float heronsformula(float x1,float y1,float x2,float y2,float x3,float y3);

	//relative joystick
	cocos2d::CCSprite *joystick;
	cocos2d::CCPoint center;
	float radius;

	// relative  plane
	cocos2d::CCSprite *plane;
	float speedX;
	float speedY;
	bool isFlying;

	//relative bullet
	std::vector <cocos2d::CCSprite*> bullets;

	//relative explosion
	cocos2d::CCSprite *explosion;

	int gGameTime;
	int gBulletNum;
};

#endif

