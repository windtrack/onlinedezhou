#include "PersonalInfomation.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"
#include "HttpSprite.h"
#include "GrabTouch.h"
#include "Util.h"
#include "PlatformSpec.h"
#include "GameConfig.h"
#include "Tools.h"
#include "GameFrame.h"
USING_NS_CC;

enum MyAvatar
{
    AvatarScrollW = 865,
    AvatarScrollH = 220
};

bool PersonalInfomation::init()
{
    if (!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }
	
    auto labelAnchor = Vec2(0, 0);
    int labelSize = 24;
    auto laberColor = Color4B(0x50, 0xbd, 0x2e, 0xff);
    auto whiteColor = Color4B::WHITE;

    //我的头像
    m_btMyAvatar = MenuItemSpriteWithLabel::create("personalinfo/myavatar.png", 3, CC_CALLBACK_1(PersonalInfomation::MyAvatar, this));
    m_btMyAvatar->setPosition(277, 529);
    auto menuMyAvatar = Menu::create(m_btMyAvatar, nullptr);
    menuMyAvatar->setPosition(Vec2::ZERO);
    this->addChild(menuMyAvatar);
    m_btMyAvatar->setEnabled(false);

    //个人资料
    m_btPersonalInfo = MenuItemSpriteWithLabel::create("personalinfo/myprofile.png", 3, CC_CALLBACK_1(PersonalInfomation::PersonalInfo, this));
    m_btPersonalInfo->setPosition(646, 529);
    auto menuPersonalInfo = Menu::create(m_btPersonalInfo, nullptr);
    menuPersonalInfo->setPosition(Vec2::ZERO);
    this->addChild(menuPersonalInfo);

    //头像滚动视图
    m_ScrollMyAvatar = CreateNode1AndPosAddTo<extension::ScrollView>(Size(AvatarScrollW, AvatarScrollH), 54, 242, this, 0);
    m_ScrollMyAvatar->setContentSize(Size(AvatarScrollW,AvatarScrollH));
    m_ScrollMyAvatar->setDirection(extension::ScrollView::Direction::HORIZONTAL);

    //拍照
    //m_btTakePhoto = MenuItemSpriteWithLabel::create("personalinfo/bluebtn.png", 3, CC_CALLBACK_1(PersonalInfomation::TakePhoto, this));
    m_btTakePhoto = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), CC_CALLBACK_1(PersonalInfomation::TakePhoto, this));
    m_btTakePhoto->setPosition(317, 187);
    auto menuTakePhoto = Menu::create(m_btTakePhoto, nullptr);
    menuTakePhoto->setPosition(Vec2::ZERO);
    this->addChild(menuTakePhoto);
    auto lableTakePhoto = CreateLabelMSYHBD(30, "拍照");
    lableTakePhoto->setTextColor(cocos2d::Color4B::WHITE);
    m_btTakePhoto->SetLabel(lableTakePhoto);

	


    //从相册选择
    //m_btSelectFromAlbum = MenuItemSpriteWithLabel::create("personalinfo/bluebtn.png", 3, CC_CALLBACK_1(PersonalInfomation::SelectFromAlbum, this));
    m_btSelectFromAlbum = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), CC_CALLBACK_1(PersonalInfomation::SelectFromAlbum, this));
    m_btSelectFromAlbum->setPosition(634, 187);
    auto menuSelectFromAlbum = Menu::create(m_btSelectFromAlbum, nullptr);
    menuSelectFromAlbum->setPosition(Vec2::ZERO);
    this->addChild(menuSelectFromAlbum);
    auto lableSelectFromAlbum = CreateLabelMSYHBD(30, "从相册选择");
    lableSelectFromAlbum->setTextColor(cocos2d::Color4B::WHITE);
    m_btSelectFromAlbum->SetLabel(lableSelectFromAlbum);
	
	m_btSelectFromAlbum->setEnabled(false) ;
	m_btTakePhoto->setEnabled(false) ;

    //警告
    m_lableWarn = CreateLabelMSYH(20, "警告：绝对禁止上传以下内容的图片：色情、淫秽、惊悚、烟草、酒精、毒品等。一旦发现，此类头像将被管理员立即删除，账号将得到应有处罚。");
    m_lableWarn->setTextColor(cocos2d::Color4B(0x5c, 0xc3, 0x80, 0xff));
    m_lableWarn->setPosition(477, 71);
    m_lableWarn->setWidth(810);
    m_lableWarn->setLineBreakWithoutSpace(true);
    m_lableWarn->setAlignment(TextHAlignment::LEFT);
    this->addChild(m_lableWarn);
	
    //昵称、性别、QQ、邮箱
    m_SpriteNickName = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 468, 414, this, 0);
    auto label = CreateLabelMSYHAndAnchorPosClrAddTo(24, "昵称 : ", labelAnchor, 43, 21, laberColor, m_SpriteNickName, 0);
    m_editNickName = CreateNode2AndPosAddTo<ui::EditBox>(Size(561, 62), ui::Scale9Sprite::create("personalinfo/bgedit.png") , 390, 35, m_SpriteNickName, 0);
    m_editNickName->setFont("fzlt.ttf", 26);
    m_editNickName->setFontColor(Color3B::WHITE);
    m_editNickName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editNickName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editNickName->setDelegate(this);
    CreateLabelMSYHAndAnchorPosClrAddTo(20, "昵称支持8位以内的汉字/字母/符号等", labelAnchor, 400, 21, Color4B(0x17,0x92,0x40,0xff), m_SpriteNickName, 0);
	
    auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 468, 331, this, 0);
    auto btExpand = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/btnarrow.png", 1, 2, 699, 36, bgItem, 0);
    label = CreateLabelMSYHAndAnchorPosClrAddTo(24, "性别 : ", labelAnchor, 43, 21, laberColor, bgItem, 0);
    auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
    touchGrabber->retain();
    touchGrabber->GetTouchProcer().SetHitCB(
        [bgItem, btExpand](bool bSel, Node*, TouchGrabber*){bgItem->SetFrameIdx(bSel?1:0);btExpand->SetFrameIdx(bSel?1:0);},
        [this](bool bSel, Node*, TouchGrabber*){if (bSel) ExpandGender();});
    m_SpriteGender = bgItem;
    m_SpriteMan = Sprite::create("personalinfo/man.png");
    bgItem->addChild(m_SpriteMan);
    m_SpriteMan->setAnchorPoint(labelAnchor);
    m_SpriteMan->setPosition(113, 16);
    m_SpriteWoman = Sprite::create("personalinfo/woman.png");
    bgItem->addChild(m_SpriteWoman);
    m_SpriteWoman->setAnchorPoint(labelAnchor);
    m_SpriteWoman->setPosition(113, 16);
    m_SpriteMan->setVisible(false);
    m_SpriteWoman->setVisible(false);

    m_SpriteQQNum = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 468, 248, this, 0);
    label = CreateLabelMSYHAndAnchorPosClrAddTo(24, "QQ : ", labelAnchor, 43, 21, laberColor, m_SpriteQQNum, 0);
    m_editQQNum = CreateNode2AndPosAddTo<ui::EditBox>(Size(561, 62), ui::Scale9Sprite::create("personalinfo/bgedit.png") , 390, 35, m_SpriteQQNum, 0);
    m_editQQNum->setFont("fzlt.ttf", 26);
    m_editQQNum->setFontColor(Color3B::WHITE);
    m_editQQNum->setInputMode(ui::EditBox::InputMode::NUMERIC);
    m_editQQNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editQQNum->setDelegate(this);

    m_SpriteEmail = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/bgitem.png", 1, 2, 468, 165, this, 0);
    label = CreateLabelMSYHAndAnchorPosClrAddTo(24, "邮箱 : ", labelAnchor, 43, 21, laberColor, m_SpriteEmail, 0);
    m_editEmail = CreateNode2AndPosAddTo<ui::EditBox>(Size(561, 62), ui::Scale9Sprite::create("personalinfo/bgedit.png") , 390, 35, m_SpriteEmail, 0);
    m_editEmail->setFont("fzlt.ttf", 26);
    m_editEmail->setFontColor(Color3B::WHITE);
    m_editEmail->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
    m_editEmail->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editEmail->setDelegate(this);

    m_SpriteUsing = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/used.png", 1, 1, 90, AvatarScrollH/2, m_ScrollMyAvatar, 3);

    //性别弹框
    m_ExpandGender = GameDialog::create("personalinfo/modifygender.png", false);
    Size sz = m_ExpandGender->getContentSize();
    m_ExpandGender->setPosition(468 - sz.width/2, 286 - sz.height/2);
    this->addChild(m_ExpandGender);
    m_ExpandGender->setVisible(true);

    m_btSelectMan = MenuItemSpriteWithLabel::create("personalinfo/bgmodifygender.png", 2, CC_CALLBACK_1(PersonalInfomation::SelectMan, this));
    m_btSelectMan->setPosition(228, 114);
    auto menuSelectMan = Menu::create(m_btSelectMan, nullptr);
    menuSelectMan->setPosition(Vec2::ZERO);
    m_ExpandGender->addChild(menuSelectMan);

    m_btSelectWoman = MenuItemSpriteWithLabel::create("personalinfo/bgmodifygender.png", 2, CC_CALLBACK_1(PersonalInfomation::SelectWoman, this));
    m_btSelectWoman->setPosition(228, 44);
    auto menuSelectWoman = Menu::create(m_btSelectWoman, nullptr);
    menuSelectWoman->setPosition(Vec2::ZERO);
    m_ExpandGender->addChild(menuSelectWoman);

    auto labelWoman = CreateLabelMSYH(24, "女");
    m_ExpandGender->addChild(labelWoman);
    labelWoman->setPosition(260, 44);

    auto labelMan = CreateLabelMSYH(24, "男");
    m_ExpandGender->addChild(labelMan);
    labelMan->setPosition(260, 114);

    auto spriteMan = Sprite::create("personalinfo/man.png");
    m_ExpandGender->addChild(spriteMan);
    spriteMan->setPosition(190, 114);

    auto spriteWoman = Sprite::create("personalinfo/woman.png");
    m_ExpandGender->addChild(spriteWoman);
    spriteWoman->setPosition(190, 44);

    //m_btTakePhoto->setEnabled(false);
    //m_btSelectFromAlbum->setEnabled(false);
    
    g_globalMyData.WaitChange(WAITSKINCHANGE, std::bind(&PersonalInfomation::OnMyDataChanged, this, std::placeholders::_1));
    
    return true;
}


