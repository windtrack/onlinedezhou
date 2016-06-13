#include "UserInfo.h"
#include "HttpSprite.h"
#include "FrameSprite.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "GrabTouch.h"
#include "Card.h"
#include "TextureNumber.h"
#include "GameCommon.h"
#include "GrabKeyboard.h"
#include "MenuItemFrameSprite.h"
#include "GlobalUserData.h"
#include "GameHall.h"
#include "FrameSpriteButton.h"
#include "MobileBind.h"
#include "AnimationManager.h"
#include "LogonLayerEx.h"
#include "Tools.h"
#include "GameConfig.h"
USING_NS_CC;

#define CLR_DETAIL_YELLOW (Color4B(0xff, 0xd2, 0x0, 255))

void UserInfoView::UpdateUserInfo(const UserDetailInfo& usrInfo, bool bLookOn, bool bMySelf,bool bOwner)
{
	m_pMenuItemGetVIP->setVisible(true) ;
	m_UsrInfo = usrInfo;
	SetLabelStringWithBoundWidth(m_pLabelName, m_UsrInfo.m_strName, 170);
	if (bMySelf) {
		if (g_globalMyData.GetSkinID() == 0) {
			SetIconUrl(m_pIcon, g_globalMyData.GetIconURL());
		} else {
			SetIconUrl(m_pIcon, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
		}
	} else {
		SetIconUrl(m_pIcon, m_UsrInfo.m_strIcon);
	}
	
	m_pSpriteSex->SetFrameIdx(m_UsrInfo.m_bBoy ? 1 : 0);
	if (bMySelf)
	{
		m_pMenuItemAt->setVisible(false);
        m_pLabelDiamond->setVisible(true);
        m_pSpriteDiamond->setVisible(true);
	}
	else
	{
		SetLabelString(m_pMenuItemAt->GetLabel(), m_UsrInfo.m_bBoy ? "@Ta" : "@Ta");
		m_pMenuItemAt->setVisible(true);
        m_pSpriteDiamond->setVisible(false);
        m_pLabelDiamond->setVisible(false);
	}

	SetLabelString(m_pLabelLeBi, FormatCash(m_UsrInfo.m_dCash));
    SetLabelString(m_pLabelDiamond, FormatCash3(m_UsrInfo.m_dDiamond));

	char szTxt[128] = {0};
	sprintf(szTxt, "%d", m_UsrInfo.m_uUID);
	SetLabelString(m_pLabelUID, szTxt);

	// 根据玩家的俱乐部等级来显示俱乐部的按钮颜色
	for(int i = 0;i < 4; i++)
	{
		m_pMenuItemFamilyButton[i]->setVisible(false);
		m_pMenuItemFamilyLabel[i]->setVisible(false);
	}
	if( m_UsrInfo.m_familyLV > 0 && m_UsrInfo.m_familyLV <= 2)
	{
		m_pMenuItemFamilyButton[0]->setVisible(true);
		m_pMenuItemFamilyLabel[0]->setVisible(true);
		m_uFamilyID = 0;
	}
	else if(m_UsrInfo.m_familyLV > 2 && m_UsrInfo.m_familyLV <= 5)
	{
		m_pMenuItemFamilyButton[1]->setVisible(true);
		m_pMenuItemFamilyLabel[1]->setVisible(true);
		m_uFamilyID = 1;
	}
	else if(m_UsrInfo.m_familyLV > 5 && m_UsrInfo.m_familyLV <= 8)
	{
		m_pMenuItemFamilyButton[2]->setVisible(true);
		m_pMenuItemFamilyLabel[2]->setVisible(true);
		m_uFamilyID = 2;
	}
	else if(m_UsrInfo.m_familyLV > 8)
	{
		m_pMenuItemFamilyButton[3]->setVisible(true);
		m_pMenuItemFamilyLabel[3]->setVisible(true);
		m_uFamilyID = 3;
	}

	// 判断俱乐部标记是否是自己
	if(bMySelf)
	{
		m_uUserFamilyID = g_globalMyData.getFamilyID();
		std::string m_uMyFamilyName = g_globalMyData.getFamilyName();
		if(g_globalMyData.getHasCreatFamily())
		{
			SetLabelString(m_pMenuItemFamilyLabel[m_uFamilyID], g_globalMyData.getFamilyName().c_str());
			m_pMenuItemFamilyLabel[m_uFamilyID]->setVisible(true);
			m_pMenuItemFamilyButton[m_uFamilyID]->setVisible(true);
		}
		else
		{
			m_pMenuItemFamilyLabel[m_uFamilyID]->setVisible(false);
			m_pMenuItemFamilyButton[m_uFamilyID]->setVisible(false);
		}
	}
	else
	{
		m_uUserFamilyID = m_UsrInfo.m_familyID;
		std::string m_uUserFamilyName = m_UsrInfo.m_familyName;
		if(m_uUserFamilyName == "" || (m_UsrInfo.m_familyLV <= 0 && m_UsrInfo.m_familyLV > 10))
		{
			m_pMenuItemFamilyLabel[m_uFamilyID]->setVisible(false);
			m_pMenuItemFamilyButton[m_uFamilyID]->setVisible(false);
		}
		else
		{
			SetLabelString(m_pMenuItemFamilyLabel[m_uFamilyID], m_uUserFamilyName.c_str());
			m_pMenuItemFamilyLabel[m_uFamilyID]->setVisible(true);
			m_pMenuItemFamilyButton[m_uFamilyID]->setVisible(true);
		}
		
	}

	if (g_globalMyData.GetVipLevel() > 0)
	{
		m_pSpriteVIPDengJi->setVisible(true);
		m_pSpriteVIPDengJi->SetFrameIdx(m_UsrInfo.m_iVipDengJi-1);
	}
	else
	{
		m_pSpriteVIPDengJi->setVisible(false);
	}
	if (m_UsrInfo.m_iVipDengJi > 0 && bMySelf)
	{
		for (auto item : m_pLabelVIP)
		{
			item->setTextColor(Color4B::WHITE);
		}
		m_pSpriteVIPDengJi->setVisible(true);
		m_pSpriteVIPDengJi->SetFrameIdx(m_UsrInfo.m_iVipDengJi-1);
		m_pLabelGetVIP->setVisible(false);
		m_pMenuItemGetVIP->setVisible(false);

		sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fJiJinDu).c_str());
		SetLabelString(m_pLabelJiJinDu, szTxt);
		
		sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fFanQianXiaZhu).c_str());
		SetLabelString(m_pLabelFanQianJiaZhu, szTxt);

		sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fZaiJiaZhu).c_str());
		SetLabelString(m_pLabelZaiJiaZhu, szTxt);

		sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fChiXuXiaZhu).c_str());
		SetLabelString(m_pLabelChiXuXiaZhu, szTxt);
	}
	else
	{
		for (auto item : m_pLabelVIP)
		{
			item->setTextColor(Color4B(0x48, 0x48, 0x48, 0xff));
		}
		
		m_pSpriteVIPDengJi->setVisible(false);
		
		m_pLabelGetVIP->setVisible(true);
		m_pMenuItemGetVIP->setVisible(true);

		SetLabelString(m_pLabelJiJinDu, "");
		SetLabelString(m_pLabelFanQianJiaZhu, "");
		SetLabelString(m_pLabelZaiJiaZhu, "");
		SetLabelString(m_pLabelChiXuXiaZhu, "");

		
	}

	if(m_UsrInfo.m_iVipDengJi > 0 && !bMySelf)
	{
		m_pSpriteVIPDengJi->SetFrameIdx(m_UsrInfo.m_iVipDengJi-1);
		m_pSpriteVIPDengJi->setVisible(true);
	}
    unsigned uCheckOut = 0;
	if(g_globalMyData.GetVipLevel() > 0 && !bMySelf)
	{
		for (auto item : m_pLabelVIP)
		{
			item->setTextColor(Color4B::WHITE);
		}
		m_pLabelGetVIP->setVisible(false);
		m_pMenuItemGetVIP->setVisible(false);

        uCheckOut = m_UsrInfo.m_fJiJinDu * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fJiJinDu * 100.0f)%100 : m_UsrInfo.m_fJiJinDu * 100.0f;
        sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
		//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fJiJinDu).c_str());
		SetLabelString(m_pLabelJiJinDu, szTxt);

        uCheckOut = m_UsrInfo.m_fFanQianXiaZhu * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fFanQianXiaZhu * 100.0f)%100 : m_UsrInfo.m_fFanQianXiaZhu * 100.0f;
        sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
		//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fFanQianXiaZhu).c_str());
		SetLabelString(m_pLabelFanQianJiaZhu, szTxt);

        uCheckOut = m_UsrInfo.m_fZaiJiaZhu * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fZaiJiaZhu * 100.0f)%100 : m_UsrInfo.m_fZaiJiaZhu * 100.0f;
        sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
		//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fZaiJiaZhu).c_str());
		SetLabelString(m_pLabelZaiJiaZhu, szTxt);

        uCheckOut = m_UsrInfo.m_fChiXuXiaZhu * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fChiXuXiaZhu * 100.0f)%100 : m_UsrInfo.m_fChiXuXiaZhu * 100.0f;
        sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
		//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fChiXuXiaZhu).c_str());
		SetLabelString(m_pLabelChiXuXiaZhu, szTxt);
		if(m_UsrInfo.m_iVipDengJi > 0)
			m_pSpriteVIPDengJi->setVisible(false);
		else
			m_pSpriteVIPDengJi->setVisible(false);
	}


	sprintf(szTxt, "%d", m_UsrInfo.m_iMeiLiZhi);
	SetLabelString(m_pLabelMeiLi, szTxt);

	sprintf(szTxt, "LV%d", m_UsrInfo.m_uJingYanDengJi);
	SetLabelString(m_pLabelJingYanDengJi1, szTxt);

	sprintf(szTxt, "%d/%d", m_UsrInfo.m_uJingYanJinDu, m_UsrInfo.m_uJingYanZongZhi);
	SetLabelString(m_pLabelJingYanDengJi2, szTxt);

	m_pProgressJingYanDengJi->setPercentage((float)m_UsrInfo.m_uJingYanJinDu / (float)m_UsrInfo.m_uJingYanZongZhi * 100.0f);

	sprintf(szTxt, "%d", m_UsrInfo.m_uPaiJu);
	SetLabelString(m_pLabelPaiJu, szTxt);
		
    uCheckOut = m_UsrInfo.m_fRuJuLv * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fRuJuLv * 100.0f)%100 : m_UsrInfo.m_fRuJuLv * 100.0f;
    sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
	//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fRuJuLv * 100.0f).c_str());
	SetLabelString(m_pLabelRuJuLv, szTxt);

    uCheckOut = m_UsrInfo.m_fShengLv * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fShengLv * 100.0f)%100 : m_UsrInfo.m_fShengLv * 100.0f;
    sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
	//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fShengLv * 100.0f).c_str());
	SetLabelString(m_pLabelShengLv, szTxt);

    uCheckOut = m_UsrInfo.m_fTanPaiLv * 100.0f > 100.0f ? (int)(m_UsrInfo.m_fTanPaiLv * 100.0f)%100 : m_UsrInfo.m_fTanPaiLv * 100.0f;
    sprintf(szTxt, "%s%%", do_fraction(uCheckOut).c_str());
	//sprintf(szTxt, "%s%%", do_fraction(m_UsrInfo.m_fTanPaiLv * 100.0f).c_str());
	SetLabelString(m_pLabelTanPaiLv, szTxt);



	m_pSpriteBiaoJi->setVisible(!bMySelf);
