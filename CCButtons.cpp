#include "CCButtons.h"

USING_NS_CC;

CCButtons* CCButtons::create() 
{
	CCButtons* ret = new CCButtons();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool CCButtons::init() 
{
	if (!CCLayer::init()) return false;

	this->setTouchEnabled(true);
	_currentButton = nullptr;

	return true;
}

void CCButtons::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void CCButtons::addButton(CCSprite* button, const std::function<void()>& callback, float scaleNormal, float scalePressed) 
{
	this->addChild(button);
	button->setScale(scaleNormal);
	_buttons.push_back({ button, callback, scaleNormal, scalePressed });
}

bool CCButtons::ccTouchBegan(CCTouch* touch, CCEvent* event) 
{
	CCPoint location = this->convertToNodeSpace(touch->getLocation());

	for (auto& btnInfo : _buttons) {
		if (btnInfo.sprite->boundingBox().containsPoint(location)) {
			_currentButton = btnInfo.sprite;
			btnInfo.sprite->stopAllActions();
			btnInfo.sprite->runAction(CCScaleTo::create(0.05f, btnInfo.scalePressed));
			break;
		}
	}
	return true;
}

void CCButtons::ccTouchMoved(CCTouch* touch, CCEvent* event) 
{
	CCPoint location = this->convertToNodeSpace(touch->getLocation());

	for (auto& btnInfo : _buttons)
		{
		if (btnInfo.sprite->boundingBox().containsPoint(location)) {
			if (_currentButton != btnInfo.sprite) {
				resetAllButtonScale();
				_currentButton = btnInfo.sprite;
				btnInfo.sprite->stopAllActions();
				btnInfo.sprite->runAction(CCScaleTo::create(0.05f, btnInfo.scalePressed));
			}
			return;
		}
	}

	resetAllButtonScale();
	_currentButton = nullptr;
}

void CCButtons::ccTouchEnded(CCTouch* touch, CCEvent* event) 
{
	if (_currentButton) {
		for (auto& btnInfo : _buttons) 
		{
			if (btnInfo.sprite == _currentButton && btnInfo.callback) 
			{
				btnInfo.callback();
				btnInfo.sprite->stopAllActions();
				btnInfo.sprite->runAction(CCEaseBackOut::create(CCScaleTo::create(0.15f, btnInfo.scaleNormal)));
				break;
			}
		}
	}
	else {
		resetAllButtonScale();
	}

	_currentButton = nullptr;
}

void CCButtons::resetAllButtonScale() 
{
	for (auto& btnInfo : _buttons) {
		btnInfo.sprite->stopAllActions();
		btnInfo.sprite->runAction(CCScaleTo::create(0.1f, btnInfo.scaleNormal));
	}
}