void PersonalInfomation::MyAvatar(cocos2d::Ref* pSender)
{
    m_btMyAvatar->setEnabled(false);
    m_btPersonalInfo->setEnabled(true);

    m_ScrollMyAvatar->setVisible(true);
    m_btTakePhoto->setVisible(true);
    m_btSelectFromAlbum->setVisible(true);
    m_lableWarn->setVisible(true);

    m_SpriteNickName->setVisible(false);
    m_SpriteGender->setVisible(false);
    m_SpriteQQNum->setVisible(false);
    m_SpriteEmail->setVisible(false);
    
}

void PersonalInfomation::PersonalInfo(cocos2d::Ref* pSender)
{
    m_btMyAvatar->setEnabled(true);
    m_btPersonalInfo->setEnabled(false);

    m_ScrollMyAvatar->setVisible(false);
	m_btTakePhoto->setVisible(false);
	m_btSelectFromAlbum->setVisible(false);
    m_lableWarn->setVisible(false);

    m_SpriteNickName->setVisible(true);
    m_SpriteGender->setVisible(true);
    m_SpriteQQNum->setVisible(true);
    m_SpriteEmail->setVisible(true);
}

void PersonalInfomation::UpdatePersonalInfo()
{

    for (auto item : m_vecHeadItemNew)
    {
        item->m_clipNode->setVisible(false);
        item->m_spriteFrame->setVisible(false);
        m_vecHeadItemOld.push_back(item);
    }
    m_vecHeadItemNew.clear();

    m_ExpandGender->setVisible(false);
    unsigned uAllItem = g_globalMyData.m_uSkinListCount;
    unsigned uOneItemW = 150;//条目宽度
    unsigned uInterval = 50; //条目间隙
    unsigned uOffSet = 25;   //左右都有偏移，不到滚动视图的两端
    unsigned uAllItemW = (uOneItemW + uInterval)*uAllItem;
    unsigned fY = (AvatarScrollH - uOneItemW)/2, fX = uOffSet;
    for (int i=0; i< uAllItem; i++)
    {
		UserSkin skin = g_globalMyData.m_userSkinList.at(i) ;
		if (skin.m_uID == g_globalMyData.GetSkinID())
        {
            m_SpriteUsing->setVisible(true);
            m_SpriteUsing->setPosition(fX + uOneItemW/2, AvatarScrollH/2);
        }
        fX += (uOneItemW + uInterval);
    }

#if !OpenNewSkin
	if (g_globalMyData.GetSkinID() == 0)
	{
		m_SpriteUsing->setVisible(false);
	}
#endif


    fX = uOffSet;
    AvatatItem avatarItem;

    for (unsigned uAvatarID = 0; uAvatarID < uAllItem; uAvatarID++)
    {
        auto headItem = GetHeadItem(fX + uOneItemW/2, uAvatarID);
        headItem->m_clipNode->setVisible(true);
        headItem->m_spriteFrame->setVisible(true);
        headItem->m_clipNode->setPosition(fX, fY);
        headItem->m_spriteFrame->setPosition(fX+76, fY+76);


		SetIconUrl(headItem->m_httpSpriteHead, g_globalMyData.m_userSkinList.at(uAvatarID).m_strIconUrl);
		

        m_vecHeadItemNew.push_back(headItem);
        fX += (uOneItemW + uInterval);

    }
    m_ScrollMyAvatar->setContentSize(Size(uAllItemW, AvatarScrollH));

	m_editNickName->setText(g_globalMyData.GetNickName().c_str());
    m_editQQNum->setText(g_globalMyData.m_strQQ.c_str());
    m_editEmail->setText(g_globalMyData.m_strEmail.c_str());
	if (!g_globalMyData.IsBoy())
    {
        m_btSelectWoman->setEnabled(false);
        m_btSelectMan->setEnabled(true);
        m_SpriteWoman->setVisible(true);
        m_SpriteMan->setVisible(false);
    }
    else
    {
        m_btSelectWoman->setEnabled(true);
        m_btSelectMan->setEnabled(false);
        m_SpriteMan->setVisible(true);
        m_SpriteWoman->setVisible(false);
    }

	MyAvatar(nullptr);
}

