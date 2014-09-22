#ifndef MAINMENU_H
#define MANIMENU_H
#include "cocos2d.h"
USING_NS_CC;

class MainMenu : public CCLayer
{
public:
	MainMenu();

	~MainMenu();

	virtual bool init();

	CREATE_FUNC(MainMenu);
	void touchMenu(CCObject *pSender);
	void toLevelChoice(CCObject *pSender);
	void toSetting(CCObject *pSender);	
	void toHelp(CCObject *pSender);
	void goGamble(CCObject *pSender);
	void toMusicSetting(CCObject *pSender);
	void toSoundSetting(CCObject *pSender);
	void keyBackClicked();
	void goGamble();

};
#endif