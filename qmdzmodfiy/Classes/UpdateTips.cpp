#include "UpdateTips.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "UpdateManager.h"
#include "PlatformSpec.h"

USING_NS_CC ;

bool UpdateTips::init()
{
	if (!GameLayer::init("userinfo/bgdialog0.png", false))
	{
		return false;
	}
	Size bgSize = this->getContentSize() ;
	auto mLableTips = CreateLabelMSYH(30, "检测到新版本，是否更新？");
	mLableTips->setTextColor(Color4B::WHITE);
	mLableTips->setPosition(bgSize.width/2,248) ;
	this->addChild(mLableTips);

	auto mLableFriendlyTips = CreateLabelMSYH(22, "友情提示：建议在WIFI环境下更新");
	mLableFriendlyTips->setTextColor(Color4B(0x50,0xbd,0x2e,255));
	mLableFriendlyTips->setPosition(bgSize.width/2,180) ;
	this->addChild(mLableFriendlyTips);




	//auto btCancel = MenuItemSpriteWithLabel::create("update/btcancel.png", 3, CC_CALLBACK_1(UpdateTips::callCancel,this));
    auto btCancel = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(UpdateTips::callCancel,this));
	Size szMenu = btCancel->getContentSize();
	btCancel->setPosition(bgSize.width/2-130, szMenu.height/2+40);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "取消", Vec2(0.5, 0.5), szMenu.width/2, szMenu.height/2, Color4B(0xff,0xff,0xff,0xff), btCancel, 0);




	//auto btSure = MenuItemSpriteWithLabel::create("update/btsure.png", 3, CC_CALLBACK_1(UpdateTips::callUpdate,this));
    auto btSure = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), CC_CALLBACK_1(UpdateTips::callUpdate,this));
	Size szSure = btSure->getContentSize();
	btSure->setPosition(bgSize.width/2+130, szSure.height/2+40);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "更新", Vec2(0.5, 0.5), szSure.width/2, szSure.height/2, Color4B(0xff,0xff,0xff,0xff), btSure, 0);

	auto menuGet = Menu::create(btCancel,btSure, nullptr);
	menuGet->setPosition(Vec2::ZERO);
	this->addChild(menuGet);

	return true ;
}

void UpdateTips::callCancel(cocos2d::Ref* pSend)
{
	Director::getInstance()->end() ;
}

void UpdateTips::callUpdate(cocos2d::Ref* pSend)
{
	auto pUpdateManager = (UpdateManager*)getParent() ;
	setVisible(false) ;
	pUpdateManager->showUpdateInfoLayer(true) ;
	callDoUpdate() ;

}