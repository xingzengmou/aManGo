#include "FlyScene.h"
#include "TitleScene.h"

using namespace cocos2d;

bool FlyScene::init() {
	bool bRet = false;

	do {
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		CCSprite *joystick1 = CCSprite::create("joystick1.png");
		CC_BREAK_IF(!joystick1);
		joystick1->setOpacity(191);
		joystick1->setAnchorPoint(ccp(0,0));
		joystick1->setPosition(ccp(0, 0));
		radius = joystick1->getContentSize().width/2;
		center = ccp(radius, radius);
		this->addChild(joystick1, 1);

		joystick = CCSprite::create("joystick2.png");
		CC_BREAK_IF(!joystick);
		joystick->setPosition(ccp(center.x, center.y));
		this->addChild(joystick, 2);

		//back
		CCMenuItemFont::setFontSize(22);
		CCMenuItemFont::setFontName("American Typewriter");
		std::string str = "back";

		CCMenuItemFont *systemMenu = CCMenuItemFont::create(str.c_str(), this, menu_selector(FlyScene::menuReturnCallback));
		CCMenu *menu = CCMenu::create(systemMenu, NULL);
		menu->setPosition(CCPointZero);

		systemMenu->setAnchorPoint(ccp(1, 0));
		systemMenu->setPosition(ccp(s.width, 0));

		this->addChild(menu, 1);

		//CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("plane.png");
		plane = CCSprite::create("plane.png");
		plane->setPosition(ccp(s.width/2, plane->getContentSize().height));
		this->addChild(plane, 0);

		this->setTouchEnabled(true);

		this->schedule(schedule_selector(FlyScene::flying));
		this->schedule(schedule_selector(FlyScene::createBullet));
		this->schedule(schedule_selector(FlyScene::checkBullet));
		this->schedule(schedule_selector(FlyScene::saveTime));

		//clear data
		this->isFlying = false;
		this->speedX = this->speedY = 0;
		this->gGameTime = 0;
		this->gBulletNum = 4000;

	} while (0);

	return true;
}

CCScene *FlyScene::scene() {
	CCScene *scene = NULL;
	do {
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		FlyScene *layer = FlyScene::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);

	return scene;
}

void FlyScene::menuReturnCallback(CCObject *pSender) {
	CCScene *scene = TitleScene::scene();
	CCTransitionFlipX *m = CCTransitionFlipX::create(1.2f, scene);
	CCDirector::sharedDirector()->replaceScene(m);
}

void FlyScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = touch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	CCRect rect = joystick->boundingBox();
	if (rect.containsPoint(convertedLocation)) {
		this->isFlying = true;
	}
}

void FlyScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = touch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
	bool inRange = pow(center.x - convertedLocation.x, 2) +
		pow(center.y - convertedLocation.y, 2) < 
		pow(radius, 2);

	if (this->isFlying && inRange) {
		CCPoint position = plane->getPosition();
		joystick->setPosition(convertedLocation);
		float radius1 = radius * 2 /6;
		float side = radius * 2 / 3;

		//向上
		if(triangleContainPoint(center.x,center.y,center.x-radius1,center.y+radius1,center.x+radius1,center.y+radius1,convertedLocation.x,convertedLocation.y)
			|| CCRectMake(center.x-radius1,center.y+radius1,side,side).containsPoint(convertedLocation)) {
			speedX=0;
			speedY=1;
		}
		//向下
		else if(triangleContainPoint(center.x,center.y,center.x-radius1,center.y-radius1,center.x+radius1,center.y-radius1,convertedLocation.x,convertedLocation.y)
			|| CCRectMake(center.x-radius1,center.y-radius1-side,side,side).containsPoint(convertedLocation)) {
			speedX=0;
			speedY=-1;
		}
		//向左
		else if(triangleContainPoint(center.x,center.y,center.x-radius1,center.y+radius1,center.x-radius1,center.y-radius1,convertedLocation.x,convertedLocation.y)
			|| CCRectMake(center.x-radius1-side,center.y-radius1,side,side).containsPoint(convertedLocation)) {
			speedX=-1;
			speedY=0;
		}
		//向右
		else if (triangleContainPoint(center.x,center.y,center.x+radius1,center.y+radius1,center.x+radius1,center.y-radius1,convertedLocation.x,convertedLocation.y)
			|| CCRectMake(center.x+radius1+side,center.y-radius1,side,side).containsPoint(convertedLocation)) {
			speedX=1;
			speedY=0;
		}
		//右上
		else if(convertedLocation.x-center.x>0&&convertedLocation.y-center.y>0)
		{
			speedX=0.7f;
			speedY=0.7f;
		}
		//左上
		else if (convertedLocation.x-center.x<0&&convertedLocation.y-center.y>0)
		{
			speedX=-0.7f;
			speedY=0.7f;
		}
		//左下
		else if (convertedLocation.x-center.x<0&&convertedLocation.y-center.y<0)
		{
			speedX=-0.7f;
			speedY=-0.7f;
		}
		//右下
		else if (convertedLocation.x-center.x>0&&convertedLocation.y-center.y<0)
		{
			speedX=0.7f;
			speedY=-0.7f;
		}
	}

}

void FlyScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
	this->isFlying = false;
	this->joystick->setPosition(center);
	this->speedX = this->speedY = 0;
}

void FlyScene::createBullet(float f) {
	if ((int)this->bullets.size() > this->gBulletNum) {
		return;
	}
	CCSprite *bullet = CCSprite::create("bullet.png");
	this->addChild(bullet, 0);
	bullets.push_back(bullet);

	float x, y;
	int speedX, speedY;
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	int entrance = abs(rand() % 4);
	//switch (entrance) {
	//case 0:
		x = abs(rand() % (int)size.width);
		y = size.height - bullet->boundingBox().size.height;
		speedX = rand() % 3;
		speedY = (abs(rand() % 3 + 1)) * -1;
		//break;
	/*case 1:
		x = abs(rand() % (int)size.width);
		y = bullet->boundingBox().size.height;
		speedX = rand() % 3;
		speedY = abs(rand() %3 + 1);
		break;

	case 2:
		x = bullet->boundingBox().size.width;
		y = abs(rand() % (int) size.height);
		speedX = abs(rand() % 3 + 1);
		speedY = rand() % 3;
		break;
	case 3:
		x = size.width - bullet->boundingBox().size.width;
		y = abs(rand() % (int) size.height);
		speedX = abs(rand() % 3 + 1) * -1;
		speedY = rand() % 3;
		break;*/
	//}

	CCActionInterval *action = CCMoveBy::create(0.1f, ccp(speedX, speedY));
	bullet->setPosition(ccp(x, y));
	bullet->runAction(CCRepeatForever::create(action));
}

void FlyScene::flying(float f) {
	if (this->isFlying && (this->speedX != 0 || this->speedY != 0)) {
		CCPoint position = ccp(plane->getPosition().x + speedX, plane->getPosition().y + speedY);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCRect rect = CCRectMake(0, 0, size.width, size.height);

		if (rect.containsPoint(position)) {
			plane->setPosition(position);
		}
	}
}

void FlyScene::checkBullet(float f) {
	if (!plane->isVisible()) return;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCRect screen = CCRectMake(0, 0, size.width, size.height);
	CCRect planeBox = plane->boundingBox();

	for (std::vector<CCSprite*>::iterator bitr = bullets.begin(); bitr != bullets.end();) {
		CCSprite *bullet = *bitr;
		CCPoint position = bullet->getPosition();
		CCRect bulletBox = bullet->boundingBox();

		if (planeBox.intersectsRect(bulletBox)) {
			this->removeChild(bullet, true);
			bitr = bullets.erase(bitr);
			plane->setVisible(false);

			CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("explosion.png");
			CCAnimation *animation = CCAnimation::create();
			animation->setDelayPerUnit(0.2f);

			for (int i = 0; i < 4; i ++) {
				animation->addSpriteFrameWithTexture(texture, CCRectMake(i*32, 0, 32, 32));
			}
			//animation->setLoops(-1);
			CCAnimate *animate = CCAnimate::create(animation);
			CCFiniteTimeAction *animateOver = CCCallFunc::create(this, callfunc_selector(FlyScene::explosionEndDid));
			CCFiniteTimeAction *sequence = CCSequence::createWithTwoActions(animate, animateOver);

			this->explosion = CCSprite::createWithTexture(texture, CCRectMake(0, 0, 32, 32));
			this->addChild(explosion);
			this->explosion->setPosition(plane->getPosition());
			//this->explosion->runAction(animate);
			this->explosion->runAction(sequence);
			break;
		} else if (!screen.containsPoint(position)) {
			this->removeChild(bullet, true);
			bitr = bullets.erase(bitr);
		} else {
			bitr ++;
		}
	}
}

void FlyScene::saveTime(float f) {

}

void FlyScene::explosionEndDid() {
	this->explosion->setVisible(false);
}


bool FlyScene::triangleContainPoint(float x1,float y1,float x2,float y2,float x3,float y3,float px,float py)  
{  
    //求S1的面积  
    float s1=heronsformula(x1,y1,x2,y2,px,py);  
    //求S2的面积  
    float s2=heronsformula(x2,y2,x3,y3,px,py);  
    //求S3的面积  
    float s3=heronsformula(x3,y3,x1,y1,px,py);  
    //求S的面积  
    float s=heronsformula(x1,y1,x2,y2,x3,y3);  
  
    //返回S是否等于S1,S2,S3的和  
    return abs(s-(s1+s2+s3))<0.001f;  
}  

//求三角形面积的函数的实现  
float FlyScene::heronsformula(float x1,float y1,float x2,float y2,float x3,float y3)  
{  
    //求边长a  
    float a=sqrt(pow(x1-x2,2)+pow(y1-y2,2));  
    //求边长b  
    float b=sqrt(pow(x2-x3,2)+pow(y2-y3,2));  
    //求边长c  
    float c=sqrt(pow(x3-x1,2)+pow(y3-y1,2));  
    //求周长s  
    float s=(a+b+c)/2;  
  
    //根据海伦公式返回三角形面积  
    return sqrt(s*(s-a)*(s-b)*(s-c));  
}  