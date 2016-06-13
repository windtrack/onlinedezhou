#include "GameData.h"
#include "cocos2d.h"
#include "Tools.h"





USING_NS_CC ;
GameData* GameData::_instance = 0;



GameData* GameData::getInstance()
{
	if (_instance == 0)
	{
		_instance = new GameData() ;
		_instance->setUpdateVersion(false) ;
		_instance->setSoundOn(true);
		_instance->setPauseNetWork(false) ;
		_instance->setPauseNetWorkClear(true) ;
		_instance->setOpenUserInfoFormGameHall(false) ;
		//_instance->readIpAddress() ;

		_instance->setGetMttFloatMathcInfo(false) ;
		_instance->setHasRequestMatch(false) ;
		//_instance->initGameTablePos();
	}

	return _instance ;
}


void GameData::readShowSkinBt(unsigned uid)
{
	char name[64];
	sprintf(name,"s_showSkinButton%d",uid) ;
	s_showSkinButton = UserDefault::getInstance()->getBoolForKey(name, true) ;
}

void GameData::setHasTeachSkinButton(unsigned uid)
{

		s_showSkinButton = false ;
		char name[64];
		sprintf(name,"s_showSkinButton%d",uid) ;
		UserDefault::getInstance()->setBoolForKey(name, s_showSkinButton) ;


}

bool GameData::getShowSkinButton(unsigned uid)
{
	readShowSkinBt(uid) ;
	return s_showSkinButton ;
}

