#include "GameSet.h"
#include "Util.h"
#include "GrabTouch.h"
#include "FrameSprite.h"
#include "PlatformSpec.h"
#include "GlobalUserData.h"
#include "GameCommon.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "MenuItemFrameSprite.h"
#include "MenuItemScale9Sprite.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "MenuItemSpriteWithLabel.h"
#include "GameAbout.h"
#include "SimpleAudioEngine.h"
#include "GameHelp.h"
#include "GameData.h"
#include "Tools.h"
USING_NS_CC;

class FangChenMiDlg : public GameDialog
{
public:
	CREATE_FUNC(FangChenMiDlg);

protected:
	bool init()
	{
		if (!GameDialog::init("dialog/bgfangchenmi.webp", true))
		{
			return false;
		}

		Size cBtSize = this->getContentSize();
		this->GetMenuItemClose()->setPosition(cBtSize.width - 18, cBtSize.height - 18);

		auto szTxtField = Size(120, 28);
		static Vec2 s_ptTxtField[6] = {
			Vec2(209, 264), Vec2(446, 264), 
			Vec2(209, 200), Vec2(446, 200),
			Vec2(188, 135), Vec2(446, 135),
		};

		for (int i = 0; i < 6; i++)
		{
			m_pTxtFiled[i] = CreateNode2AndPosAddTo<ui::EditBox>(szTxtField, ui::Scale9Sprite::create("login/bgedit.png") , s_ptTxtField[i].x, s_ptTxtField[i].y, this, 0);
			m_pTxtFiled[i]->setFont("fzlt.ttf", 24);
			m_pTxtFiled[i]->setFontColor(Color3B(0xff, 0xd2, 0));
			m_pTxtFiled[i]->setInputMode(ui::EditBox::InputMode::NUMERIC);
            m_pTxtFiled[i]->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		}

		//auto bt = MenuItemFrameSprite::create("dialog/bt1.png", 2, [this](Ref*){
        auto bt = MenuItemScale9Sprite::create(0, Size(156,74), [this](Ref*){
			setVisible(false);
			Json::Value param;
			param["type"] = 2;
			param["data"]["money"] = m_pTxtFiled[0]->getText();
			param["data"]["time"] = m_pTxtFiled[1]->getText();
			SendClientRequest(ClientRequest::kSetGameSet, param);
			param["type"] = 3;
			param["data"]["money"] = m_pTxtFiled[2]->getText();
			param["data"]["time"] = m_pTxtFiled[3]->getText();
			SendClientRequest(ClientRequest::kSetGameSet, param);
			param["type"] = 4;
			param["data"]["money"] = m_pTxtFiled[4]->getText();
			param["data"]["time"] = m_pTxtFiled[5]->getText();
			SendClientRequest(ClientRequest::kSetGameSet, param);
		});
		bt->setPosition(339, 60);
		auto label = CreateLabelMSYHBD(30, "确定");
		auto sz = bt->getContentSize();
		label->setPosition(sz.width / 2, sz.height / 2);
		bt->addChild(label);
		auto pMenu = Menu::create(bt, nullptr);
		pMenu->setPosition(Vec2::ZERO);
		addChild(pMenu);

		return true;
	}

	virtual void onSetVisible(bool bVisible)
	{
		if (bVisible)
		{
			char szTxt[128] = {0};
			m_pTxtFiled[0]->setText(do_fraction(g_globalConfig.m_dMaxWinPerDay).c_str());
			sprintf(szTxt, "%d", g_globalConfig.m_uFreezenMinWhenWin);
			m_pTxtFiled[1]->setText(szTxt);
			m_pTxtFiled[2]->setText(do_fraction(g_globalConfig.m_dMaxLosePerDay).c_str());
			sprintf(szTxt, "%d", g_globalConfig.m_uFreezenMinWhenLose);
			m_pTxtFiled[3]->setText(szTxt);
			sprintf(szTxt, "%d", g_globalConfig.m_MaxOnlineHour);
			m_pTxtFiled[4]->setText(szTxt);
			sprintf(szTxt, "%d", g_globalConfig.m_uFreezenMinWhenTimeFull);
			m_pTxtFiled[5]->setText(szTxt);
		}
	}

private:
	cocos2d::ui::EditBox *m_pTxtFiled[6];
};

