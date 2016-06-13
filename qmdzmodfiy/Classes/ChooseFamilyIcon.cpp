#include "ChooseFamilyIcon.h"

#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"
#include "HttpSprite.h"
#include "GrabTouch.h"
#include "Util.h"
#include "MyFamily.h"
USING_NS_CC;

enum MyAvatar
{
    AvatarScrollW = 865,
    AvatarScrollH = 220
};

ChooseFamilyIcon* ChooseFamilyIcon::creatWithLayer(MyFamily* layer)
{
	ChooseFamilyIcon* sFamliy = new ChooseFamilyIcon() ;
	if (sFamliy && sFamliy->initWithLayer(layer))
	{
		sFamliy->autorelease();
		return sFamliy ;
	}
	else
	{
		CC_SAFE_DELETE(sFamliy);
		return nullptr;
	}
}

bool ChooseFamilyIcon::initWithLayer(MyFamily* layer)
{

	if (!Layer::init())
	{
		return false;
	}

	return true ;
}

bool ChooseFamilyIcon::init()
{
	if (!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}


	auto labelAnchor = Vec2(0, 0);
	int labelSize = 24;
	auto laberColor = Color4B(0x50, 0xbd, 0x2e, 0xff);
	auto whiteColor = Color4B::WHITE;


	spriteTitle = Sprite::create("family/family_head-title.png");
	spriteTitle->setPosition(469,569);
	this->addChild(spriteTitle);


	//头像滚动视图
	m_ScrollMyAvatar = CreateNode1AndPosAddTo<extension::ScrollView>(Size(AvatarScrollW, AvatarScrollH), 54, 242, this, 0);
	m_ScrollMyAvatar->setContentSize(Size(AvatarScrollW,AvatarScrollH));
	m_ScrollMyAvatar->setDirection(extension::ScrollView::Direction::HORIZONTAL);

	//拍照
	//m_btTakePhoto = MenuItemSpriteWithLabel::create("personalinfo/bluebtn.png", 3, CC_CALLBACK_1(ChooseFamilyIcon::TakePhoto, this));
    m_btTakePhoto = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), CC_CALLBACK_1(ChooseFamilyIcon::TakePhoto, this));
	m_btTakePhoto->setPosition(317, 187);
	auto menuTakePhoto = Menu::create(m_btTakePhoto, nullptr);
	menuTakePhoto->setPosition(Vec2::ZERO);
	this->addChild(menuTakePhoto);
	auto lableTakePhoto = CreateLabelMSYHBD(30, "拍照");
	lableTakePhoto->setTextColor(cocos2d::Color4B::WHITE);
	m_btTakePhoto->SetLabel(lableTakePhoto);

	//从相册选择
	//m_btSelectFromAlbum = MenuItemSpriteWithLabel::create("personalinfo/bluebtn.png", 3, CC_CALLBACK_1(ChooseFamilyIcon::SelectFromAlbum, this));
    m_btSelectFromAlbum = MenuItemScale9SpriteWithLabel::create(1, Size(234, 74), CC_CALLBACK_1(ChooseFamilyIcon::SelectFromAlbum, this));
	m_btSelectFromAlbum->setPosition(634, 187);
	auto menuSelectFromAlbum = Menu::create(m_btSelectFromAlbum, nullptr);
	menuSelectFromAlbum->setPosition(Vec2::ZERO);
	this->addChild(menuSelectFromAlbum);
	auto lableSelectFromAlbum = CreateLabelMSYHBD(30, "从相册选择");
	lableSelectFromAlbum->setTextColor(cocos2d::Color4B::WHITE);
	m_btSelectFromAlbum->SetLabel(lableSelectFromAlbum);


	m_btTakePhoto->setEnabled(false) ;
	m_btSelectFromAlbum->setEnabled(false) ;

	//警告
	m_lableWarn = CreateLabelMSYH(20, "警告：绝对禁止上传以下内容的图片：色情、淫秽、惊悚、烟草、酒精、毒品等。一旦发现，此类头像将被管理员立即删除，账号将得到应有处罚。");
	m_lableWarn->setTextColor(cocos2d::Color4B(0x5c, 0xc3, 0x80, 0xff));
	m_lableWarn->setPosition(477, 71);
	m_lableWarn->setWidth(810);
	m_lableWarn->setLineBreakWithoutSpace(true);
	m_lableWarn->setAlignment(TextHAlignment::LEFT);
	this->addChild(m_lableWarn);


	m_SpriteUsing = CreateNode3AndPosAddTo<FrameSprite>("personalinfo/used.png", 1, 1, 90, AvatarScrollH/2, m_ScrollMyAvatar, 2);

	//m_btTakePhoto->setEnabled(false);
	//m_btSelectFromAlbum->setEnabled(false);

    return true;
}