void GameData::initGameTablePos(Node*& root)
{
	//auto root = CSLoader::createNode("res/ui/gametable.csb");
	//Size frameSize = Director::getInstance()->getVisibleSize(); 
	//root->setContentSize(frameSize); 
	//ui::Helper::doLayout(root);  

	Node* m_rootUser[GAME_PLAYER];

	//公共牌的坐标
	Node* nodecardPos= Tools::getWidgetByName(root,"Panel_public_card");

	Node* nodeAllinChizi= Tools::getWidgetByName(root,"Panel_userallin");
	Node* nodeQipai= Tools::getWidgetByName(root,"Panel_qipai");

	//聊天框坐标
	Node* nodeTalk= Tools::getWidgetByName(root,"Panel_talk");

	//视频播放坐标
	Node* nodeVideo= Tools::getWidgetByName(root,"Panel_videoplayer");

	//重制所有翻面坐标
	int posX[GAME_PLAYER];

	char buffer[64];
	//更新坐标   寻找好麻烦  实例化一次 保存下来
	for (int i=0; i< GAME_PLAYER; i++)
	{
		memset(buffer, 0, 64);
		//char pathUserBlock[64];
		sprintf(buffer,"user_block%d",i) ;
		m_rootUser[i] = Tools::getWidgetByName(root,buffer);

		memset(buffer, 0, 64);
		//char pathNode[64];
		sprintf(buffer,"user%d",i) ;
		Node* nodeRoot = Tools::getWidgetByName(root,buffer);

		memset(buffer, 0, 64);
		//char pathHead[64];
		sprintf(buffer,"user_head%d",i) ;
		Node* nodeHead = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_headPos = nodeHead->getPosition();

		memset(buffer, 0, 64);
		//char pathName[64];
		sprintf(buffer,"user_name%d",i) ;
		Node* nodeName = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_namePos = nodeName->getPosition();

		memset(buffer, 0, 64);
		//char pathVip[64];
		sprintf(buffer,"user_vip%d",i) ;
		Node* nodeVip = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_vipPos = nodeVip->getPosition();

		memset(buffer, 0, 64);
		//char pathCash[64];
		sprintf(buffer,"user_cash%d",i) ;
		Node* nodeCash = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_cashPos = nodeCash->getPosition();

		memset(buffer, 0, 64);
		//char pathThink[64];
		sprintf(buffer,"user_thinking%d",i) ;
		Node* nodeThink = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_thinkPos = nodeThink->getPosition();

		memset(buffer, 0, 64);
		//char pathBiaoji[64];
		sprintf(buffer,"user_biaoji%d",i) ;
		Node* nodeBiaoji = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_biaojiPos = nodeBiaoji->getPosition();

		memset(buffer, 0, 64);
		//char pathDNL[64];
		sprintf(buffer,"user_denvlang%d",i) ;
		Node* nodeDNL = Tools::getWidgetByName(m_rootUser[i],buffer);
		m_usePosInfo[i].u_denvlangPos = nodeDNL->getPosition();


		// 用户下注筹码-从用户池飞向边池
		memset(buffer, 0, 64);
		//char pathchoumaG[64];
		sprintf(buffer,"user_chouma%d",i) ;
		Node* nodeChouma= Tools::getWidgetByName(nodeRoot,buffer);
		s_ptJettonDst[i] = nodeChouma->getPosition();


		//下注金额位置
		memset(buffer, 0, 64);
		//char pathUseXiazhuCash[64];
		sprintf(buffer,"user_xiazhucash%d",i) ;
		Node* nodeXiazhuCash = Tools::getWidgetByName(nodeRoot,buffer);
		s_ptLabelBet[i] = nodeXiazhuCash->getPosition();


		//下注背景位置
		memset(buffer, 0, 64);
		//char pathUseXiazhuBG[64];
		sprintf(buffer,"user_xiazhublock%d",i) ;
		Node* nodeXiazhuBG= Tools::getWidgetByName(nodeRoot,buffer);
		s_ptBgBet[i] = nodeXiazhuBG->getPosition();

		//D的位置
		memset(buffer, 0, 64);
		//char pathD[64];
		sprintf(buffer,"user_zhuang%d",i) ;
		Node* nodeD= Tools::getWidgetByName(nodeRoot,buffer);
		s_ptDPosition[i] = nodeD->getPosition();


		//////////////////////////////////////////////////////////////////
		s_ptUserViewDefault[i] = m_rootUser[i]->getPosition() ;
		s_ptUserViewDefaultCenter[i]= m_rootUser[i]->getPosition()  ;

		memset(buffer, 0, 64);
		//char pathD0[64];
		sprintf(buffer,"gt_Node_hand card_%d_%d",i,1) ;
		Node* nodeD0= Tools::getWidgetByName(nodecardPos,buffer);
		s_ptDispatchedCard[i] = nodeD0->getPosition();

		memset(buffer, 0, 64);
		//char pathD1[64];
		sprintf(buffer,"gt_Node_hand card_%d_%d",i,2) ;
		Node* nodeD1= Tools::getWidgetByName(nodecardPos,buffer);
		s_ptDispatchedCard[i+9] = nodeD1->getPosition();

		//9个池子坐标
		memset(buffer, 0, 64);
		//char pathchizi[64];
		sprintf(buffer,"bianchi%d",i) ;
		Node* nodechzzi= Tools::getWidgetByName(nodeAllinChizi,buffer);
		s_allinChizi[i] = nodechzzi->getPosition();
		s_allinChizi[i].x-=nodechzzi->getContentSize().width/2;
		s_allinChizi[i].y-=nodechzzi->getContentSize().height/2;


		//弃牌中心店
		memset(buffer, 0, 64);
		//char pathqipai[64];
		sprintf(buffer,"gt_node_qipai_%d",i) ;
		Node* nodeqipai= Tools::getWidgetByName(nodeQipai,buffer);
		s_ptFoldingCard[i] = nodeqipai->getPosition();

		////////////////////////////////////////////////////
		memset(buffer, 0, 64);
		//char pathtalk[64];
		sprintf(buffer,"gt_Node_chat bg%d",i) ;
		Node* nodetalk= Tools::getWidgetByName(nodeTalk,buffer);

		s_ptChatTip[i] = nodetalk->getPosition();

		//////////////////////////////////////////////////////////////
		memset(buffer, 0, 64);
		//char pathvideoopen[64];
		sprintf(buffer,"gt_node_videoopen%d",i) ;
		Node* nodeVeiopopen= Tools::getWidgetByName(nodeVideo,buffer);

		s_ptOpen[i] = nodeVeiopopen->getPosition();

		memset(buffer, 0, 64);
		//char pathvideo[64];
		sprintf(buffer,"gt_node_video%d",i) ;
		Node* nodeVeioplayer= Tools::getWidgetByName(nodeVideo,buffer);

		s_ptPlayer[i] = nodeVeioplayer->getPosition();

		/////////////////////////////////////////////
		posX[i]= s_allinChizi[i].x;
	}

	std::string posCard5[5]= {"gt_Node_public card_0", "gt_Node_public card_1", "gt_Node_public card_2", "gt_Node_public card_3", "gt_Node_public card_4", } ;
	for (int i=0 ;i<5 ;i++)
	{
		Node* nodeChouma= Tools::getWidgetByName(nodecardPos,posCard5[i]);
		s_ptPublicCard[i] = nodeChouma->getPosition();		
	}

	s_ptUserViewOutTable[0] = Vec2(859,750);
	s_ptUserViewOutTable[1] = Vec2(1240,467);
	s_ptUserViewOutTable[2] = Vec2(1297,652);
	s_ptUserViewOutTable[3] = Vec2(998,-135);
	s_ptUserViewOutTable[4] = Vec2(555,-149);
	s_ptUserViewOutTable[5] = Vec2(100,-135);
	s_ptUserViewOutTable[6] = Vec2(-150,10);
	s_ptUserViewOutTable[7] = Vec2(-150,535);
	s_ptUserViewOutTable[8] = Vec2(306,750);



	//主角正面牌坐标
	s_ptMyCard[0] =Tools::getWidgetByName(nodecardPos,"gt_Node_hand card_4_a_2")->getPosition();
	s_ptMyCard[1] =Tools::getWidgetByName(nodecardPos,"gt_Node_hand card_4_a_1")->getPosition();

    s_ptDiChi = Tools::getWidgetByName(root,"gt_text_dichi")->getPosition();

	int posY[2];
	posY[0] = s_allinChizi[0].y;
	posY[1] = s_allinChizi[5].y;


	static Vec2 s_ptPot1[GAME_PLAYER] = {
		Vec2(posX[2],posY[0]),
		Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	};
	static Vec2 s_ptPot2[GAME_PLAYER] = {
		Vec2(posX[6],posY[0]),Vec2(posX[7],posY[0]),
		Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	};
	static Vec2 s_ptPot3[GAME_PLAYER] = {
		Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),
		Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	};
	static Vec2 s_ptPot4[GAME_PLAYER] = {
		Vec2(posX[5],posY[0]),Vec2(posX[6],posY[0]),Vec2(posX[7],posY[0]),Vec2(posX[8],posY[0]),
		Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	};
	static Vec2 s_ptPot5[GAME_PLAYER] = {
		Vec2(posX[0],posY[0]),Vec2(posX[1],posY[0]),
		Vec2(posX[2],posY[0]),
		Vec2(posX[3],posY[0]),Vec2(posX[4],posY[0]),	
		Vec2(), Vec2(), Vec2(), Vec2(),
	};
	static Vec2 s_ptPot6[GAME_PLAYER] = {
		Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),
		Vec2(posX[1],posY[1]),Vec2(posX[2],posY[1]),Vec2(posX[3],posY[1]),	
		Vec2(), Vec2(), Vec2(),
	};

	static Vec2 s_ptPot7[GAME_PLAYER] = {
		Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),
		Vec2(posX[6],posY[1]),Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),Vec2(posX[8],posY[1]),
		Vec2(), Vec2(),
	};
	static Vec2 s_ptPot8[GAME_PLAYER] = {
		Vec2(posX[5],posY[0]),Vec2(posX[6],posY[0]),Vec2(posX[7],posY[0]),Vec2(posX[8],posY[0]),
		Vec2(posX[5],posY[1]),Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),Vec2(posX[8],posY[1]),
		Vec2(),
	};
	static Vec2 s_ptPot9[GAME_PLAYER] = {
		Vec2(posX[0],posY[0]),Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),Vec2(posX[4],posY[0]),
		Vec2(posX[5],posY[1]),Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),Vec2(posX[8],posY[1]),
	};

	 s_ptPots[0] = s_ptPot1;
	 s_ptPots[1] = s_ptPot2;
	 s_ptPots[2] = s_ptPot3;
	 s_ptPots[3] = s_ptPot4;
	 s_ptPots[4] = s_ptPot5;
	 s_ptPots[5] = s_ptPot6;
	 s_ptPots[6] = s_ptPot7;
	 s_ptPots[7] = s_ptPot8;
	 s_ptPots[8] = s_ptPot9;
}