//	std::string m_strBiaoJi;
	if (bMySelf)
	{
		m_pMenuItemGuanZhu->setVisible(false);
		m_pMenuItemQuXiaoGuanZhu->setVisible(false);
	
		m_pMenuItemPingBi->setVisible(false);
		m_pMenuItemQuXiaoPingBi->setVisible(false);

		m_pMenuItemZhuBoDangAn->setVisible(false);
		m_pSpriteBeiZhu->setVisible(false);
	}
	else
	{
		m_pMenuItemGuanZhu->setVisible(!m_UsrInfo.m_bYiGuanZhu);
		m_pMenuItemQuXiaoGuanZhu->setVisible(m_UsrInfo.m_bYiGuanZhu);
	
		m_pMenuItemPingBi->setVisible(!m_UsrInfo.m_bYiPingBi);
		m_pMenuItemQuXiaoPingBi->setVisible(m_UsrInfo.m_bYiPingBi);

		m_pMenuItemZhuBoDangAn->setVisible(true);
		//m_pMenuItemZhuBoDangAn->setEnabled(m_UsrInfo.m_bZhuBo);

		
		

		if (m_UsrInfo.m_strBeiZhu.length()==0)
		{
			m_pSpriteBeiZhu->setVisible(true);
			m_pEdtBeiZhu->setText("");
		}
		else
		{
			m_pSpriteBeiZhu->setVisible(false);
			m_pEdtBeiZhu->setText(m_UsrInfo.m_strBeiZhu.c_str());
		}
		if(m_UsrInfo.m_iBiaoJi == -1)
		{
			m_pSpriteBiaoJi->setVisible(false);
			m_pMenuItemBiaoJi->setPosition(590, 325);
		}
		else
		{
			m_pSpriteBiaoJi->SetFrameIdx(m_UsrInfo.m_iBiaoJi);
		}
		
	}


	//bool m_bZhuBo;
	//unsigned m_uZhuBoFangJian;
	//std::string m_strShengRi;
	//unsigned m_uShenGao;
	//unsigned m_uTiZhong;
	//std::string m_strYiShuZhao[4];

	if (!bLookOn)
	{
		ShowTool(!bMySelf);
		ShowExpress(bMySelf);
		m_pLabelLookOnTip->setVisible(false);

		m_pLabelBeiZhu->setVisible(!bMySelf);
		m_pLabelBiaoJi->setVisible(!bMySelf);
		m_pEdtBeiZhu->setVisible(!bMySelf);
		m_pMenuItemBiaoJi->setVisible(!bMySelf);
	}
	else
	{
		ShowTool(false);
		ShowExpress(false);

		m_pMenuItemGuanZhu->setVisible(false);
		m_pMenuItemQuXiaoGuanZhu->setVisible(false);
		m_pLabelBeiZhu->setVisible(false);
		m_pEdtBeiZhu->setVisible(false);
		m_pSpriteBeiZhu->setVisible(false);
		m_pLabelBiaoJi->setVisible(false);
		m_pSpriteBiaoJi->setVisible(false);
		m_pMenuItemBiaoJi->setVisible(false);
		m_pMenuItemPingBi->setVisible(false);
		m_pMenuItemQuXiaoPingBi->setVisible(false);
		m_pMenuItemZhuBoDangAn->setVisible(false);
		SetLabelString(m_pLabelLookOnTip, bMySelf ? "旁观时不可以发表情" : "旁观时不能使用魔法表情");
		m_pLabelLookOnTip->setVisible(true);
		//return ;
	}



	
	spriteLine->setVisible(bMySelf);
	spriteLineSmall->setVisible(!bMySelf);

	if (!bMySelf)
	{
		m_pMenuItemPushPlayer->setVisible(bOwner) ;
	}
	if(bMySelf)
	{
		m_pMenuItemPushPlayer->setVisible(false) ;
	}



	setShowDataLoading(false) ;
}
void UserInfoView::setShowDataLoading(bool flag)
{
	if (s_dataLoading == nullptr)
	{

		s_dataLoading = Tools::creatDataLoading(this,367, 253,99) ;
	}
	s_dataLoading->setVisible(flag) ;

}
void UserInfoView::SetToolPrice(double dToolPrice)
{
	char szTxt[128] = {0};
	sprintf(szTxt, "魔法表情使用一次收取%s德州币", FormatCash(dToolPrice).c_str());
	SetLabelString(m_pLabelToolPrice, szTxt);
}

//void UserInfoView::AppandExpress(unsigned uType, unsigned uIdx)
//{
//	int iExp = -1;
//	unsigned uIdxAll = uType * 16 + uIdx;
//	for (unsigned i = 0; i < m_deqExpress.size(); i++)
//	{
//		if (m_deqExpress[i] == uIdxAll)
//		{
//			iExp = i;
//			break;
//		}
//	}
//	
//	unsigned uTools = 7;
//	float fXSpace = 99;
//	float fX = 57 - 15;
//	float fAllW = (uTools - 1) * fXSpace + fX * 2;
//	float fH = 100;
//
//	if (iExp == -1)
//	{
//		m_deqExpress.push_front(uIdxAll);
//		auto sz = m_pScrollViewQuickExpress->getContentSize();
//		float fAllW = (m_deqExpress.size() - 1) * fXSpace + fX * 2;
//		if (fAllW > sz.width)
//		{
//			m_pScrollViewQuickExpress->setContentSize(Size(fAllW, sz.height));
//		}
//		auto pSprite = CreateNode3AndPosAddTo<FrameSprite>(uType == 0 ? "express/allexpress.png" : "express/allexpressnoble.png", 4, 4, fX + (m_deqExpress.size() - 1) * fXSpace, 50, m_pScrollViewQuickExpress, 0);
//		m_vecQuickExpress.push_back(pSprite);
//		
//		auto bt = LayerButton::create(false, true);
//		bt->setContentSize(pSprite->getContentSize());
//		bt->setPosition(pSprite->getPosition());
//		m_pScrollViewQuickExpress->addChild(bt);
//		bt->SetHitCB([uType, uIdx](Ref*){
//			Json::Value param;
//			param["expressidx"] = uIdx;
//			param["expresstype"] = uType;
//			SendClientRequest(ClientRequest::kPublicExpress, param);
//		});
//		//DTouchHitProcer hitProcer(std::bind(&IsTouchMoving, std::placeholders::_1, 16), [](bool bselect, Node*node, TouchGrabber*){}, [](bool bselect, Node*node, TouchGrabber*){});
//		//auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(pSprite, false, hitProcer);
//		//touchGrabber->retain();
//		//touchGrabber->SetTouchEnable(m_pScrollViewQuickExpress->isVisible());
//		//m_vecExpressTouchGrabber.push_back(touchGrabber);
//	}
//	else
//	{
//		m_deqExpress.erase(m_deqExpress.begin() + iExp);
//		m_deqExpress.push_front(uIdxAll);
//	}
//	for (int i = 0; i < m_vecQuickExpress.size(); ++i)
//	{
//		m_vecQuickExpress[i]->SetFrameIdx(m_deqExpress[i] % 16);
//	}
//}

void UserInfoView::SetGuanZhuResult(unsigned uTUID, int iGuanZhuType, int iRet)
{
	if (iRet == 1)
	{
		if (m_UsrInfo.m_uUID == uTUID)
		{
			if (iGuanZhuType == 1)
			{
				m_pMenuItemQuXiaoGuanZhu->setVisible(true);
				m_pMenuItemGuanZhu->setVisible(false);
				m_UsrInfo.m_bYiGuanZhu = true;
			}
			else
			{
				m_pMenuItemQuXiaoGuanZhu->setVisible(false);
				m_pMenuItemGuanZhu->setVisible(true);
				m_UsrInfo.m_bYiGuanZhu = false;
			}
			setVisible(false);
		}
	}	
}

void UserInfoView::SetPingBiResult(unsigned uTUID, int iPingBiType, int iRet)
{
	if (iRet == 1)
	{
		if (m_UsrInfo.m_uUID == uTUID)
		{
			if (iPingBiType == 1)
			{
				m_pMenuItemQuXiaoPingBi->setVisible(true);
				m_pMenuItemPingBi->setVisible(false);
				m_UsrInfo.m_bYiPingBi = true;
			}
			else
			{
				m_pMenuItemQuXiaoPingBi->setVisible(false);
				m_pMenuItemPingBi->setVisible(true);
				m_UsrInfo.m_bYiPingBi = false;
			}
		}
	}
}