void GameSet::onSetVisible(bool bVisible)
{
	//m_pTouchGrabberZhuXiao->SetTouchEnable(bVisible);
	m_pTouchGrabberYinYue->SetTouchEnable(bVisible);
	m_pTouchGrabberYinXiao->SetTouchEnable(bVisible);
	m_pTouchGrabberVibrate->SetTouchEnable(bVisible);
	//m_pTouchGrabberJinZhiPangGuan->SetTouchEnable(bVisible);
	m_pTouchGrabberJinZhiTongZhuo->SetTouchEnable(bVisible);
	m_pTouchGrabberJuJueYaoQing->SetTouchEnable(bVisible);
	m_pTouchGrabberPaiZhuoXuanZe->SetTouchEnable(bVisible);
	m_pTouchGrabberPaiMianXuanZe->SetTouchEnable(bVisible);
	m_pTouchGrabberFangChenMi->SetTouchEnable(bVisible);
	m_pTouchGrabberGuanYu->SetTouchEnable(bVisible);
	m_pTouchGrabberXinShowJiaoCheng->SetTouchEnable(bVisible);
	m_pTouchGrabberBangZhu->SetTouchEnable(bVisible);

	if (bVisible)
	{
	}
	else
	{
	}
}

bool GameSet::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}

	auto bg = Sprite::create("dialog/bg.png");
	auto sz = bg->getContentSize();
	bg->setPosition(sz.width / 2, sz.height / 2);
	setContentSize(sz);
	addChild(bg);

	auto title = CreateNode1AndPosAddTo<Sprite>("dialog/titlegameset.png", 470, 568, this, 0);
	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(900, 430), 20, 68, this, 0);
	m_pScrollView->setContentSize(Size(900, 1110));
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);

	auto labelAnchor = Vec2(0, 0);
	int labelSize = 26;
	int labelFenLeiSize = 24;
	auto labelClr = Color4B::WHITE;
	auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);

	// 帮助
	auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 43, m_pScrollView, 0);
	auto btExpand = CreateNode3AndPosAddTo<FrameSprite>("dialog/btexpand.png", 1, 2, 695, 35, bgItem, 0);
	auto label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "帮助", labelAnchor, 29, 21, labelClr, bgItem, 0);
	auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
		[this](bool bSel, Node*, TouchGrabber*){if (bSel) PopDlgBangZhu();});
	m_pTouchGrabberBangZhu = touchGrabber;

	// 新手教程
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 116, m_pScrollView, 0);
	btExpand = CreateNode3AndPosAddTo<FrameSprite>("dialog/btexpand.png", 1, 2, 695, 36, bgItem, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "新手教程", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
		[this](bool bSel, Node*, TouchGrabber*){if (bSel) PopDlgXinShowJiaoCheng();});
	m_pTouchGrabberXinShowJiaoCheng = touchGrabber;

	// 关于
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 189, m_pScrollView, 0);
	btExpand = CreateNode3AndPosAddTo<FrameSprite>("dialog/btexpand.png", 1, 2, 695, 36, bgItem, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "关于", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
		[this](bool bSel, Node*, TouchGrabber*){if (bSel) PopGuanYu();});
	m_pTouchGrabberGuanYu = touchGrabber;

	// 版本
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 262, m_pScrollView, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "版本", labelAnchor, 29, 21, labelClr, bgItem, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, getVersionName(), Vec2(1, 0), 720, 21, labelClr, bgItem, 0);
	
	// 其他（分类）
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelFenLeiSize, "其他", labelAnchor, 87, 302, labelFenLeiClr, m_pScrollView, 0);

	// 防沉迷设置
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 372, m_pScrollView, 0);
	btExpand = CreateNode3AndPosAddTo<FrameSprite>("dialog/btexpand.png", 1, 2, 695, 36, bgItem, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "防沉迷设置", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
		[this](bool bSel, Node*, TouchGrabber*){if (bSel) PopDlgFangChenMi();});
	m_pTouchGrabberFangChenMi = touchGrabber;

	// 牌面选择
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 445, m_pScrollView, 0);
	btExpand = CreateNode3AndPosAddTo<FrameSprite>("dialog/btexpand.png", 1, 2, 695, 36, bgItem, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "牌面选择", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
		[this](bool bSel, Node*, TouchGrabber*){if (bSel) PopDlgPaiMianXuanZe();});
	m_pTouchGrabberPaiMianXuanZe = touchGrabber;

	// 牌桌选择
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 518, m_pScrollView, 0);
	btExpand = CreateNode3AndPosAddTo<FrameSprite>("dialog/btexpand.png", 1, 2, 695, 36, bgItem, 0);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "牌桌选择", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
		[this](bool bSel, Node*, TouchGrabber*){if (bSel) PopDlgPaiZhuoXuanZe();});
	m_pTouchGrabberPaiZhuoXuanZe = touchGrabber;

	// 拒绝好友邀请
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 591, m_pScrollView, 0);
	auto btSwitch = CreateNode3AndPosAddTo<FrameSprite>("dialog/btswitch.png", 1, 2, 644, 36, bgItem, 0);
	m_pSpriteJuJueYaoQing = btSwitch;
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "拒绝好友邀请", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);},
		[this, btSwitch](bool bSel, Node*, TouchGrabber*)
	{
		if (bSel)
		{
			bool bJinZhi = btSwitch->GetFrameIdx() == 0;
			bJinZhi = !bJinZhi;
			g_globalConfig.m_bJuJueYaoQing = bJinZhi;
			btSwitch->SetFrameIdx(bJinZhi ? 0 : 1);

			Json::Value param;
			param["type"] = 5;
			param["data"] = bJinZhi ? 1 : 0;
			SendClientRequest(ClientRequest::kSetGameSet, param);
		}
	});
	m_pTouchGrabberJuJueYaoQing = touchGrabber;

	// 禁止同桌
	unsigned uItemOff = 70;
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 593 + uItemOff, m_pScrollView, 0);
	btSwitch = CreateNode3AndPosAddTo<FrameSprite>("dialog/btswitch.png", 1, 2, 644, 36, bgItem, 0);
	m_pSpriteJinZhiTongZhuo = btSwitch;
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "IP前三位相同禁止同桌", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);},
		[this, btSwitch](bool bSel, Node*, TouchGrabber*){
			if (bSel) 
			{
				//btSwitch->SetFrameIdx(1-btSwitch->GetFrameIdx());
				bool bJinZhi = btSwitch->GetFrameIdx() == 0;
				bJinZhi = !bJinZhi;
				g_globalConfig.m_bJinZhiTongZhuo = bJinZhi;
				btSwitch->SetFrameIdx(bJinZhi ? 0 : 1);

				Json::Value param;
				param["type"] = 1;
				param["data"] = bJinZhi ? 1 : 0;
				SendClientRequest(ClientRequest::kSetGameSet, param);

			}
	});
	m_pTouchGrabberJinZhiTongZhuo = touchGrabber;

	// 禁止旁观
	/*bgItem = CreateNode3AndPosAddTo<FrameSprite>("dialog/bgitem.png", 1, 2, 358, 646 + uItemOff, m_pScrollView, 0);
	btSwitch = CreateNode3AndPosAddTo<FrameSprite>("dialog/btswitch.png", 1, 2, 591, 31, bgItem, 0);
	m_pSpriteJinZhiPangGuan = btSwitch;
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "禁止旁观", labelAnchor, 26, 17, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
	[bgItem](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);},
	[this, btSwitch](bool bSel, Node*, TouchGrabber*){
	if (bSel) 
	{
	bool bJinZhi = btSwitch->GetFrameIdx() == 0;
	bJinZhi = !bJinZhi;
	g_globalConfig.m_bJinZhiPangGuan = bJinZhi;
	btSwitch->SetFrameIdx(bJinZhi ? 0 : 1);

	Json::Value param;
	param["type"] = 0;
	param["data"] = bJinZhi ? 0 : 1;
	SendClientRequest(ClientRequest::kSetGameSet, param);
	}
	});
	m_pTouchGrabberJinZhiPangGuan = touchGrabber;*/

	// 游戏设置
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelFenLeiSize, "游戏设置", labelAnchor, 87, 634 + uItemOff, labelFenLeiClr, m_pScrollView, 0);

	double dSoundValue, dEffectValue;
	g_globalConfig.GetSoundValue(dSoundValue, dEffectValue);

	// 震动

	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 705 + uItemOff, m_pScrollView, 0);
	btSwitch = CreateNode3AndPosAddTo<FrameSprite>("dialog/btswitch.png", 1, 2, 644, 36, bgItem, 0);
	m_pSpriteVibrate = btSwitch;
	btSwitch->SetFrameIdx(DataMgr->getVibrate() ? 0 : 1);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "震动", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);},
		[this, btSwitch](bool bSel, Node*, TouchGrabber*) {
			unsigned uCurSel = btSwitch->GetFrameIdx();
			if (bSel) {
				btSwitch->SetFrameIdx(1 - uCurSel);
				bool flagVibrate = false;
				if (uCurSel == 0)
				{
					flagVibrate = false;
				}
				else
				{
					flagVibrate = true;
				}

				DataMgr->setVibrate(flagVibrate);
			}
	});
	m_pTouchGrabberVibrate = touchGrabber;


	// 音效
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 778 + uItemOff, m_pScrollView, 0);
	btSwitch = CreateNode3AndPosAddTo<FrameSprite>("dialog/btswitch.png", 1, 2, 644, 36, bgItem, 0);
	m_pSpriteYinXiao = btSwitch;
	btSwitch->SetFrameIdx(dEffectValue<0.5);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "音效", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);},
		[this, btSwitch](bool bSel, Node*, TouchGrabber*){
			unsigned uCurSel = btSwitch->GetFrameIdx();
			if (bSel) 
			{
				btSwitch->SetFrameIdx(1 - uCurSel);
				double dValue = 0;
				if (uCurSel == 0)
				{
					dValue = 0;
				}
				else
				{
					dValue = 1;
				}
				g_globalConfig.SetSoundEffectValue(dValue);
				auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
				pAudioEngine->setEffectsVolume(dValue);
			}

	});
	m_pTouchGrabberYinXiao = touchGrabber;

	// 音乐
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 851 + uItemOff, m_pScrollView, 0);
	btSwitch = CreateNode3AndPosAddTo<FrameSprite>("dialog/btswitch.png", 1, 2, 644, 36, bgItem, 0);
	m_pSpriteYinYue = btSwitch;
	btSwitch->SetFrameIdx(dSoundValue<0.5);
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "音乐", labelAnchor, 29, 21, labelClr, bgItem, 0);
	touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[bgItem](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);},
		[this, btSwitch](bool bSel, Node*, TouchGrabber*){
			unsigned uCurSel = btSwitch->GetFrameIdx();
			if (bSel) 
			{
				btSwitch->SetFrameIdx(1 - uCurSel);
				double dValue = 0;
				if (uCurSel == 0)
				{
					dValue = 0;
				}
				else
				{
					dValue = 1;
				}
				g_globalConfig.SetSoundMusicValue(dValue);
				auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
				pAudioEngine->setBackgroundMusicVolume(dValue);
			}
	});
	m_pTouchGrabberYinYue = touchGrabber;

	// 声音设置(分类)
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelFenLeiSize, "声音设置", labelAnchor, 87, 896 + uItemOff, labelFenLeiClr, m_pScrollView, 0);

	// 账户
	bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 449, 967 + uItemOff, m_pScrollView, 0);
	//m_pBtZhuXiao = CreateNode3AndPosAddTo<FrameSprite>("dialog/bt.png", 1, 2, 597, 32, bgItem, 0);
	m_pBtZhuXiao = MenuItemScale9SpriteWithLabel::create(0, Size(203, 72), CC_CALLBACK_1(GameSet::ZhuXiao, this));
	m_pBtZhuXiao->setPosition(Vec2(636, 35));
	auto friendsWorldRecord = Menu::create(m_pBtZhuXiao,nullptr);
	friendsWorldRecord->setPosition(Vec2::ZERO);
	bgItem->addChild(friendsWorldRecord);

	CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "注销", Vec2(0.5, 0.5), 644, 36, labelClr, bgItem, 0);
	m_pLabelAccount = CreateLabelMSYHAndAnchorPosClrAddTo(labelSize, "", labelAnchor, 29, 21, labelClr, bgItem, 0);

	// 账户(分类)
	label = CreateLabelMSYHAndAnchorPosClrAddTo(labelFenLeiSize, "昵称", labelAnchor, 87, 1007 + uItemOff, labelFenLeiClr, m_pScrollView, 0);

	m_pScrollView->setContentOffset(Vec2(0, 348 - 957 - (int)uItemOff), false);

	m_pFangChenMiDlg = nullptr;
	m_pGameAbout = nullptr;
    m_pGameHelp = nullptr;

	g_globalMyData.WaitChange(WAITNICKNAMECHANGE, std::bind(&GameSet::OnMyDataChanged, this, std::placeholders::_1));
	return true;
}