void GameData::setVibrate(bool flags)
{
	UserDefault::getInstance()->setBoolForKey("vibrate", flags);
	UserDefault::getInstance()->flush();
}

bool GameData::getVibrate()
{
	return UserDefault::getInstance()->getBoolForKey("vibrate", true);
}



int GameData::getMessageLastReadTime()
{

	s_messageLastReadTime = UserDefault::getInstance()->getIntegerForKey("s_messageLastReadTime",0) ;

	if (s_messageLastReadTime ==0 )
	{
		updateCurMessageReadTime() ;
	}

	return s_messageLastReadTime ; 
}

void GameData::updateCurMessageReadTime()
{
	
	time_t time_Now = time(NULL);
	s_messageLastReadTime = time_Now ;
	UserDefault::getInstance()->setIntegerForKey("s_messageLastReadTime",time_Now);
	s_hasNewMessage = false ;
}


void GameData::setNewMessageTime(long time)
{
	s_messageNewMessageTime = time ;

	getMessageLastReadTime() ;
	if (s_messageNewMessageTime>=s_messageLastReadTime)
	{
		s_hasNewMessage = true ;
	}
	else
	{
		s_hasNewMessage = false ;
	}

}


//

void GameData::setFirstCreatFamily(unsigned uid,bool flag)
{
	if (getIsFirstCreatFamily(uid))
	{
		return ;
	}
	s_firstCreatFamily = flag ;
	char name[64];
	sprintf(name,"s_firstCreatFamily%d",uid) ;
	UserDefault::getInstance()->setBoolForKey(name,s_firstCreatFamily);
}
bool GameData::getIsFirstCreatFamily(unsigned uid)
{
	char name[64];
	sprintf(name,"s_firstCreatFamily%d",uid) ;
	
	return UserDefault::getInstance()->getBoolForKey(name,false) ;
}