void UserInfoView::SetBiaoJiOrBeiZhuResult(unsigned uTUID, const std::string& strBeiZhu, int iBiaoJi, const std::string& strBiaoJi)
{
	if (m_UsrInfo.m_uUID == uTUID)
	{
		if (strBeiZhu.length()==0)
		{
			m_pSpriteBeiZhu->setVisible(true);
			m_pEdtBeiZhu->setText("");
		}
		else
		{
			m_pSpriteBeiZhu->setVisible(false);
			m_pEdtBeiZhu->setText(strBeiZhu.c_str());
		}
		m_pSpriteBiaoJi->SetFrameIdx(iBiaoJi);
		m_UsrInfo.m_strBeiZhu = strBeiZhu;
		m_UsrInfo.m_iBiaoJi = iBiaoJi;
		m_UsrInfo.m_strBiaoJi = strBiaoJi;
	}
}
void UserInfoView::initData()
{

	m_pMenuItemGetVIP->setVisible(false) ;

	SetLabelStringWithBoundWidth(m_pLabelName, "", 170);
	 m_pIcon->ResetToLocalSprite("defaulticon.png");

	 m_pMenuItemAt->setVisible(false);
	 SetLabelString(m_pMenuItemAt->GetLabel(), "");
	SetLabelString(m_pLabelLeBi, "0");
	SetLabelString(m_pLabelUID, "");
    SetLabelString(m_pLabelDiamond, "0");



	// 根据玩家的俱乐部等级来显示俱乐部的按钮颜色
	for(int i = 0;i < 4; i++)
	{
		m_pMenuItemFamilyButton[i]->setVisible(false);
		m_pMenuItemFamilyLabel[i]->setVisible(false);
	}

	m_pSpriteVIPDengJi->setVisible(false);

	for (auto item : m_pLabelVIP)
	{
		item->setTextColor(Color4B(0x48, 0x48, 0x48, 0xff));
	}

	m_pSpriteVIPDengJi->setVisible(false);

	m_pLabelGetVIP->setVisible(false);
	m_pMenuItemGetVIP->setVisible(false);

	SetLabelString(m_pLabelJiJinDu, "");
	SetLabelString(m_pLabelFanQianJiaZhu, "");
	SetLabelString(m_pLabelZaiJiaZhu, "");
	SetLabelString(m_pLabelChiXuXiaZhu, "");
	m_pSpriteVIPDengJi->setVisible(false);





	SetLabelString(m_pLabelMeiLi,  "");


	SetLabelString(m_pLabelJingYanDengJi1,  "");


	SetLabelString(m_pLabelJingYanDengJi2,  "");

	m_pProgressJingYanDengJi->setPercentage(0);


	SetLabelString(m_pLabelPaiJu,  "");


	SetLabelString(m_pLabelRuJuLv,  "");


	SetLabelString(m_pLabelShengLv,  "");


	SetLabelString(m_pLabelTanPaiLv,  "");



	m_pSpriteBiaoJi->setVisible(false);


	m_pMenuItemGuanZhu->setVisible(false);
	m_pMenuItemQuXiaoGuanZhu->setVisible(false);

	m_pMenuItemPingBi->setVisible(false);
	m_pMenuItemQuXiaoPingBi->setVisible(false);

	m_pMenuItemZhuBoDangAn->setVisible(false);
	m_pSpriteBeiZhu->setVisible(false);


	ShowTool(false);
	ShowExpress(false);

	m_pLabelLookOnTip->setVisible(false);

	m_pLabelBeiZhu->setVisible(false);
	m_pLabelBiaoJi->setVisible(false);
	m_pEdtBeiZhu->setVisible(false);
	m_pMenuItemBiaoJi->setVisible(false);

	ShowTool(false);
	ShowExpress(false);

	m_pMenuItemGuanZhu->setVisible(false);
	m_pMenuItemQuXiaoGuanZhu->setVisible(false);
	m_pLabelBeiZhu->setVisible(false);
	m_pEdtBeiZhu->setVisible(false);
	m_pSpriteBeiZhu->setVisible(false);
	m_pLabelBiaoJi->setVisible(false);
	m_pSpriteBiaoJi->setVisible(false);
	m_pMenuItemBiaoJi->setVisible(false);
	m_pMenuItemPingBi->setVisible(false);
	m_pMenuItemQuXiaoPingBi->setVisible(false);
	m_pMenuItemZhuBoDangAn->setVisible(false);
	SetLabelString(m_pLabelLookOnTip,"");
	m_pLabelLookOnTip->setVisible(false);
	//setShowDataLoading(false) ;



}
bool UserInfoView::init()
{
	if (!GameDialog::init("family/informationbg.png", false))
	{
		return false;
	}
	m_hasMoveUp = false ;
	s_dataLoading =nullptr ;
	spriteLine = CreateNode1AndPosAddTo<Sprite>("userinfo/line.png", 343, 111, this, 0);

	spriteLineSmall =  CreateNode1AndPosAddTo<Sprite>("userinfo/linesmall.png", 340+100, 111, this, 0);
	//auto bg = Sprite::create("userinfo/bg.png");
	//auto sz = bg->getContentSize();
	//setContentSize(sz);
	//bg->setPosition(sz.width / 2, sz.height / 2);
	//addChild(bg);

	//auto pClipMask = Sprite::create("userinfo/iconclipmask.png");
	//pClipMask->setPosition(66, 66);
	//pClipMask->retain();

	//auto pClip = CreateNodeAndPosAddTo<ClippingNode>(21, 282, this, 0);
	//pClip->setContentSize(Size(132, 132));

	//m_pIcon = CreateNode2AndPosAddTo<HttpSprite>("defaulticon.png", Size(132, 132), 0, 0, pClip, 0);
	//CreateNode1AndPosAddTo<Sprite>("userinfo/iconround.png", 21 + 66, 282 + 66, this, 0);
	//pClip->setStencil(pClipMask);
	//pClip->setAlphaThreshold(0.5);
	m_pIcon = CreateUserHeadSprite("defaulticon.png", "userinfo/iconclipmask.png", this, Vec2(21, 312), "userinfo/iconround.png");

	m_pSpriteSex = CreateNode3AndPosAddTo<FrameSprite>("userinfo/sex.png", 2, 1, 225, 443, this, 0);
	m_pLabelName = CreateLabelMSYHAndAnchorPosClrAddTo(22, "昵称", Vec2::ZERO, 244, 433, Color4B::WHITE, this, 0);
	m_pSpriteVIPDengJi = CreateNode3AndPosAddTo<FrameSprite>("vip.png", 6, 1, 55, 451, this, 0);

	m_pLabelBeiZhu = CreateLabelMSYHAndAnchorPosClrAddTo(18, "备注:", Vec2::ZERO, 525, 351, Color4B::WHITE, this, 0);
	m_pEdtBeiZhu = CreateNode2AndPosAddTo<ui::EditBox>(Size(90, 20), ui::Scale9Sprite::create("userinfo/bgedit.png") , 612, 363, this, 0);
	m_pEdtBeiZhu->setFont("fzlt.ttf", 18);
    m_pEdtBeiZhu->setReturnType(ui::EditBox::KeyboardReturnType::DONE);

	m_pEdtBeiZhu->setDelegate(this);
	m_pSpriteBeiZhu = CreateNode3AndPosAddTo<FrameSprite>("userinfo/btinputable.png", 1, 2, 590, 363, this, 0);
	//m_pMenuItemBeiZhu = MenuItemSpriteWithLabel::create("userinfo/btinputable.png", 2, CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	//m_pMenuItemBeiZhu->setPosition(502, 382);

	CreateNode1AndPosAddTo<Sprite>("userinfo/bgprogress.png", 365, 325, this, 0);
	m_pProgressJingYanDengJi = CreateNode1AndPosAddTo<ProgressTimer>(Sprite::create("userinfo/progress.png"), 365, 325, this, 0);
	m_pProgressJingYanDengJi->setType(ProgressTimer::Type::BAR);
    m_pProgressJingYanDengJi->setMidpoint(Vec2(0, 0.5));
    m_pProgressJingYanDengJi->setBarChangeRate(Vec2(1, 0));
    auto progressSz = m_pProgressJingYanDengJi->getContentSize();
	m_pLabelJingYanDengJi1 = CreateLabelMSYHAndAnchorPosClrAddTo(18, "lv0", Vec2::ZERO, 211, 312, CLR_DETAIL_YELLOW, this, 0);
	m_pLabelJingYanDengJi2 = CreateLabelMSYHAndAnchorPosClrAddTo(18, "0/25", Vec2(0.5, 0.5), progressSz.width/2, progressSz.height/2, Color4B::WHITE, m_pProgressJingYanDengJi, 0);
	
	m_pLabelBiaoJi = CreateLabelMSYHAndAnchorPosClrAddTo(18, "标记:", Vec2::ZERO, 525, 312, Color4B::WHITE, this, 0);
	m_pSpriteBiaoJi = CreateNode3AndPosAddTo<FrameSprite>("userinfo/playericons.png", 8, 1, 630, 320, this, 0);
	m_pMenuItemBiaoJi = MenuItemSpriteWithLabel::create("userinfo/btinputable.png", 2, CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
    m_pMenuItemBiaoJi->ignoreAnchorPointForPosition(false);
    m_pMenuItemBiaoJi->setAnchorPoint(Vec2(0.5,0.5));
	m_pMenuItemBiaoJi->setPosition(590, 325);


	////德州币上的动画
    auto spriteCash = CreateNode1AndPosAddTo<Sprite>("userinfo/lebi.png", 224, 394, this, 0);
    auto shineCash = CreateNode1AndPosAddTo<Sprite>("hall/moneyshine.png",29, 15, spriteCash, 0);
    shineCash->setScale(0);
    auto action1 = Sequence::create(ScaleTo::create(0.3, 1.2, 1.2, 0), DelayTime::create(0.3),ScaleTo::create(0.3, 0,0,0), DelayTime::create(1.5), nullptr);
    auto action2 = RepeatForever::create(Spawn::create(RotateBy::create(1.5f, Vec3(0,0,150)), action1, nullptr));
    shineCash->runAction(action2);
    //钻石上的动画
    m_pSpriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", 224, 361, this, 0);
    m_pSpriteDiamond->setScale(0.8);
    auto shineDiamond = CreateNode1AndPosAddTo<Sprite>("hall/moneyshine.png",39, 21, m_pSpriteDiamond, 0);
    shineDiamond->setScale(0);
    auto action3 = Sequence::create(ScaleTo::create(0.3, 1.2, 1.2, 0), DelayTime::create(0.3),ScaleTo::create(0.3, 0,0,0), DelayTime::create(1.5), nullptr);
    auto action4 = RepeatForever::create(Spawn::create(RotateBy::create(1.5f, Vec3(0,0,150)), action3, nullptr));
    shineDiamond->runAction(action4);

	m_pLabelLeBi = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2(0, 0.5), 252, 394, CLR_DETAIL_YELLOW, this, 0);
	m_pLabelLeBi->setHorizontalAlignment(TextHAlignment::LEFT);
	m_pLabelLeBi->setWidth(150);
    m_pLabelDiamond = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2(0, 0.5), 252, 361, CLR_DETAIL_YELLOW, this, 0);
    m_pLabelDiamond->setHorizontalAlignment(TextHAlignment::LEFT);
    m_pLabelDiamond->setWidth(150);

	CreateLabelMSYHAndAnchorPosClrAddTo(22, "幸运值:", Vec2::ZERO, 525, 383, CLR_DETAIL_YELLOW, this, 0);
	m_pLabelMeiLi = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 603, 383, CLR_DETAIL_YELLOW, this, 0);
	m_pLabelMeiLi->setHorizontalAlignment(TextHAlignment::LEFT);
	m_pLabelMeiLi->setWidth(150);

	CreateLabelMSYHAndAnchorPosClrAddTo(18, "ID:", Vec2::ZERO, 525, 434, Color4B(0x7f,0x7f,0x7f,0xff), this, 0);
	m_pLabelUID = CreateLabelMSYHAndAnchorPosClrAddTo(18, "0", Vec2::ZERO, 550, 434, Color4B(0x7f,0x7f,0x7f,0xff), this, 0);
	m_pLabelUID->setHorizontalAlignment(TextHAlignment::LEFT);
	m_pLabelUID->setWidth(150);

	CreateLabelMSYHAndAnchorPosClrAddTo(22, "牌   局:", Vec2::ZERO, 211, 271, Color4B::WHITE, this, 0);
	m_pLabelPaiJu = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 299, 271, Color4B::WHITE, this, 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(22, "入 局 率:", Vec2::ZERO, 459, 271, Color4B::WHITE, this, 0);
	m_pLabelRuJuLv = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 569, 271, Color4B::WHITE, this, 0);

	CreateLabelMSYHAndAnchorPosClrAddTo(22, "胜   率:", Vec2::ZERO, 211, 229, Color4B::WHITE, this, 0);
	m_pLabelShengLv = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 299, 229, Color4B::WHITE, this, 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(22, "摊 牌 率:", Vec2::ZERO, 459, 229, Color4B::WHITE, this, 0);
	m_pLabelTanPaiLv = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 569, 229, Color4B::WHITE, this, 0);

	m_pLabelVIP[0] = CreateLabelMSYHAndAnchorPosClrAddTo(22, "激进度:", Vec2::ZERO, 211, 187, Color4B(0x6d,0x6d,0x6d,0xff), this, 0);
	m_pLabelJiJinDu = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 299, 187, Color4B::WHITE, this, 0);
	m_pLabelGetVIP = CreateLabelMSYHAndAnchorPosClrAddTo(22, "VIP数据", Vec2::ZERO, 295, 187, Color4B(0x90,0x5d,0x13,0xff), this, 0);
	m_pMenuItemGetVIP = MenuItemSpriteWithLabel::create("userinfo/btgetvip.png", 2, CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemGetVIP->setPosition(392, 202);

	m_pLabelVIP[1] = CreateLabelMSYHAndAnchorPosClrAddTo(22, "翻前加注:", Vec2::ZERO, 459, 187, Color4B(0x6d,0x6d,0x6d,0xff), this, 0);
	m_pLabelFanQianJiaZhu = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 569, 187, Color4B::WHITE, this, 0);

	m_pLabelVIP[2] = CreateLabelMSYHAndAnchorPosClrAddTo(22, "再加注:", Vec2::ZERO, 211, 145, Color4B(0x6d,0x6d,0x6d,0xff), this, 0);
	m_pLabelZaiJiaZhu = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 299, 145, Color4B::WHITE, this, 0);

	m_pLabelVIP[3] = CreateLabelMSYHAndAnchorPosClrAddTo(22, "持续加注:", Vec2::ZERO, 459, 145, Color4B(0x6d,0x6d,0x6d,0xff), this, 0);
	m_pLabelChiXuXiaZhu = CreateLabelMSYHAndAnchorPosClrAddTo(22, "0", Vec2::ZERO, 569, 145, Color4B::WHITE, this, 0);

	m_pLabelToolPrice = CreateLabelMSYHAndAnchorPosClrAddTo(18, "魔法表情使用一次收取 德州币哦", Vec2(0.5, 0.5), 345+147-50, 112, Color4B(0x70, 0x53, 0xf, 255), this, 0);
	m_pLabelQuickExpress = CreateLabelMSYHAndAnchorPosClrAddTo(18, "最近使用过的表情", Vec2(0.5, 0.5), 345, 112, Color4B(0x70, 0x53, 0xf, 255), this, 0);
	m_pLabelLookOnTip = CreateLabelMSYHAndAnchorPosClrAddTo(18, "", Vec2(0.5, 0.5), 345, 112, Color4B(0x70, 0x53, 0xf, 255), this, 0);
	m_pScrollViewTool = CreateNode1AndPosAddTo<extension::ScrollView>(Size(480, 100), 205, 5, this, 0);
	unsigned uTools = 12;
	float fXSpace = 99;
	float fX = 57 - 15;
	float fAllW = (uTools - 1) * fXSpace + fX * 2;
	float fH = 100;
	m_pScrollViewTool->setContentSize(Size(fAllW, fH));
	m_pScrollViewTool->setDirection(extension::ScrollView::Direction::HORIZONTAL);

	const char *szTools[13] = {
		"userinfo/tool0.png","userinfo/tool1.png","userinfo/tool2.png",
		"userinfo/tool3.png","userinfo/tool4.png","userinfo/tool5.png",
		"userinfo/tool6.png",/*"userinfo/tool12.png",*/
		
		"userinfo/tool7.png","userinfo/tool8.png",
		"userinfo/tool9.png","userinfo/tool10.png","userinfo/tool11.png"
	};
	//unsigned uToolFlag[13] = {1, 0, 2, 2, 2, 2, 1,2, 3, 3, 3, 3, 3};
	unsigned uToolFlag[12] = {1, 0, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3};
	for (int i = 0; i < uTools; i++)
	{
		//auto bgtool = CreateNode3AndPosAddTo<FrameSprite>(szBgTool[uToolNobel[i]], 1, 2, fX + i * fXSpace, 50, m_pScrollViewTool, 0);
		auto bt = FrameSpriteButton::createWithVertFrameSprite("userinfo/bgtoolcommon.png", false, true);
		bt->SetHitCB([this, i](Ref*){
			bool flag = false ;
			unsigned uToolNobel[13] = {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
			//如果使用魔法表情  判定可使用情况
			if(uToolNobel[i] == 1 )
			{
				if ( (g_globalMyData.GetVipLevel() > 1))
				{
					flag = true ;
				}
				if (g_globalMyData.m_useMagicExpress)
				{
					flag = true ;
				}
			}
			else
			{
				flag = true ;
			}

			if (flag)
			{
				Json::Value param;
				param["magicID"] = i;
				param["sendUser"] = g_globalMyData.m_iUserId;
				param["rcvUser"] = m_UsrInfo.m_uUID;
				SendClientRequest(ClientRequest::kSendTool, param);
				setVisible(false);
				g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskMagic);
			}
			else
			{
				SendClientRequest(ClientRequest::kSendVipMessage); //提示需要会员或购买哦表情
			}


		
		});
		bt->setPosition(fX + i * fXSpace, 50);
		m_pScrollViewTool->addChild(bt);
		auto tool = CreateNode1AndPosAddTo<Sprite>(szTools[i], fX + i * fXSpace, 50, m_pScrollViewTool, 0);
		auto btSpriteBg = CreateNode3AndPosAddTo<FrameSprite>("userinfo/bgtoolnoble.png", 4, 1, fX + i * fXSpace, 50, m_pScrollViewTool, 0);
		btSpriteBg->SetFrameIdx(uToolFlag[i]);
	

	}
	if(g_globalMyData.GetVipLevel() >= 1)
	{
		m_pScrollViewTool->setContentOffset(Vec2(-200, 0), false);
	}
	else
	{
		m_pScrollViewTool->setContentOffset(Vec2(-504, 0), false);
	}

	m_pScrollViewQuickExpress = CreateNode1AndPosAddTo<extension::ScrollView>(Size(657, 100), 15, 0, this, 0);
	m_pScrollViewQuickExpress->setContentSize(Size(657, 100));
	m_pScrollViewQuickExpress->setDirection(extension::ScrollView::Direction::HORIZONTAL);

	m_pMenuItemAt = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemAt->setPosition(99, 285);
	auto label = CreateLabelMSYHBD(22, "@Ta");
	m_pMenuItemAt->SetLabel(label);
	
	m_pMenuItemGuanZhu = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemGuanZhu->setPosition(99, 236-7);
	label = CreateLabelMSYHBD(22, "加关注");
	m_pMenuItemGuanZhu->SetLabel(label);
	m_pMenuItemQuXiaoGuanZhu = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemQuXiaoGuanZhu->setPosition(99, 236-7);
	label = CreateLabelMSYHBD(22, "取消关注");
	m_pMenuItemQuXiaoGuanZhu->SetLabel(label);

	m_pMenuItemPingBi = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemPingBi->setPosition(99, 188-14);
	label = CreateLabelMSYHBD(22, "屏蔽聊天");
	m_pMenuItemPingBi->SetLabel(label);
	m_pMenuItemQuXiaoPingBi = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemQuXiaoPingBi->setPosition(99, 188-14);
	label = CreateLabelMSYHBD(22, "取消屏蔽");
	m_pMenuItemQuXiaoPingBi->SetLabel(label);

		for(int i = 0;i < 4; i++)
		{
			char *szButton[128] = {"userinfo/userinfor_familybg0.png", "userinfo/userinfor_familybg1.png",
				"userinfo/userinfor_familybg2.png", "userinfo/userinfor_familybg3.png"};
			m_pMenuItemFamilyButton[i] = MenuItemSpriteWithLabel::create(szButton[i], 3, CC_CALLBACK_1(UserInfoView::onMenuCallBackPaijuFamily, this));
			m_pMenuItemFamilyButton[i]->setScale(0.6);
			m_pMenuItemFamilyButton[i]->setPosition(460, 445);
			m_pMenuItemFamilyButton[i]->setVisible(false);
			m_pMenuItemFamilyLabel[i] = CreateLabelMSYHBD(26, "");
			m_pMenuItemFamilyButton[i]->SetLabel(m_pMenuItemFamilyLabel[i]);
		}




	m_pMenuItemZhuBoDangAn = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemZhuBoDangAn->setPosition(99, 139-21);
	label = CreateLabelMSYHBD(22, "投诉");
	m_pMenuItemZhuBoDangAn->SetLabel(label);

	m_pMenuItemPushPlayer = MenuItemScale9SpriteWithLabel::create(1, Size(154, 54), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
	m_pMenuItemPushPlayer->setPosition(99, 91-28);
	label = CreateLabelMSYHBD(22, "请他离开");
	m_pMenuItemPushPlayer->SetLabel(label);


	auto pMenu = Menu::create(/*m_pMenuItemBeiZhu, */m_pMenuItemBiaoJi, m_pMenuItemGetVIP, 
		m_pMenuItemAt, m_pMenuItemGuanZhu, m_pMenuItemQuXiaoGuanZhu, m_pMenuItemPingBi,m_pMenuItemFamilyButton[0],m_pMenuItemFamilyButton[1],
		m_pMenuItemFamilyButton[2],m_pMenuItemFamilyButton[3],m_pMenuItemQuXiaoPingBi, m_pMenuItemZhuBoDangAn,m_pMenuItemPushPlayer, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu);
	m_uFamilyID = 0;
	m_pDlgPingBi = nullptr;
	m_pDlgPingBiShiBai = nullptr;
	m_pDlgKaiTongVIP = nullptr;
	m_pDlgBiaoJi = nullptr;
	m_pSpriteBgTouSu = nullptr;

	initData() ;
	return true;
}

