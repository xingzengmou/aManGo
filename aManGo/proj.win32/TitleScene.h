#ifndef __TITLESCENE_H__
#define __TITLESCENE_H__

#include <cocos2d.h>
//#include <SimpleAudioEngine.h>

class TitleScene : public cocos2d::CCLayer
{
public:
	
	virtual bool init();
	static cocos2d::CCScene* scene();
	void menuNewCallback(CCObject* pSender);
	void menuOptionCallback(CCObject* pSender);
	void menuQuitCallback(CCObject* pSender);

	CREATE_FUNC(TitleScene);
};

#endif