void ChooseFamilyIcon::MyAvatar(cocos2d::Ref* pSender)
{
    m_ScrollMyAvatar->setVisible(true);
    m_btTakePhoto->setVisible(true);
    m_btSelectFromAlbum->setVisible(true);
    m_lableWarn->setVisible(true);  
}

void ChooseFamilyIcon::PersonalInfo(cocos2d::Ref* pSender)
{
    m_ScrollMyAvatar->setVisible(false);

    m_lableWarn->setVisible(false);

	//m_btTakePhoto->setVisible(false);
	//m_btSelectFromAlbum->setVisible(false);
}

void ChooseFamilyIcon::UpdatePersonalInfo()
{
    if (!m_vecAvatarItem.empty())
    {
        std::vector<HeadItem>::iterator iter;
        iter = m_vecAvatarItem.begin();
        while (iter != m_vecAvatarItem.end())
        {
            iter->m_layerBt->removeFromParent();
            iter->m_httpSprite->removeFromParent();
            iter++;
        }
        m_vecAvatarItem.clear();
    }
	
    unsigned uAllItem = g_globalMyData.m_FamilySkinListCount;
    unsigned uOneItemW = 150;//条目宽度
    unsigned uInterval = 50; //条目间隙
    unsigned uOffSet = 25;   //左右都有偏移，不到滚动视图的两端
    unsigned uAllItemW = (uOneItemW + uInterval)*uAllItem;
    unsigned fY = (AvatarScrollH - uOneItemW)/2, fX = uOffSet;
    for (int i=0; i< g_globalMyData.m_FamilySkinListCount; i++)
    {
		if (g_globalMyData.m_familySkinList[i].m_fuid == g_globalMyFamilyInfo.getFamilyIconID())
        {
            m_SpriteUsing->setVisible(true);
            m_SpriteUsing->setPosition(fX + uOneItemW/2, AvatarScrollH/2);
        }
        fX += (uOneItemW + uInterval);
    }
    if (g_globalMyFamilyInfo.getFamilyIconID() == 0)
    {
        m_SpriteUsing->setVisible(false);
    }

    fX = uOffSet;
    HeadItem avatarItem;

    for (unsigned uAvatarID = 0; uAvatarID < uAllItem; uAvatarID++)
    {
        auto httpSprite = CreateUserHeadSprite("family/defaulticon.png", "userinfo/iconclipmask.png", m_ScrollMyAvatar, Vec2(fX, fY), "userinfo/iconround.png");
        auto layerBt = LayerButton::create(false, true);
        Size sz = httpSprite->getContentSize();
        layerBt->setContentSize(sz);
		SetFamilyIconUrl(httpSprite, g_globalMyData.m_familySkinList[uAvatarID].m_strIconUrl);
        httpSprite->addChild(layerBt);
		layerBt->SetHitCB(std::bind(&ChooseFamilyIcon::SelectAvatar, this, fX + uOneItemW/2, uAvatarID));

        fX += (uOneItemW + uInterval);
        avatarItem.m_httpSprite = httpSprite;
        avatarItem.m_layerBt = layerBt;
        m_vecAvatarItem.push_back(avatarItem);
    }
    m_ScrollMyAvatar->setContentSize(Size(uAllItemW, AvatarScrollH));



	MyAvatar(nullptr);
}

void ChooseFamilyIcon::SelectAvatar(unsigned fX, unsigned uIdx)
{

	m_SpriteUsing->setVisible(true) ;

	m_SpriteUsing->setPosition(fX, AvatarScrollH/2);
	std::string strData;
	unsigned uOpType = 1;
	Json::Value param;
	char szT[128] = {0};
	/*sprintf(szT, "%d", g_globalMyData.m_userSkinList[uIdx].m_uID);*/
	param["idindex"] = g_globalMyData.m_familySkinList[uIdx].m_fuid;
	param["id"] = g_globalMyData.m_familySkinList[uIdx].m_strIconUrl;
	param["ufamilyid"] = g_globalMyData.getFamilyID();
	SendClientRequest(ClientRequest::kEditFamilyIcon, param);

}

void ChooseFamilyIcon::TakePhoto(cocos2d::Ref* pSender)
{

}

void ChooseFamilyIcon::SelectFromAlbum(cocos2d::Ref* pSender)
{
}


void ChooseFamilyIcon::onMenuItemCloseSelected(cocos2d::Ref * ref)
{
	SendClientRequest(ClientRequest::kOptHallBgLayer);
	GameDialog::onMenuItemCloseSelected(ref);
}