void UserInfoView::onGrabTouchBegan(bool bHitDialog)
{
	if (m_hasMoveUp)
	{
		
	}
	else
	{
			GameDialog::onGrabTouchBegan(bHitDialog) ;
	}

}


void UserInfoView::setMoveUp(bool flag)
{
	m_hasMoveUp = flag ;
}

void UserInfoView::onSetVisible(bool bVisible)
{
	PlayVisibleSound(bVisible);
	if (!bVisible)
	{
		if (m_pDlgPingBi)
		{
			m_pDlgPingBi->setVisible(false);
		}
		if (m_pDlgPingBiShiBai)
		{
			m_pDlgPingBiShiBai->setVisible(false);
		}
		if (m_pDlgKaiTongVIP)
		{
			m_pDlgKaiTongVIP->setVisible(false);
		}
		if (m_pDlgBiaoJi)
		{
			m_pDlgBiaoJi->setVisible(false);
		}
		if (m_pSpriteBgTouSu)
		{
			m_pSpriteBgTouSu->setVisible(false);
		}
		initData() ;
	}
	else
	{
	}
	setShowDataLoading(bVisible) ;
}

void UserInfoView::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	const char *pInput = editBox->getText();
	if (pInput && strlen(pInput))
	{
		m_pSpriteBeiZhu->setVisible(false);
	}
	else
	{
		m_pSpriteBeiZhu->setVisible(true);
	}
	Json::Value param;
	param["type"] = 4;
	param["tuid"] = m_UsrInfo.m_uUID;
	Json::Value data;
	data["tipDes"] = m_UsrInfo.m_strBiaoJi;
	data["tipId"] = m_UsrInfo.m_iBiaoJi;
	data["remarkDes"] = pInput;
	param["data"] = data;
	SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
}

