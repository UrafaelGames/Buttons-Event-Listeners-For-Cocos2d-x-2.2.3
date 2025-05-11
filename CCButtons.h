#ifndef BDBUTTONS_H
#define BDBUTTONS_H

#include "cocos2d.h"
#include <vector>
#include <functional>

class CCButtons : public cocos2d::CCLayer
{
public:
	virtual bool init();
	static CCButtons* create();

	void addButton(cocos2d::CCSprite* button, const std::function<void()>& callback, float scaleNormal = 1.f, float scalePressed = 1.2f);

	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void registerWithTouchDispatcher();

private:
	struct ButtonInfo
	{
		cocos2d::CCSprite* sprite;
		std::function<void()> callback;
		float scaleNormal;
		float scalePressed;
	};

	std::vector<ButtonInfo> _buttons;
	cocos2d::CCSprite* _currentButton;

	void resetAllButtonScale();
};

#endif // BDBUTTONS_H