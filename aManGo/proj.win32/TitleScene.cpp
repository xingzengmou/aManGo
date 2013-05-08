#include "TitleScene.h"
#include "FlyScene.h"

using namespace cocos2d;

cocos2d::CCScene* TitleScene::scene() {
	cocos2d::CCScene *scene = NULL;
	do {
		scene = cocos2d::CCScene::create();
		TitleScene *layer = TitleScene::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);
	return scene;
}

bool TitleScene::init() {
	bool bRet = false;
	do {
		std::string titleStr = "dead man";
		cocos2d::CCLabelTTF *pLabel = cocos2d::CCLabelTTF::create(titleStr.c_str(), "Thonburi", 30);
		CC_BREAK_IF(!pLabel);
		pLabel->setColor(cocos2d::ccRED);

		cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(size.width/2, size.height*0.8f));
		this->addChild(pLabel, 1);

		cocos2d::CCSprite *pSprite = cocos2d::CCSprite::create("background.png");
		CC_BREAK_IF(!pSprite);

		pSprite->setPosition(ccp(size.width/2, size.height/2));

		this->addChild(pSprite, 0);

		cocos2d::CCMenuItemFont::setFontName("thonburi");
		cocos2d::CCMenuItemFont::setFontSize(25);

		std::string menuItemStr = "new game";

		CCMenuItem *pNewGameMenuItem = CCMenuItemFont::create(menuItemStr.c_str(), this, menu_selector(TitleScene::menuNewCallback));
		//pNewGameFont->setPosition(ccp(size.width/2, size.height/2));
		CC_BREAK_IF(!pNewGameMenuItem);

		CCMenuItem *pOptionMenuItem = CCMenuItemFont::create("option", this, menu_selector(TitleScene::menuOptionCallback));
		//pNewGameFont->setPosition(ccp(size.width/2, size.height/2 - 30));
		CC_BREAK_IF(!pOptionMenuItem);

		CCMenuItem *pQuitMenuItem = CCMenuItemFont::create("quit", this, menu_selector(TitleScene::menuQuitCallback));
		CC_BREAK_IF(!pQuitMenuItem);

		CCMenu *pMenu = CCMenu::create(pNewGameMenuItem, pOptionMenuItem, pQuitMenuItem, NULL);
		CC_BREAK_IF(!pMenu);
		pMenu->alignItemsVertically();

		this->addChild(pMenu);

	} while (0);

	return true;
}

void TitleScene::menuNewCallback(CCObject* pSender) {
	CCScene *scene = FlyScene::scene();
	CCTransitionFlipX *m = CCTransitionFlipX::create(1.2f, scene);
	CCDirector::sharedDirector()->replaceScene(m);
}

void TitleScene::menuOptionCallback(CCObject* pSender) {

}
void TitleScene::menuQuitCallback(CCObject* pSender) {
	CCDirector::sharedDirector()->end();
}