void GameSet::UpdateSet()
{
	SetLabelString(m_pLabelAccount, g_globalMyData.GetNickName());
	double dSoundValue, dEffectValue;
	g_globalConfig.GetSoundValue(dSoundValue, dEffectValue);
	m_pSpriteYinYue->SetFrameIdx(dSoundValue<0.5);
	m_pSpriteYinXiao->SetFrameIdx(dEffectValue<0.5);
	m_pSpriteVibrate->SetFrameIdx(DataMgr->getVibrate() ? 0 : 1);
	//m_pSpriteJinZhiPangGuan->SetFrameIdx(g_globalConfig.m_bJinZhiPangGuan ? 0 : 1);
	m_pSpriteJinZhiTongZhuo->SetFrameIdx(g_globalConfig.m_bJinZhiTongZhuo ? 0 : 1);
	m_pSpriteJuJueYaoQing->SetFrameIdx(g_globalConfig.m_bJuJueYaoQing ? 0 : 1);
}

void GameSet::PopDlgBangZhu()
{
    if (nullptr == m_pGameHelp)
    {
        m_pGameHelp = GameHelp::create();
        m_pGameHelp->setPosition(0, 0);
        addChild(m_pGameHelp, 201);
    }
    m_pGameHelp->setVisible(true);
}