void UserInfoView::onMenuItemSelected(cocos2d::Ref *pItem)
{
	if (pItem == m_pMenuItemPushPlayer)
	{
		Json::Value param;
		param["uid"] = m_UsrInfo.m_uUID;
		SendClientRequest(ClientRequest::kCallPrivateRoomPushPlayer, param);
	}
	else if (pItem == m_pMenuItemPingBi)
	{
		if (m_pDlgPingBi == nullptr)
		{
			auto sz = getContentSize();
			m_pDlgPingBi = CreateNode2AndPosAddTo<GameDialog>("userinfo/bgdialog0.png", false, 5,5, this, 0);
			m_pLabelPingBiConfirm = CreateLabelMSYHAndAnchorPosClrAddTo(30, "您确定要屏蔽该玩家的消息吗？", Vec2(0.5, 0.5), 339, 300, Color4B(0xff, 0xd2, 0x00, 0xff), m_pDlgPingBi, 0);
			m_pLabelPingBiDescribe = CreateLabelMSYHAndAnchorPosClrAddTo(26, "屏蔽消息后，将不再对您显示该玩家的聊天信息。", Vec2(0, 1), 102, 260, Color4B::WHITE, m_pDlgPingBi, 0);
			m_pLabelPingBiDescribe->setLineBreakWithoutSpace(true);
			m_pLabelPingBiDescribe->setMaxLineWidth(480);
			m_pLabelPingBiDescribe->setHorizontalAlignment(TextHAlignment::LEFT);
			m_MenuItemPingBiOk = MenuItemScale9SpriteWithLabel::create(0, Size(203, 70), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
			m_MenuItemPingBiOk->SetLabel(CreateLabelMSYHBD(30, "确定"));
			m_MenuItemPingBiOk->setPosition(220, 65);
			m_MenuItemPingBiCancel = MenuItemScale9SpriteWithLabel::create(0, Size(203, 70), CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
			m_MenuItemPingBiCancel->SetLabel(CreateLabelMSYHBD(30, "取消"));
			m_MenuItemPingBiCancel->setPosition(445, 65);
			m_pMenuDialogPingBi = Menu::create(m_MenuItemPingBiOk, m_MenuItemPingBiCancel, nullptr);
			m_pMenuDialogPingBi->setPosition(Vec2::ZERO);
			m_pDlgPingBi->addChild(m_pMenuDialogPingBi);
		}
		m_pDlgPingBi->setVisible(true);
	}
	else if (pItem == m_pMenuItemQuXiaoPingBi)
	{
		Json::Value param;
		param["type"] = 2;
		param["data"] = 0;
		param["tuid"] = m_UsrInfo.m_uUID;
		SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
	}
	else if (m_MenuItemPingBiCancel == pItem)
	{
		m_pDlgPingBi->setVisible(false);
	}
	else if (m_MenuItemPingBiOk == pItem)
	{
		//if (m_pDlgPingBiShiBai == nullptr)
		//{
		//	auto sz = getContentSize();
		//	m_pDlgPingBiShiBai = CreateNode2AndPosAddTo<GameDialog>("userinfo/bgdialog0.png", false, 0, 53, this, 0);
		//	m_pLabelPingBiShiBaiConfirm = CreateLabelMSYHAndAnchorPosClrAddTo(30, "您无法屏蔽该玩家消息哦！", Vec2(0.5, 0.5), 279, 250, Color4B(0xff, 0xd2, 0x00, 0xff), m_pDlgPingBiShiBai, 0);
		//	m_pLabelPingBiShiBaiDescribe = CreateLabelMSYHAndAnchorPosClrAddTo(26, "对方逼您的VIP等级高，要想屏蔽TA的消息，点击下方按钮开通更高VIP。", Vec2(0, 1), 43, 198, Color4B::WHITE, m_pDlgPingBiShiBai, 0);
		//	m_pLabelPingBiShiBaiDescribe->setLineBreakWithoutSpace(true);
		//	m_pLabelPingBiShiBaiDescribe->setMaxLineWidth(480);
		//	m_pLabelPingBiShiBaiDescribe->setHorizontalAlignment(TextHAlignment::LEFT);
		//	m_MenuItemKaiTongVIP1 = MenuItemSpriteWithLabel::create("userinfo/btred.png", 3, CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
		//	m_MenuItemKaiTongVIP1->SetLabel(CreateLabelMSYHBD(30, "开通VIP"));
		//	m_MenuItemKaiTongVIP1->setPosition(sz.width / 2, 35);
		//	m_pMenuDialogPingBiShiBai = Menu::create(m_MenuItemKaiTongVIP1, nullptr);
		//	m_pMenuDialogPingBiShiBai->setPosition(Vec2::ZERO);
		//	m_pDlgPingBiShiBai->addChild(m_pMenuDialogPingBiShiBai);
		//}
		//m_pDlgPingBiShiBai->setVisible(true);
		m_pMenuItemQuXiaoGuanZhu->setEnabled(true);
		m_pDlgPingBi->setVisible(false);
		Json::Value param;
		param["type"] = 2;
		param["data"] = 1;
		param["tuid"] = m_UsrInfo.m_uUID;
		SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
	}
	else if (pItem == m_pMenuItemGuanZhu)
	{
		Json::Value param;
		param["type"] = 1;
		param["data"] = 1;
		param["tuid"] = m_UsrInfo.m_uUID;
		SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
        g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskFllow);
	}
    else if (pItem == m_pMenuItemAt)
    {
        SendClientRequest(ClientRequest::kClickChatBox);
    }
	else if (pItem == m_pMenuItemQuXiaoGuanZhu)
	{
		Json::Value param;
		param["type"] = 1;
		param["data"] = 0;
		param["tuid"] = m_UsrInfo.m_uUID;
		SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
	}
	else if (pItem == m_pMenuItemZhuBoDangAn)
	{
		if (m_pSpriteBgTouSu == nullptr)
		{
			m_pSpriteBgTouSu = GameDialog::create("userinfo/bgtousu.png", false);
			m_pSpriteBgTouSu->setPosition(Vec2(3, 3/*275, 60*/)); // TODO
			m_pSpriteBgTouSu->setVisible(false);
			char *szBt[4] = {"userinfo/bttousu0.png", "userinfo/bttousu1.png",
				"userinfo/bttousu2.png", "userinfo/bttousu3.png"};
			auto bg = m_pSpriteBgTouSu;
			auto uID = m_UsrInfo.m_uUID;
			MenuItemFrameSprite *pBtTouSu[4];
			for (int i = 0; i < 4; i++)
			{
				auto menuitem = MenuItemFrameSprite::create(szBt[i], 2, [uID, bg, i](cocos2d::Ref*){
					Json::Value param;
					param["id"] = i;
					param["uid"] = uID;
					SendClientRequest(ClientRequest::kTouSu, param);
					bg->setVisible(false);
				});
				menuitem->setPosition(Vec2(105 + i * 159, 62)); // TODO
				pBtTouSu[i] = menuitem;
			}
			auto menu = Menu::create(pBtTouSu[0], pBtTouSu[1], pBtTouSu[2], pBtTouSu[3], nullptr);
			menu->setPosition(Vec2::ZERO);
			m_pSpriteBgTouSu->addChild(menu);
			addChild(m_pSpriteBgTouSu);
		}
		m_pSpriteBgTouSu->setVisible(true);
	}
	else if (m_MenuItemKaiTongVIP1 == pItem)
	{

	}
	else if (m_pMenuItemGetVIP == pItem)
	{
		Json::Value param;
		param["idx"] = 1;
		SendClientRequest(ClientRequest::kPopMall, param);
		//if (m_pDlgKaiTongVIP == nullptr)
		//{
		//	auto sz = getContentSize();
		//	m_pDlgKaiTongVIP = CreateNode2AndPosAddTo<GameDialog>("userinfo/bgdialog0.png", false, 0, 53, this, 0);
		//	m_pLabelKaiTongVIPDescribe = CreateLabelMSYHAndAnchorPosClrAddTo(26, "开通VIP就可以获取专属数据哦！", Vec2(0.5, 0.5), sz.width / 2, 195, Color4B::WHITE, m_pDlgKaiTongVIP, 0);
		//	//m_pLabelPingBiShiBaiDescribe->setLineBreakWithoutSpace(true);
		//	//m_pLabelPingBiShiBaiDescribe->setMaxLineWidth(480);
		//	//m_pLabelPingBiShiBaiDescribe->setHorizontalAlignment(TextHAlignment::LEFT);
		//	m_MenuItemKaiTongVIP2 = MenuItemSpriteWithLabel::create("userinfo/btred.png", 3, CC_CALLBACK_1(UserInfoView::onMenuItemSelected, this));
		//	m_MenuItemKaiTongVIP2->SetLabel(CreateLabelMSYHBD(30, "开通VIP"));
		//	m_MenuItemKaiTongVIP2->setPosition(sz.width / 2, 35);
		//	m_pMenuDialogKaiTongVIP = Menu::create(m_MenuItemKaiTongVIP2, nullptr);
		//	m_pMenuDialogKaiTongVIP->setPosition(Vec2::ZERO);
		//	m_pDlgKaiTongVIP->addChild(m_pMenuDialogKaiTongVIP);
		//}
		//m_pDlgKaiTongVIP->setVisible(true);
	}
	else if (pItem == m_pMenuItemBiaoJi)
	{
		if (m_pDlgBiaoJi == nullptr)
		{
			Size screenSize = Director::getInstance()->getOpenGLView()->getFrameSize();

			//m_pDlgBiaoJi = CreateNode2AndPosAddTo<GameDialog>("userinfo/bgdialogbiaoji.png", false, 4, 10, this, 0);

			m_pDlgBiaoJi = GameLayer::create("userinfo/bgdialogbiaoji.png",true, true);
			m_pDlgBiaoJi->setPosition(Vec2(4, 10));
			addChild(m_pDlgBiaoJi);


			//m_pTouchGrabberDialogBiaoJi = CreateTouchGrabberWithStaticProcer<STouchDialogProcer<true>>(m_pSpriteBgDialogBiaoJi, true);
			//m_pTouchGrabberDialogBiaoJi->retain();
			auto label = CreateLabelMSYHAndAnchorPosClrAddTo(24, "为TA选择一个图案", Vec2(0.5, 0.5), 329, 355, Color4B(0xff, 0xd2, 0x00, 0xff), m_pDlgBiaoJi, 0);
			label = CreateLabelMSYHAndAnchorPosClrAddTo(24, "对TA添加文字描述", Vec2(0.5, 0.5), 329, 215, Color4B(0xff, 0xd2, 0x00, 0xff), m_pDlgBiaoJi, 0);

			m_pEdtBiaoJi = CreateNode2AndPosAddTo<ui::EditBox>(Size(511 - 38, 90), ui::Scale9Sprite::create("userinfo/bgedit.png") , 106 + (511 - 38) / 2, 168, m_pDlgBiaoJi, 0); // ui::EditBox::create(124, );
			m_pEdtBiaoJi->setFont("fzlt.ttf", 24);
			m_pEdtBiaoJi->setFontColor(Color4B(0x81,0x2e,0x0a,0xff));
            m_pEdtBiaoJi->setReturnType(ui::EditBox::KeyboardReturnType::DONE);

			m_MenuItemBiaoJiOk = MenuItemScale9SpriteWithLabel::create(0, Size(203, 70), CC_CALLBACK_1(UserInfoView::onMenuItemBiaoJiSelected, this));
			label = CreateLabelMSYHBD(30, "确定");
			m_MenuItemBiaoJiOk->setPosition(220, 52);
			m_MenuItemBiaoJiOk->SetLabel(label);
			m_MenuItemBiaoJiCancel = MenuItemScale9SpriteWithLabel::create(0, Size(203, 70), CC_CALLBACK_1(UserInfoView::onMenuItemBiaoJiSelected, this));
			label = CreateLabelMSYHBD(30, "取消");
			m_MenuItemBiaoJiCancel->SetLabel(label);
			m_MenuItemBiaoJiCancel->setPosition(454, 52);

			const char *pMiaoShu[8] = {
				"未知型", "跟注站", "疯子", "紧凶型",
				"紧弱型", "岩石型", "黄色警报", "捉鸡",
			};
			for (int i = 0; i < 8; i++)
			{
				m_pSpriteBiaoJiItem[i] = CreateNode4AndPosAddTo<FrameSprite>("userinfo/playericons.png", 8, 1, i, 60 + i * 80, 303, m_pDlgBiaoJi, 0);
				m_MenuItemBiaoJiItem[i] = MenuItemSpriteWithLabel::create("userinfo/btselectbiaoji.png", 3, CC_CALLBACK_1(UserInfoView::onMenuItemBiaoJiSelected, this));
				m_MenuItemBiaoJiItem[i]->setPosition(60 + i * 80, 292);
				m_pLabelBiaoJiTuPianMiaoShu[i] = CreateLabelMSYHAndAnchorPosClrAddTo(20, pMiaoShu[i], Vec2(0.5, 0.5), 295, 335, Color4B(0x88, 0xe7, 0xb4, 0xff), m_pDlgBiaoJi, 1);
				m_pLabelBiaoJiTuPianMiaoShu[i]->setPosition(60 + i * 80, 264);
			}

			m_pMenuBiaoJi = Menu::create(m_MenuItemBiaoJiOk, m_MenuItemBiaoJiCancel,
				m_MenuItemBiaoJiItem[0], m_MenuItemBiaoJiItem[1], m_MenuItemBiaoJiItem[2], 
				m_MenuItemBiaoJiItem[3], m_MenuItemBiaoJiItem[4], m_MenuItemBiaoJiItem[5],
				m_MenuItemBiaoJiItem[6], m_MenuItemBiaoJiItem[7], nullptr);
			m_pMenuBiaoJi->setPosition(Vec2::ZERO);
			m_pDlgBiaoJi->addChild(m_pMenuBiaoJi);
		}
		m_pEdtBiaoJi->setText(m_UsrInfo.m_strBiaoJi.c_str());
		if(m_UsrInfo.m_iBiaoJi != -1)
		{
			m_pLabelBiaoJiTuPianMiaoShu[m_UsrInfo.m_iBiaoJi]->setTextColor(Color4B::BLACK);
		}	

		for (int i = 0; i < 8; i++)
		{
			m_MenuItemBiaoJiItem[i]->setEnabled(true);
		}
		if(m_UsrInfo.m_iBiaoJi != -1)
		{
			m_MenuItemBiaoJiItem[m_UsrInfo.m_iBiaoJi]->setEnabled(false);
		}
		m_pDlgBiaoJi->setVisible(true);
	}
}

void UserInfoView::onMenuCallBackPaijuFamily(cocos2d::Ref *pItem)
{
	//for(int i = 0; i < 4; i++)
	//{
	//	if(pItem == m_pMenuItemFamilyButton[i])
	//	{
	//		Json::Value param;
	//		param["familyID"] = m_uUserFamilyID;
	//		SendClientRequest(ClientRequest::kGetShowFamilyInfo, param);
	//		m_pMenuItemFamilyButton[i]->setVisible(true);
	//	}
	//	else
	//	{
	//		m_pMenuItemFamilyButton[i]->setVisible(false);
	//	}
	//}
}

void UserInfoView::onMenuItemBiaoJiSelected(cocos2d::Ref *pItem)
{
	if (pItem == m_MenuItemBiaoJiOk)
	{
		int iSel = 0;
		for (int i = 0; i < 8; i++)
		{
			if (!m_MenuItemBiaoJiItem[i]->isEnabled())
			{
				iSel = i;
				break;
			}
		}
		Json::Value param;
		param["type"] = 4;
		param["tuid"] = m_UsrInfo.m_uUID;
		Json::Value data;
		data["tipDes"] = m_pEdtBiaoJi->getText();
		data["tipId"] = iSel;
		data["remarkDes"] = m_UsrInfo.m_strBeiZhu;
		param["data"] = data;
		SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
		m_pSpriteBiaoJi->setVisible(true);
		m_pDlgBiaoJi->setVisible(false);
		m_pMenuItemBiaoJi->setPosition(590, 325);
	}
	else if (pItem == m_MenuItemBiaoJiCancel)
	{
		m_pDlgBiaoJi->setVisible(false);
	}
	else
	{
		int iNew = -1;
		int iOld = -1;
		for (int i = 0; i < 8; i++)
		{
			if (pItem == m_MenuItemBiaoJiItem[i])
			{
				iNew = i;
			}
			if (!m_MenuItemBiaoJiItem[i]->isEnabled())
			{
				iOld = i;
			}
		}
		if (iNew >= 0)
		{
			m_MenuItemBiaoJiItem[iNew]->setEnabled(false);
			m_pLabelBiaoJiTuPianMiaoShu[iNew]->setTextColor(Color4B::BLACK);
		}
		if (iOld >= 0)
		{
			m_MenuItemBiaoJiItem[iOld]->setEnabled(true);
			m_pLabelBiaoJiTuPianMiaoShu[iOld]->setTextColor(Color4B(0x88, 0xe7, 0xb4, 0xff));
		}
	}
}

void UserInfoView::ShowTool(bool bShow)
{
	m_pLabelToolPrice->setVisible(bShow);
	//m_pScrollViewTool->setVisible(bShow);
	m_pScrollViewTool->setTouchEnabled(bShow);
	for (auto child : m_pScrollViewTool->getChildren()) {
		child->setVisible(bShow);
	}

	//for (auto item : m_vecToolTouchGrabber)
	//{
	//	item->SetTouchEnable(bShow);
	//}
	// ShowExpress(!bShow);
}

void UserInfoView::ShowExpress(bool bShow)
{
	if (bShow)
	{
		// m_pScrollViewQuickExpress->removeAllChildrenWithCleanup(true);
		for(auto item: m_vecQuickExpress)
		{
			item->removeFromParent();
		}
		for(auto item: m_vecBtQuickExpress)
		{
			item->removeFromParent();
		}
		m_vecQuickExpress.clear();
		m_vecBtQuickExpress.clear();

		auto& deqExpress = g_globalMyData.m_deqExpress;
		unsigned uTools = deqExpress.size();
		float fXSpace = 100;
		float fX = 50;
		float fAllW = uTools* fXSpace + fX * 2 - fXSpace;
		float fH = 100;
		auto sz = m_pScrollViewQuickExpress->getContentSize();
		if (fAllW > sz.width)
		{
			m_pScrollViewQuickExpress->setContentSize(Size(fAllW, sz.height));
		}
		else
		{
			auto szV = m_pScrollViewQuickExpress->getViewSize();
			if (fAllW < szV.width)
			{
				fAllW = szV.width;
			}
			m_pScrollViewQuickExpress->setContentSize(Size(fAllW, sz.height));
		}

		static char *s_szAllExpress[] = {
			"express/allexpress.png",
			"express/allexpressnoble.png"
		};
		for (auto item : deqExpress)
		{
			unsigned uType = item / 16;
			unsigned uIdx = item % 16;
			auto pSprite = CreateNode4AndPosAddTo<FrameSprite>(s_szAllExpress[item / 16], 4, 4, uIdx, fX, 50, m_pScrollViewQuickExpress, 0);
			auto bt = LayerButton::create(false, true);
			auto sz = pSprite->getContentSize();
			bt->setContentSize(sz);
			bt->setPosition(fX - sz.width / 2, 50 - sz.height / 2);
			m_pScrollViewQuickExpress->addChild(bt);

			bt->SetHitCB([uType, uIdx, this](Ref*){
				Json::Value param;
				param["expressidx"] = uIdx;
				param["expresstype"] = uType;
				SendClientRequest(ClientRequest::kPublicExpress, param);
				this->setVisible(false);
			});
			fX += fXSpace;
			m_vecQuickExpress.push_back(pSprite);
			m_vecBtQuickExpress.push_back(bt);
		}
	}

	m_pLabelQuickExpress->setVisible(bShow);
	//m_pScrollViewQuickExpress->setVisible(bShow);

	m_pScrollViewQuickExpress->setTouchEnabled(bShow);
	for (auto child : m_pScrollViewQuickExpress->getChildren()) {
		child->setVisible(bShow);
	}
}


UserInfoHallView *UserInfoHallView::create()
{
	auto pRet = new UserInfoHallView;
	if (pRet->init("userinfo/bguserhallview.png"))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}
void UserInfoHallView::initUserInfoHallViewData(bool bVisible)
{
	m_pMenuItemPaiJuJiLu->setVisible(bVisible);
	m_pMenuItemWoDeDaoJu->setVisible(bVisible);
	m_pMenuItemChengJiu->setVisible(bVisible);
	m_pLabelChengJiu->setVisible(bVisible);
	for(int i = 0;i < 4; i++)
	{
		m_pSpriteChengJiu[i]->setVisible(bVisible);
	}	
	if (bVisible)
	{
		m_pSpriteTouXiang->ResetToLocalSprite("defaulticon.png");
	}
	

	m_pMenuItemBianJiTouXiang->setVisible(false);
	m_pMenuItemBindPhone->setVisible(false);
	m_pMenuItemBindPhone->setEnabled(bVisible);

	m_pSpriteXingBie->SetFrameIdx(0);
	SetLabelStringWithBoundWidth(m_pLabelNiChen, "", 250);
	m_pSpriteVIP->setVisible(false);

	for (int i=0 ;i<4;i++)
	{
		m_pMenuItemJoinButton[i]->setVisible(false);
			SetLabelString(m_pMenuItemFamilyLabel[i],"");
	}
	m_pMenuItemJoinFamily->setVisible(false);
		
	SetLabelString(m_pLabelID, "");
	m_pLabelID->setVisible(bVisible);

	m_pMenuItemKaiQiVIP->setVisible(false);

	SetLabelString(m_pLabelMeiLi, "");
	m_pLabelMeiLi->setVisible(bVisible);
	SetLabelString(m_pLabelLeBi, "");
	m_pLabelLeBi->setVisible(bVisible);
    SetLabelString(m_pLabelDiamond, "");
    m_pLabelDiamond->setVisible(bVisible);

	SetLabelString(m_pLabelLV, "");
	m_pLabelLV->setVisible(bVisible);

	SetLabelString(m_pLabelLV2, "");
	m_pLabelLV2->setVisible(bVisible);

	m_pProgressLV->setPercentage(0);

	SetLabelString(m_pLabelZhouYingLi, "");
	m_pLabelZhouYingLi->setVisible(bVisible);
	SetLabelString(m_pLabelRiDanJu, "");
	m_pLabelRiDanJu->setVisible(bVisible);

	SetLabelString(m_pLabelZongJuShu, "");
	m_pLabelZongJuShu->setVisible(bVisible);

	SetLabelString(m_pLabelZongShengLv, "");
	m_pLabelZongShengLv->setVisible(bVisible);

	SetLabelString(m_pLabelYingLiPaiMing, "");
	m_pLabelYingLiPaiMing->setVisible(bVisible);
	SetLabelString(m_pLabelDanJuPaiMing, "");
	m_pLabelDanJuPaiMing->setVisible(bVisible);
	for (unsigned i = 0; i < 5; ++i)
	{

			m_pSpriteZuiDaPaiXing[i]->SetFrameIdx(0);
			m_pSpriteZuiDaPaiXing[i]->setVisible(false);
	}

	for (unsigned i = 0; i < 4; ++i)
	{
		m_pSpriteChengJiu[i]->SetFrameIdx(0);
	}	
	SetLabelString(m_pLabelChengJiu, "");
	m_pLabelChengJiu->setVisible(bVisible);

}

void UserInfoHallView::UpdateUserSngInfo(const UserSngInfo& usrInfo)
{



	char szTxt[128] = {0};
	sprintf(szTxt, "%d%", usrInfo.m_uSNGBiSaiCiShu);
	SetLabelString(m_pLabelSNGCanSaiCiShu,szTxt) ;//"参赛次数:"
	sprintf(szTxt, "%d", usrInfo.m_uSNGHuoJiangCiShu);
	SetLabelString(m_pLabelSNGHuoJiangCiShu,szTxt) ;//"获奖次数:"
	sprintf(szTxt, "%d", usrInfo.m_uSNGZhouDaShiFen);
	SetLabelString(m_pLabelSNGZhouDaShiFen,szTxt) ;//"周大师分:"
	sprintf(szTxt, "%d", usrInfo.m_iSNGDaShiPaiMing);
	SetLabelString(m_pLabelSNGDaShiPaiMing,szTxt) ;//"大师排名:"

	for (int i = 0; i < 3; i++)
	{
		sprintf(szTxt, "x%d", usrInfo.m_uSNGJiangBei[i]);
		double dValue = usrInfo.m_uSNGJiangBei[i];
		m_pNumSNGJiangBei[i]->UpdateNumber(dValue);
	}
}

void UserInfoHallView::UpdateUserMTTInfo(const UserMTTInfo& usrInfo)
{
	char szTxt[128] = {0};
	sprintf(szTxt, "%d%", usrInfo.m_uMTTBiSaiCiShu);
	SetLabelString(m_pLabelMTTCanSaiCiShu,szTxt) ;//"参赛次数:"
	sprintf(szTxt, "%d", usrInfo.m_uMTTHuoJiangCiShu);
	SetLabelString(m_pLabelMTTJiangLiCiShu,szTxt) ;//"获奖次数:"
	
	sprintf(szTxt, "%d", usrInfo.m_uMTTZhouDaShiFen);
	SetLabelString(m_pLabelMTTZhouDaShiFen,szTxt) ;//"周大师分:"
	sprintf(szTxt, "%d", usrInfo.m_iMTTDaShiPaiMing);
	SetLabelString(m_pLabelMTTDaShiPaiMing,szTxt) ;//"大师排名:"

	if (usrInfo.m_iZuiHaoChengJi ==0)
	{
		SetLabelString(m_pLabelMTTBest,"本周最好成绩：暂无") ;//"获奖次数:"
	}
	else
	{
		sprintf(szTxt, "本周最好成绩：第%d名", usrInfo.m_iZuiHaoChengJi);
		SetLabelString(m_pLabelMTTBest,szTxt) ;//"获奖次数:"
	}
	
}

void UserInfoHallView::UpdateUserInfo(const UserDetailInfo& usrInfo, bool bMySelf)
{
	m_uTUID = usrInfo.m_uUID;

	m_uid = usrInfo.m_uUID ;
	m_fid = usrInfo.m_familyID ;
	m_hasJoinFamily = usrInfo.m_hasJoinFamily ;
	if (usrInfo.m_familyID!=0)
	{
		
	}

	if (!bMySelf)
	{
		m_pMenuItemPaiJuJiLu->setVisible(false);
		m_pMenuItemWoDeDaoJu->setVisible(false);
		m_pMenuItemChengJiu->setVisible(false);
		m_pLabelChengJiu->setVisible(false);
		for(int i = 0;i < 4; i++)
		{
			m_pSpriteChengJiu[i]->setVisible(false);
		}	
		SetIconUrl(m_pSpriteTouXiang, usrInfo.m_strIcon);
        m_pLabelDiamond->setVisible(false);
        m_pSpriteDiamond->setVisible(false);
	}
	else
	{
		m_pMenuItemPaiJuJiLu->setVisible(true);
		m_pMenuItemWoDeDaoJu->setVisible(true);
		m_pMenuItemChengJiu->setVisible(true);
		m_pLabelChengJiu->setVisible(true);
        m_pLabelDiamond->setVisible(true);
        m_pSpriteDiamond->setVisible(true);
		for(int i = 0;i < 4; i++)
		{
			m_pSpriteChengJiu[i]->setVisible(true);
		}
        
        if (g_globalMyData.GetSkinID() == 0) {
            SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetIconURL());
        } else {
            SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
        }
        
	}
	m_pMenuItemBianJiTouXiang->setVisible(bMySelf);
	m_pMenuItemBindPhone->setVisible(bMySelf);
	m_pMenuItemBindPhone->setEnabled(!g_globalMyData.IsBindPhone());
	if (bMySelf && !g_globalMyData.IsBindPhone() && !m_bAlreadyOpen)
	{
			PopTipWithBt2("绑定手机可以更有效地保护您的账号安全，建议立即绑定。", "立即绑定", "暂不绑定", [this](unsigned uBt){
				if (uBt == 0)
				{
					UserInfoHallView::PopMobileBind();
				}
                m_bAlreadyOpen = true;
			});
	}
	m_pSpriteXingBie->SetFrameIdx(usrInfo.m_bBoy ? 0 : 1);
	SetLabelStringWithBoundWidth(m_pLabelNiChen, usrInfo.m_strName, 250);
	if (usrInfo.m_iVipDengJi > 0)
	{
		m_pSpriteVIP->setVisible(true);
		m_pSpriteVIP->SetFrameIdx(usrInfo.m_iVipDengJi-1);
	}
	else
	{
		m_pSpriteVIP->setVisible(false);
	}

	if(usrInfo.m_familyLV > 0 && usrInfo.m_familyLV <= 2)
	{
		m_pMenuItemJoinButton[0]->setVisible(true);
		m_uFamilyID = 0;
	}
	else if(usrInfo.m_familyLV > 2 && usrInfo.m_familyLV <= 5)
	{
		m_pMenuItemJoinButton[1]->setVisible(true);
		m_uFamilyID = 1;
	}
	else if(usrInfo.m_familyLV > 5 && usrInfo.m_familyLV <= 8)
	{
		m_pMenuItemJoinButton[2]->setVisible(true);
		m_uFamilyID = 2;
	}
	else if(usrInfo.m_familyLV > 8)
	{
		m_pMenuItemJoinButton[3]->setVisible(true);
		m_uFamilyID = 3;
	}

	if(usrInfo.m_hasJoinFamily)
	{
		// 显示俱乐部的名称
		m_pMenuItemJoinFamily->setVisible(false);
		for(int i = 0; i < 4; i++)
		{
			if(i == m_uFamilyID)
			{
				m_pMenuItemJoinButton[m_uFamilyID]->setVisible(true);
				SetLabelString(m_pMenuItemFamilyLabel[i],usrInfo.m_familyName.c_str());
			}	
			else
			{
				m_pMenuItemJoinButton[i]->setVisible(false);
			}
		}	
	}
	else
	{
		//m_pMenuItemJoinFamily->setVisible(true);
		m_pMenuItemJoinFamily->setVisible(false);
		for(int i = 0; i < 4; i++)
		{
			m_pMenuItemJoinButton[i]->setVisible(false);
		}
	}



	//m_pMenuItemJoinFamily->setVisible(true);
	m_pMenuItemJoinFamily->setVisible(false);
	for(int i = 0; i < 4; i++)
	{
		m_pMenuItemJoinButton[i]->setVisible(false);
	}










	char szTxt[128] = {0};
	sprintf(szTxt, "%d", usrInfo.m_uUID);
	SetLabelString(m_pLabelID, szTxt);
	if (bMySelf)
	{
		m_pMenuItemKaiQiVIP->setVisible(usrInfo.m_iVipDengJi < 1);
	}
	else
	{
		m_pMenuItemKaiQiVIP->setVisible(false);
		m_pMenuItemJoinFamily->setVisible(false);
	}
	
	sprintf(szTxt, "+%s", FormatCash3(usrInfo.m_iMeiLiZhi).c_str());
	SetLabelString(m_pLabelMeiLi, szTxt);
	SetLabelString(m_pLabelLeBi, FormatCash3(usrInfo.m_dCash));
    SetLabelString(m_pLabelDiamond, FormatCash3(usrInfo.m_dDiamond));

	sprintf(szTxt, "LV%d", usrInfo.m_uJingYanDengJi);
	SetLabelString(m_pLabelLV, szTxt);
	sprintf(szTxt, "%d/%d", usrInfo.m_uJingYanJinDu, usrInfo.m_uJingYanZongZhi);
	SetLabelString(m_pLabelLV2, szTxt);
	m_pProgressLV->setPercentage((float)usrInfo.m_uJingYanJinDu / (float)usrInfo.m_uJingYanZongZhi * 100.0f);

	SetLabelString(m_pLabelZhouYingLi, do_fraction(usrInfo.m_fZhouYinLi));
	SetLabelString(m_pLabelRiDanJu, do_fraction(usrInfo.m_fRiDanJu));
	sprintf(szTxt, "%d", usrInfo.m_uZongJuShu);
	SetLabelString(m_pLabelZongJuShu, szTxt);
	sprintf(szTxt, "%d%%", usrInfo.m_uZongShengLv);
	SetLabelString(m_pLabelZongShengLv, szTxt);

	SetLabelString(m_pLabelYingLiPaiMing, do_fraction(usrInfo.m_iYinLiPaiMing));
	SetLabelString(m_pLabelDanJuPaiMing, do_fraction(usrInfo.m_iDanJuPaiMing));
	for (unsigned i = 0; i < 5; ++i)
	{
		if(usrInfo.m_cbMaxCardData[i] == 0)
		{
			m_pSpriteZuiDaPaiXing[i]->setVisible(false);
		}
		else
		{
			m_pSpriteZuiDaPaiXing[i]->SetFrameIdx(GetCardFrameIdx(usrInfo.m_cbMaxCardData[i]));
			m_pSpriteZuiDaPaiXing[i]->setVisible(true);
		}
	}

	for (unsigned i = 0; i < 4; ++i)
	{
		m_pSpriteChengJiu[i]->SetFrameIdx(1-usrInfo.m_uChengJiu[i]);
	}	
	sprintf(szTxt, "成就进度%d/%d", usrInfo.m_uChengJiuStep[0], usrInfo.m_uChengJiuStep[1]);
	SetLabelString(m_pLabelChengJiu, szTxt);

	//sng

	//sprintf(szTxt, "%d%", usrInfo.m_uSNGBiSaiCiShu);
	//SetLabelString(m_pLabelSNGCanSaiCiShu,szTxt) ;//"参赛次数:"
	//sprintf(szTxt, "%d", usrInfo.m_uSNGHuoJiangCiShu);
	//SetLabelString(m_pLabelSNGCanSaiCiShu,szTxt) ;//"获奖次数:"
	//sprintf(szTxt, "%d", usrInfo.m_uSNGZhouDaShiFen);
	//SetLabelString(m_pLabelSNGCanSaiCiShu,szTxt) ;//"周大师分:"
	//sprintf(szTxt, "%d", usrInfo.m_iSNGDaShiPaiMing);
	//SetLabelString(m_pLabelSNGCanSaiCiShu,szTxt) ;//"大师排名:"


	//for (int i = 0; i < 3; i++)
	//{
	//	sprintf(szTxt, "x%d", usrInfo.m_uSNGJiangBei[i]);
	//	double dValue = usrInfo.m_uSNGJiangBei[i];
	//	m_pNumSNGJiangBei[i]->UpdateNumber(dValue);
	//}


	setShowDataLoading(false) ;
}