void GameData::setFistInMyfamily(unsigned uid,bool flag)
{
	if (getFirstInMyFamily(uid))
	{
		return ;
	}
	s_firstInMyGamily = flag ;
	char name[64];
	sprintf(name,"s_firstInMyGamily%d",uid) ;
	UserDefault::getInstance()->setBoolForKey(name,s_firstInMyGamily);
}

bool GameData::getFirstInMyFamily(unsigned uid)
{
	char name[64];
	sprintf(name,"s_firstInMyGamily%d",uid) ;
	return UserDefault::getInstance()->getBoolForKey(name,false) ;
}

void GameData::setPauseNetWork(bool flag)
{
	pauseNetWork = flag ;
}

void GameData::readIpAddress(std::string ip)
{
	m_ipaddress = UserDefault::getInstance()->getStringForKey("ip") ;
	if (m_ipaddress== "")
	{
		m_ipaddress = ip ;
		UserDefault::getInstance()->setStringForKey("ip",m_ipaddress) ;
		
	}
}

void GameData::readSngOnline(unsigned uid)
{
	char name[64];
	sprintf(name,"s_showSngOnline%d",uid) ;
	s_showSngOnline = UserDefault::getInstance()->getBoolForKey(name,false) ;
}

void GameData::saveHasShowSngOnline(unsigned uid)
{
	char name[64];
	sprintf(name,"s_showSngOnline%d",uid) ;
	s_showSngOnline = true ;
	UserDefault::getInstance()->setBoolForKey(name,true) ;
}
/************************************************************************/
/* 添加滚动提示                                                                     */
/************************************************************************/
void GameData::addGameTips(MoveLabel* label)
{
	if (label == nullptr)
	{
		return ;
	}
	m_vecMoveLineLabel.pushBack(label);

	g_pGameFrame->callGameTipsMove() ;


}
/************************************************************************/
/* 取一个提示来显示                                                                     */
/************************************************************************/
MoveLabel* GameData::getOneMoveLable()
{
	MoveLabel* label = m_vecMoveLineLabel.at(0) ;
	return  label; 
}


void GameData::readTeach(int index)
{
    char buff[32] ;
    sprintf(buff,"teach_%d",index) ;
    UserDefault::getInstance()->getBoolForKey(buff,false) ;
    UserDefault::getInstance()->getStringForKey(buff,"0") ;

}

void GameData::saveTeach(int index)
{
    char buff[32] ;
    sprintf(buff,"teach_%d",index) ;

    UserDefault::getInstance()->setBoolForKey(buff,true) ;
}