PersonalInfomation::HeadItem* PersonalInfomation::GetHeadItem(unsigned fX, unsigned uSkinId)
{
    HeadItem *headItem = nullptr;
    if (m_vecHeadItemOld.empty())
    {
        //遮罩
        auto spriteMask = Sprite::create("userinfo/iconclipmask.png");
        auto bgSz = spriteMask->getContentSize();
        spriteMask->setPosition(bgSz.width/2, bgSz.height/2);
        auto clipNode = ClippingNode::create();
        clipNode->setStencil(spriteMask);
        clipNode->setInverted(false);
        clipNode->setAlphaThreshold(0.5);
        clipNode->setPosition(0, 0);
        clipNode->setContentSize(spriteMask->getContentSize());
        m_ScrollMyAvatar->addChild(clipNode, 0);
        //头像
        auto  httpSprite = CreateNode2AndPosAddTo<HttpSprite>("defaulticon.png", bgSz, 0, 0, clipNode, 0);
        httpSprite->ignoreAnchorPointForPosition(false);
        httpSprite->setAnchorPoint(Vec2(0.5, 0.5));
        httpSprite->setPosition(bgSz.width/2, bgSz.height/2);
        //头像上的点击事件
        auto layerBt = LayerButton::create(false, true);
        Size sz = httpSprite->getContentSize();
        layerBt->setContentSize(sz);
        httpSprite->addChild(layerBt);
        //外框
        auto spriteFrame = CreateNode1AndPosAddTo<Sprite>("userinfo/iconround.png", 0, 0, m_ScrollMyAvatar, 2);

        auto item = new HeadItem;
        item->m_spriteFrame = spriteFrame;
        item->m_spriteMask = spriteMask;
        item->m_clipNode = clipNode;
        item->m_httpSpriteHead = httpSprite;
        item->m_btnLayer = layerBt;
        headItem = item;
    }
    else
    {
        headItem = m_vecHeadItemOld.back();
        m_vecHeadItemOld.pop_back();
    }
    headItem->m_btnLayer->SetHitCB(std::bind(&PersonalInfomation::SelectAvatar, this, fX, uSkinId));

    return headItem;
}