void UserInfoHallView::Preload(bool bMySelf)
{
	if (!bMySelf)
	{
		m_pMenuItemPaiJuJiLu->setVisible(false);
		m_pMenuItemWoDeDaoJu->setVisible(false);
		m_pMenuItemBindPhone->setVisible(false);
		m_pMenuItemChengJiu->setVisible(false);
		m_pLabelChengJiu->setVisible(false);
		for(int i = 0;i < 4; i++)
		{
			m_pSpriteChengJiu[i]->setVisible(false);
		}	
	}
	else
	{
		m_pMenuItemPaiJuJiLu->setVisible(true);
		m_pMenuItemWoDeDaoJu->setVisible(true);
		m_pMenuItemBindPhone->setVisible(true);
		m_pMenuItemChengJiu->setVisible(true);
		m_pLabelChengJiu->setVisible(true);
		for(int i = 0;i < 4; i++)
		{
			m_pSpriteChengJiu[i]->setVisible(true);
		}	
		SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
	}
}
bool UserInfoHallView::init(const std::string& strBg)
{
	if (!GameDialog::init(strBg, true))
	{
		return false;
	}
	s_dataLoading =nullptr ;

	m_pSpriteTouXiang = CreateUserHeadSprite("defaulticon.png", "userinfo/iconclipmask.png", this, Vec2(79, 448), "userinfo/iconround.png");
	
	ccMenuCallback menuCB = CC_CALLBACK_1(UserInfoHallView::onMenuItemSelected, this);
	m_pMenuItemBianJiTouXiang = MenuItemSpriteWithLabel::create("userinfo/touxiang.png", 2, menuCB);
	CreateLabelMSYHAndAnchorPosClrAddTo(20, "编辑头像", Vec2(0.5, 0.5), 76, 24, Color4B::WHITE, m_pMenuItemBianJiTouXiang, 0);
	m_pMenuItemBianJiTouXiang->setPosition(154, 523);

	m_pMenuItemBindPhone = MenuItemFrameSprite::create("userinfo/btbindphone.png", 3, menuCB);
	m_pMenuItemBindPhone->setPosition(740, 532);


	// m_pSpriteBianJiTouXiang = CreateNode1AndPosAddTo<Sprite>("userinfo/iconedit.png", 0, 0, this, 0);
	CreateNode1AndPosAddTo<Sprite>("userinfo/iconround.png", 155, 523, this, 2);

	Color4B clrWhite = Color4B::WHITE;
	Color4B clrYellow(0xff, 0xd2, 0x00, 0xff);
	int iFontSize = 28;
	m_pSpriteXingBie = CreateNode3AndPosAddTo<FrameSprite>("userinfo/sex.png", 2, 1, 280, 582, this, 0);
	m_pLabelNiChen = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "中视网001", Vec2(0, 0), 310, 565, clrWhite, this, 0);
	m_pSpriteVIP = CreateNode4AndPosAddTo<FrameSprite>("vip.png", 6, 1, 1, 78, 563, this, 6);
	
	CreateLabelMSYHAndAnchorPosClrAddTo(22, "ID:", Vec2(0, 0), 667, 563, Color4B(0x50,0xbd,0x2e,0xff), this, 0);
	char szTxt[64] = {0};
	sprintf(szTxt, "%d", g_globalMyData.m_iUserId);
	m_pLabelID = CreateLabelMSYHAndAnchorPosClrAddTo(22, szTxt, Vec2(0, 0), 697, 563, Color4B(0x50,0xbd,0x2e,0xff), this, 0);
	m_pMenuItemKaiQiVIP = MenuItemSpriteWithLabel::create("userinfo/btkaiqivip.png", 3, menuCB);
	m_pMenuItemKaiQiVIP->setPosition(78, 563);

	m_pMenuItemJoinFamily = MenuItemSpriteWithLabel::create("userinfo/joinfamily.png", 3, menuCB);
	m_pMenuItemJoinFamily->setPosition(151, 415);

	CreateLabelMSYHAndAnchorPosClrAddTo(24, "幸运值:", Vec2(0, 0), 667, 440, clrYellow, this, 0);
	m_pLabelMeiLi = CreateLabelMSYHAndAnchorPosClrAddTo(24, "0", Vec2(0, 0), 750, 440, clrYellow, this, 0);

	//德州币上的动画
    auto spriteCash = CreateNode1AndPosAddTo<Sprite>("userinfo/lebi.png", 287, 533, this, 0);
    auto shineCash = CreateNode1AndPosAddTo<Sprite>("hall/moneyshine.png",29, 15, spriteCash, 0);
    shineCash->setScale(0);
    auto action1 = Sequence::create(ScaleTo::create(0.3, 1.2, 1.2, 0), DelayTime::create(0.3),ScaleTo::create(0.3, 0,0,0), DelayTime::create(1.5), nullptr);
    auto action2 = RepeatForever::create(Spawn::create(RotateBy::create(1.5f, Vec3(0,0,150)), action1, nullptr));
    shineCash->runAction(action2);
    //钻石上的动画
    m_pSpriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", 287, 495, this, 0);
    m_pSpriteDiamond->setScale(0.8);
    auto shineDiamond = CreateNode1AndPosAddTo<Sprite>("hall/moneyshine.png",39, 21, m_pSpriteDiamond, 0);
    shineDiamond->setScale(0);
    auto action3 = Sequence::create(ScaleTo::create(0.3, 1.2, 1.2, 0), DelayTime::create(0.3),ScaleTo::create(0.3, 0,0,0), DelayTime::create(1.5), nullptr);
    auto action4 = RepeatForever::create(Spawn::create(RotateBy::create(1.5f, Vec3(0,0,150)), action3, nullptr));
    shineDiamond->runAction(action4);

	m_pLabelLeBi = CreateLabelMSYHAndAnchorPosClrAddTo(24, "0", Vec2(0, 0.5), 320, 533, clrYellow, this, 0);
    m_pLabelDiamond = CreateLabelMSYHAndAnchorPosClrAddTo(24, "0", Vec2(0, 0.5), 320, 495, clrYellow, this, 0);

	m_pLabelLV = CreateLabelMSYHAndAnchorPosClrAddTo(24, "LV0", Vec2(0, 0), 273, 440, clrYellow, this, 0);

	CreateNode1AndPosAddTo<Sprite>("userinfo/bgprogress.png", 440, 454, this, 0);
	m_pProgressLV = CreateNode1AndPosAddTo<ProgressTimer>(Sprite::create("userinfo/progress.png"), 440, 454, this, 0);
	m_pProgressLV->setType(ProgressTimer::Type::BAR);
    m_pProgressLV->setMidpoint(Vec2(0, 0.5));
    m_pProgressLV->setBarChangeRate(Vec2(1, 0));
	m_pLabelLV2 = CreateLabelMSYHAndAnchorPosClrAddTo(16, "0/0", Vec2(0.5, 0.5), 440, 454, clrWhite, this, 0);
	m_pProgressLV->setPercentage(61.8);
	m_pMenuItemSwitch[0] = MenuItemSpriteWithLabel::create("userinfo/btchangguichang.png", 3, menuCB);
	m_pMenuItemSwitch[0]->setPosition(357, 403);
    auto lableCommon = CreateLabelMSYHBD(24, "常规场");
    lableCommon->setPosition(m_pMenuItemSwitch[0]->getContentSize().width/2, m_pMenuItemSwitch[0]->getContentSize().height/2);
    m_pMenuItemSwitch[0]->SetLabel(lableCommon);
	m_pMenuItemSwitch[1] = MenuItemSpriteWithLabel::create("userinfo/btsng.png", 3, menuCB);
	m_pMenuItemSwitch[1]->setPosition(542, 403);
    auto lableSNG = CreateLabelMSYHBD(24, "SNG");
    lableSNG->setPosition(m_pMenuItemSwitch[1]->getContentSize().width/2, m_pMenuItemSwitch[1]->getContentSize().height/2);
    m_pMenuItemSwitch[1]->SetLabel(lableSNG);
	m_pMenuItemSwitch[2] = MenuItemSpriteWithLabel::create("userinfo/btmtt.png", 3, menuCB);
	m_pMenuItemSwitch[2]->setPosition(727, 403);
    auto lableMTT = CreateLabelMSYHBD(24, "MTT");
    lableMTT->setPosition(m_pMenuItemSwitch[2]->getContentSize().width/2, m_pMenuItemSwitch[2]->getContentSize().height/2);
    m_pMenuItemSwitch[2]->SetLabel(lableMTT);

	for (auto& item : m_pLayerBg)
	{
		item = Layer::create();
		item->setContentSize(Size(900, 100));
		item->setPosition(4, 131);
		addChild(item);
	}
	iFontSize = 26;
	Vec2 labelAnchor(0, 0);
	Color4B clrGreen(0x50, 0xbd, 0x2e, 0xff);
	// 普通场
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "周盈利:", labelAnchor, 93, 183, clrWhite, m_pLayerBg[0], 0);
	m_pLabelZhouYingLi = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 194, 183, clrWhite, m_pLayerBg[0], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "日单局:", labelAnchor, 93, 136, clrWhite, m_pLayerBg[0], 0);
	m_pLabelRiDanJu = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 194, 136, clrWhite, m_pLayerBg[0], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "总局数:", labelAnchor, 93, 89, clrWhite, m_pLayerBg[0], 0);
	m_pLabelZongJuShu = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 194, 89, clrWhite, m_pLayerBg[0], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "总胜率:", labelAnchor, 93, 42, clrWhite, m_pLayerBg[0], 0);
	m_pLabelZongShengLv = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 194, 42, clrWhite, m_pLayerBg[0], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "盈利排名:", labelAnchor, 529, 183, clrWhite, m_pLayerBg[0], 0);
	m_pLabelYingLiPaiMing = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 660, 183, clrWhite, m_pLayerBg[0], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "单局排名:", labelAnchor, 529, 136, clrWhite, m_pLayerBg[0], 0);
	m_pLabelDanJuPaiMing = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 660, 136, clrWhite, m_pLayerBg[0], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "最大牌型:", labelAnchor, 529, 89, clrWhite, m_pLayerBg[0], 0);
	for (int i = 0; i < 5; ++i)
	{

		/*m_pSpriteZuiDaPaiXing[i] = CreateNode4AndPosAddTo<FrameSprite>("littlecard.webp", 13, 5, GetCardFrameIdx(0x11), 560 + 50 * i, 54, m_pLayerBg[0], 0);*/

		m_pSpriteZuiDaPaiXing[i] = CreateNode4AndPosAddTo<FrameSprite>("publiccard.webp", 13, 5, GetCardFrameIdx(0x11), 560 + 55 * i, 54, m_pLayerBg[0], 0);
		m_pSpriteZuiDaPaiXing[i]->setScale(0.6);
	}

	// SNG
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "参赛次数:", labelAnchor, 93, 183, clrWhite, m_pLayerBg[1], 0);
	m_pLabelSNGCanSaiCiShu = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 183, clrWhite, m_pLayerBg[1], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "获奖次数:", labelAnchor, 93, 136, clrWhite, m_pLayerBg[1], 0);
	m_pLabelSNGHuoJiangCiShu = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 136, clrWhite, m_pLayerBg[1], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "周大师分:", labelAnchor, 93, 89, clrWhite, m_pLayerBg[1], 0);
	m_pLabelSNGZhouDaShiFen = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 89, clrWhite, m_pLayerBg[1], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "大师排名:", labelAnchor, 93, 42, clrWhite, m_pLayerBg[1], 0);
	m_pLabelSNGDaShiPaiMing = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 42, clrWhite, m_pLayerBg[1], 0);
	CreateNode1AndPosAddTo<Sprite>("userinfo/wodejiangbei.png", 667, 140, m_pLayerBg[1], 0);

	for (int i = 0; i < 3; i++)
	{
		m_pNumSNGJiangBei[i] = CreateNode2AndPosAddTo<TextureNumber>("userinfo/numjiangbei.png", true, 564 + 113 * i, 39, m_pLayerBg[1], 0);
		m_pNumSNGJiangBei[i]->UpdateNumber(0);
	}

	// MTT
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "参赛次数:", labelAnchor, 93, 183, clrWhite, m_pLayerBg[2], 0);
	m_pLabelMTTCanSaiCiShu = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 183, clrWhite, m_pLayerBg[2], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "奖励次数:", labelAnchor, 93, 136, clrWhite, m_pLayerBg[2], 0);
	m_pLabelMTTJiangLiCiShu = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 136, clrWhite, m_pLayerBg[2], 0);
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "周大师分:", labelAnchor, 93, 89, clrWhite, m_pLayerBg[2], 0);
	m_pLabelMTTZhouDaShiFen = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 89, clrWhite, m_pLayerBg[2], 0);
	//m_pLabelMTTZhouDaShiFen->setVisible(false) ;
	CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "大师排名:", labelAnchor, 93, 42, clrWhite, m_pLayerBg[2], 0);
	m_pLabelMTTDaShiPaiMing = CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "0", labelAnchor, 222, 42, clrWhite, m_pLayerBg[2], 0);
	//m_pLabelMTTDaShiPaiMing->setVisible(false) ;
	CreateNode1AndPosAddTo<Sprite>("userinfo/mttzuihaochengji.png", 667, 130, m_pLayerBg[2], 0);

	m_pLabelMTTBest= CreateLabelMSYHAndAnchorPosClrAddTo(iFontSize, "本周最好成绩", Point(0.5,0.5), 660, 91, clrWhite, m_pLayerBg[2], 0);

	// 成就
	m_pMenuItemChengJiu = MenuItemSpriteWithLabel::create("userinfo/btchengjiu.png", 2, menuCB);
	m_pMenuItemChengJiu->setPosition(216, 88);
	char szChengJiu[64] = {0};
	for (int i = 0; i < 4; ++i)
	{
		sprintf(szChengJiu, "userinfo/chengjiu%d.png", i);
		m_pSpriteChengJiu[i] = CreateNode3AndPosAddTo<FrameSprite>(szChengJiu, 1, 2, 120 + i * 68, 100, this, 6);
	}
	m_pLabelChengJiu = CreateLabelMSYHAndAnchorPosClrAddTo(20, "成就进度 ", Vec2(0.5, 0.5), 215, 55, clrWhite, this, 6);

	//
	//m_pMenuItemPaiJuJiLu = MenuItemSpriteWithLabel::create("userinfo/btgreen2.png", 3, menuCB);
    m_pMenuItemPaiJuJiLu = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), menuCB);
	m_pMenuItemPaiJuJiLu->setPosition(523, 88);
	auto label = CreateLabelMSYHBD(28, "牌局记录");
	m_pMenuItemPaiJuJiLu->SetLabel(label);
	//m_pMenuItemWoDeDaoJu = MenuItemSpriteWithLabel::create("userinfo/btgreen2.png", 3, menuCB);
    m_pMenuItemWoDeDaoJu = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), menuCB);
	m_pMenuItemWoDeDaoJu->setPosition(776, 88);
	label = CreateLabelMSYHBD(28, "我的道具");
	m_pMenuItemWoDeDaoJu->SetLabel(label);

	m_pMenuItemSwitch[0]->setEnabled(false);
    m_pMenuItemSwitch[0]->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
	m_pLayerBg[1]->setVisible(false);
	m_pLayerBg[2]->setVisible(false);

	for(int i = 0;i < 4; i++)
	{
		char *szButton[128] = {"userinfo/userinfor_familybg0.png", "userinfo/userinfor_familybg1.png",
			"userinfo/userinfor_familybg2.png", "userinfo/userinfor_familybg3.png"};
		m_pMenuItemJoinButton[i] = MenuItemSpriteWithLabel::create(szButton[i], 3, CC_CALLBACK_1(UserInfoHallView::onMenuCallBackFamily, this));
		m_pMenuItemJoinButton[i]->setPosition(152, 415);
		m_pMenuItemFamilyLabel[i] = CreateLabelMSYHBD(28, "");
		m_pMenuItemJoinButton[i]->SetLabel(m_pMenuItemFamilyLabel[i]);
		m_pMenuItemJoinButton[i]->setVisible(false);
	}






	auto pMenu = Menu::create(m_pMenuItemBianJiTouXiang, m_pMenuItemKaiQiVIP, m_pMenuItemJoinFamily,m_pMenuItemJoinButton[0],m_pMenuItemJoinButton[1],m_pMenuItemJoinButton[2], m_pMenuItemJoinButton[3],m_pMenuItemSwitch[0], m_pMenuItemSwitch[1], m_pMenuItemSwitch[2],m_pMenuItemChengJiu, m_pMenuItemPaiJuJiLu, m_pMenuItemWoDeDaoJu, m_pMenuItemBindPhone, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu, 5);

	m_uTUID = 0;
	m_uFamilyID = 0;
	m_pMobileBind = nullptr;
	g_globalMyData.WaitChange(WAITCASHCHANGE |WAITGENDERCHANGE| WAITSKINCHANGE|WAITVIPCHANGE | WAITNICKNAMECHANGE | WAITBINDPHONECHANGE, std::bind(&UserInfoHallView::OnMyDataChanged, this, std::placeholders::_1));
	//initUserInfoHallViewData(false) ;
    m_bAlreadyOpen = false;

	return true;
}