void GameSet::PopDlgXinShowJiaoCheng()
{
    SendClientRequest(ClientRequest::kNewbieGuidLayer);
}

void GameSet::PopDlgFangChenMi()
{
	if (nullptr == m_pFangChenMiDlg)
	{
		m_pFangChenMiDlg = FangChenMiDlg::create();
		m_pFangChenMiDlg->setVisible(false);
		m_pFangChenMiDlg->setPosition(130, 112);
		addChild(m_pFangChenMiDlg);
	}
	m_pFangChenMiDlg->setVisible(true);
}

void GameSet::PopDlgPaiMianXuanZe()
{
}

void GameSet::PopDlgPaiZhuoXuanZe()
{
}

void GameSet::ZhuXiao(Ref* pSender)
{
	SendClientRequest(ClientRequest::kZhuXiao);
	setVisible(false);
	m_pBtZhuXiao->setEnabled(true);
}

void GameSet::ZhuXiaoShowSetVisible(bool bIfShow)
{
	m_pBtZhuXiao->setEnabled(bIfShow);
}


void GameSet::PopGuanYu()
{
	if(m_pGameAbout == nullptr)
	{
		m_pGameAbout = GameAbout::create();
		this->addChild(m_pGameAbout, 201);
		m_pGameAbout->setVisible(false);
	}
	m_pGameAbout->setVisible(true);
}

void GameSet::OnMyDataChanged(unsigned uDataType)
{
	if (uDataType == WAITNICKNAMECHANGE)
	{
		SetLabelString(m_pLabelAccount, g_globalMyData.GetNickName());
	}
}

void GameSet::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
	GameDialog::setVisible(true);
    if (bVisible)
    {
		Tools::callScaleAction(this) ;
		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 1), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		//GameDialog::setVisible(false);
		callHideAction() ;
		/*Tools::callHideScaleAction(this) ;*/

		/* auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
    }
}