void PersonalInfomation::updateHeadItem(PersonalInfomation::HeadItem* headShop, unsigned uSkinId)
{
    SetIconUrl(headShop->m_httpSpriteHead, g_globalMyData.m_userSkinList.at(uSkinId).m_strIconUrl);
}

void PersonalInfomation::SelectAvatar(unsigned fX, unsigned uIdx)
{
	m_SpriteUsing->setVisible(true);
    m_SpriteUsing->setPosition(fX, AvatarScrollH/2);
	std::string strData;
	unsigned uOpType = 1;
	Json::Value param;
	char szT[128] = {0};
	sprintf(szT, "%d", g_globalMyData.m_userSkinList.at(uIdx).m_uID);
	param["data"] = szT;
	param["type"] = uOpType;
	SendClientRequest(ClientRequest::kEditMyInfo, param);
}

void PersonalInfomation::ExpandGender()
{
    m_ExpandGender->setVisible(true);
}

void PersonalInfomation::SelectMan(cocos2d::Ref* pSender)
{
    m_btSelectWoman->setEnabled(true);
    m_btSelectMan->setEnabled(false);
    m_SpriteMan->setVisible(true);
    m_SpriteWoman->setVisible(false);
    m_ExpandGender->setVisible(false);

	std::string strData;
	unsigned uOpType = 3;
	Json::Value param;
	param["data"] = "1";
	param["type"] = uOpType;
	SendClientRequest(ClientRequest::kEditMyInfo, param);
}
void PersonalInfomation::SelectWoman(cocos2d::Ref* pSender)
{
    m_btSelectMan->setEnabled(true);
    m_btSelectWoman->setEnabled(false);
    m_SpriteWoman->setVisible(true);
    m_SpriteMan->setVisible(false);
    m_ExpandGender->setVisible(false);

	std::string strData;
	unsigned uOpType = 3;
	Json::Value param;
	param["data"] = "2";
	param["type"] = uOpType;
	SendClientRequest(ClientRequest::kEditMyInfo, param);
}