void UserInfoHallView::onMenuItemSelected(cocos2d::Ref* pItem)
{
	if (pItem == m_pMenuItemSwitch[0])
	{
		m_pMenuItemSwitch[0]->setEnabled(false);
		m_pMenuItemSwitch[1]->setEnabled(true);
		m_pMenuItemSwitch[2]->setEnabled(true);
        m_pMenuItemSwitch[0]->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
        m_pMenuItemSwitch[1]->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
        m_pMenuItemSwitch[2]->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
		m_pLayerBg[0]->setVisible(true);
		m_pLayerBg[1]->setVisible(false);
		m_pLayerBg[2]->setVisible(false);
	}
	else if (pItem == m_pMenuItemSwitch[1])
	{
		


#if Open_Sng
		m_pMenuItemSwitch[0]->setEnabled(true);
		m_pMenuItemSwitch[1]->setEnabled(false);
		m_pMenuItemSwitch[2]->setEnabled(true);
        m_pMenuItemSwitch[0]->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
        m_pMenuItemSwitch[1]->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
        m_pMenuItemSwitch[2]->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
		m_pLayerBg[0]->setVisible(false);
		m_pLayerBg[1]->setVisible(true);
		m_pLayerBg[2]->setVisible(false);
#else
		PopTipWithBt1("功能尚未开启！","确定",nullptr) ;
#endif
		

	}
	else if (pItem == m_pMenuItemSwitch[2])
	{
		//PopTipWithBt1("功能尚未开启！","确定",nullptr) ;
		m_pMenuItemSwitch[0]->setEnabled(true);
		m_pMenuItemSwitch[1]->setEnabled(true);
		m_pMenuItemSwitch[2]->setEnabled(false);
		m_pMenuItemSwitch[0]->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
		m_pMenuItemSwitch[1]->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
		m_pMenuItemSwitch[2]->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
		m_pLayerBg[0]->setVisible(false);
		m_pLayerBg[1]->setVisible(false);
		m_pLayerBg[2]->setVisible(true);
	}
	else if (pItem == m_pMenuItemChengJiu)
	{
		if (m_uTUID)
		{
			Json::Value param;
			param["tid"] = m_uTUID;
			SendClientRequest(ClientRequest::kPopUserAchievementInfo, param);
		}
	}
	else if (pItem == m_pMenuItemBianJiTouXiang)
	{
		auto pHall = dynamic_cast<GameHall*>(getParent());
		pHall->PopPersonalInfo();
	}
	else if (pItem == m_pMenuItemBindPhone)
	{
		UserInfoHallView::PopMobileBind();
	}
	else if (pItem == m_pMenuItemPaiJuJiLu)
	{
		if (m_uTUID == g_globalMyData.m_iUserId)
		{
			for (int i = 0; i < 2; i++)
			{
				g_globalGameRecord.m_vecRecord[i].clear();
				g_globalGameRecord.m_vecPageIdx[i].clear();
			}

			Json::Value param;
			param["page"] = 0;
			param["sortType"] = 1;
			param["countPerPage"] = GameRecordPerPage;
			SendClientRequest(ClientRequest::kGetGamePlayBackList, param);
		}
	}
	else if (pItem == m_pMenuItemKaiQiVIP)
	{
		// TODO
		Json::Value param;
		param["idx"] = 1;
		SendClientRequest(ClientRequest::kPopMall, param);
	}
	else if(pItem == m_pMenuItemJoinFamily)
	{
		/*auto pHall = dynamic_cast<GameHall*>(getParent());

		pHall->showGameFamily(false,true);
		GameDialog::setVisible(false) ;*/


	}
	else if (pItem == m_pMenuItemWoDeDaoJu)
	{
		auto pHall = dynamic_cast<GameHall*>(getParent());

		pHall->showUserItem(true) ;
	

	}
}