/*
当type为1时，修改用户的【头像】
当type为2时，修改用户的【昵称】
当type为3时，修改用户的【性别】
当type为4时，修改用户的【QQ号码】
当type为5时，修改用户的【邮箱】
*/
void PersonalInfomation::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	std::string strData = editBox->getText();
	bool flag = false ;
	unsigned uOpType = 0;
	if (editBox == m_editNickName)
	{
		uOpType = 2;
        int byteNum = getStrByteNumOfUTF8(strData, 8);
        if (strData.length() >= byteNum)
        {
            strData = strData.substr(0, byteNum);
            editBox->setText(strData.c_str());
			flag = true ;
        }
	}
	else if (editBox == m_editQQNum)
	{
		uOpType = 4;

		if (Tools::is_number(strData))
		{
			editBox->setText(strData.c_str());
			flag = true ;
		}
		else
		{
			 editBox->setText(g_globalMyData.m_strQQ.c_str());
			 PopTipWithBt1("请输入正确的qq号码","确定",nullptr) ;
		}

     /*   if (strData.length() > 12)
        {
            strData = strData.substr(0, 12);
            editBox->setText(strData.c_str());
        }*/
	}
	else if (editBox == m_editEmail)
	{
		uOpType = 5;

		if (Tools::IsValidEmail(strData))
		{
			editBox->setText(strData.c_str());
			flag = true ;
		}
		else
		{
			editBox->setText(g_globalMyData.m_strEmail.c_str());
			 PopTipWithBt1("请输入正确的邮箱","确定",nullptr) ;
		}

  /*      if (strData.length() > 24)
        {
            strData = strData.substr(0, 24);
            editBox->setText(strData.c_str());
        }*/
	}
	if (flag)
	{
		Json::Value param;
		param["data"] = strData;
		param["type"] = uOpType;
		SendClientRequest(ClientRequest::kEditMyInfo, param);
	}
	
}

void PersonalInfomation::TakePhoto(cocos2d::Ref* pSender)
{
    getImageFromCamera();
	//SendClientRequest(ClientRequest::kCallUploadPhoto);
}

void PersonalInfomation::SelectFromAlbum(cocos2d::Ref* pSender)
{
    getImageFromGallery();
}

void PersonalInfomation::OnMyDataChanged(unsigned uDataType)
{
    if (uDataType == WAITSKINCHANGE)
    {

#if !OpenNewSkin
		if (g_globalMyData.GetSkinID() == 0) {
			m_SpriteUsing->setVisible(false);
			setVisible(false);
		}
#endif
 
    }
}