void UserInfoHallView::onMenuCallBackFamily(cocos2d::Ref *pItem)
{
	for(int i = 0; i < 4; i++)
	{
		if(pItem == m_pMenuItemJoinButton[i])
		{
			auto pHall = dynamic_cast<GameHall*>(getParent());
			if (pHall->isVisible()&&!pHall->isMyFamilyVisible())
			{
				if (m_fid == g_globalMyData.getFamilyID())
				{
					GameDialog::setVisible(false) ;
				/*	pHall->showMyFamily(false,true);

					Json::Value msg;
					msg["ufamilyid"] = m_fid;
					SendClientRequest(ClientRequest::kGetMyFamilyInfo,msg);*/
				}
			}
		
		
		}
		else
		{
			m_pMenuItemJoinButton[i]->setVisible(false);
		}
	}	
}

void UserInfoHallView::PopMobileBind()
{
	if(m_pMobileBind == nullptr)
		{
			m_pMobileBind = MobileBind::create();
			m_pMobileBind->setPosition((938 - 678) / 2,(640-410) / 2);
			addChild(m_pMobileBind,7);
		}
		else
		{
			m_pMobileBind->setVisible(true);
		}
}

void UserInfoHallView::OnMyDataChanged(unsigned uDataType)
{
	if (m_uTUID == g_globalMyData.m_iUserId)
	{
		if (uDataType == WAITCASHCHANGE)
		{
			SetLabelString(m_pLabelLeBi, FormatCash(g_globalMyData.GetCash()));
		}
		else if (uDataType == WAITGENDERCHANGE)
		{
			m_pSpriteXingBie->SetFrameIdx(g_globalMyData.IsBoy() ? 0 : 1);
		}
		else if (uDataType == WAITSKINCHANGE)
		{
            if (g_globalMyData.GetSkinID() == 0) {
                SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetIconURL());
            } else {
                SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
            }
		}
		else if (uDataType == WAITNICKNAMECHANGE)
		{
			SetLabelString(m_pLabelNiChen, g_globalMyData.GetNickName());
		}
		else if (uDataType == WAITVIPCHANGE)
		{
			if (g_globalMyData.GetVipLevel() > 0)
			{
				m_pSpriteVIP->setVisible(true);
				m_pSpriteVIP->SetFrameIdx(g_globalMyData.GetVipLevel()-1);
			}
			else
			{
				m_pSpriteVIP->setVisible(false);
				m_pMenuItemKaiQiVIP->setVisible(true);
			}
		}
		else if (uDataType == WAITBINDPHONECHANGE)
		{
			m_pMenuItemBindPhone->setEnabled(!g_globalMyData.IsBindPhone());
		}
	}
}

void UserInfoHallView::onSetVisible(bool bVisible)
{
	//PlayVisibleSound(bVisible);
}

void UserInfoHallView::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		/* auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this, bVisible](){setOpacity(0);setScale(0.3);GameDialog::setVisible(true);
		initUserInfoHallViewData(bVisible);
		}), actScale, nullptr));*/
		
		initUserInfoHallViewData(bVisible) ;
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		//GameDialog::setVisible(false);
		callHideAction() ;
		initUserInfoHallViewData(bVisible) ;
		/*  auto actScale = Spawn::create(ScaleTo::create(0.2, 0.3), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this, bVisible](){GameDialog::setVisible(false);
		initUserInfoHallViewData(bVisible);
		}), nullptr));*/
    }
	setShowDataLoading(bVisible) ;
}

void UserInfoHallView::setShowDataLoading(bool flag)
{
	if (s_dataLoading == nullptr)
	{

		s_dataLoading = Tools::creatDataLoading(this,489, 330,99) ;
	}
	s_dataLoading->setVisible(flag) ;

}

