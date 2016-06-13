#include "SelfDefinition.h"
#include "Util.h"
#include <stdlib.h>
#include "GlobalUserData.h"
#include "Tools.h"

USING_NS_CC;


bool SelfDefinition::init()
{
	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	s_uiroot = CSLoader::createNode("res/ui/private_creatroom.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(s_uiroot,1);



	m_slider_playerNum = static_cast<ui::Slider*>(Tools::getWidgetByName(s_uiroot, "Slider_num"));
	m_slider_playerNum->addEventListenerSlider(this,sliderpercentchangedselector(SelfDefinition::onSliderChanged)) ;

	m_slider_gameJijin = static_cast<ui::Slider*>(Tools::getWidgetByName(s_uiroot, "Slider_jijin"));
	m_slider_gameJijin->addEventListenerSlider(this,sliderpercentchangedselector(SelfDefinition::onSliderChanged)) ;

	m_text_playerNum = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot, "Text_mod_num"));
	m_text_jiJin = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot, "Text_mod_jijin"));

	char buff[64] ;
	for (int i=0; i<7 ;i++)
	{
		sprintf(buff,"Text_mod_moren_%d",i) ;
		m_text_valueDeflaut[i] = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,buff));
		sprintf(buff,"Text_mod_%d",i) ;
		m_text_valueMod[i] = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot, buff));

		sprintf(buff,"Image_editbg%d",i) ;
		m_imgbg[i] = static_cast<ui::ImageView*>(Tools::getWidgetByName(s_uiroot,buff));

		m_EditMod[i] = CreateNode2AndPosAddTo<ui::EditBox>(Size(m_imgbg[i]->getContentSize().width, m_imgbg[i]->getContentSize().height),
		ui::Scale9Sprite::create("vipprivateroom/bgedit.png") , m_imgbg[i]->getContentSize().width / 2, m_imgbg[i]->getContentSize().height / 2,m_imgbg[i], 0);

		m_EditMod[i] ->setFont("fzlt.ttf", 26);
		m_EditMod[i] ->setFontColor(Color3B(0xff,0xd8,0x00));
		m_EditMod[i] ->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
		m_EditMod[i] ->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_EditMod[i] ->setDelegate(this);


	}






	m_btCreatRoom = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot, "Button_creat"));
	m_btCreatRoom->addTouchEventListener(CC_CALLBACK_2(SelfDefinition::OnMenuItemSelect, this));



	m_btFund  = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot, "Button_jijinhelp"));
	m_btFund->addTouchEventListener(CC_CALLBACK_2(SelfDefinition::OnMenuItemSelect, this));

	m_btMinCollect  = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot, "Button_qishouhelp"));
	m_btMinCollect->addTouchEventListener(CC_CALLBACK_2(SelfDefinition::OnMenuItemSelect, this));

	m_btMaxCollect  = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot, "Button_fengdinghelp"));
	m_btMaxCollect->addTouchEventListener(CC_CALLBACK_2(SelfDefinition::OnMenuItemSelect, this));

	m_imgFund = static_cast<ui::ImageView*>(Tools::getWidgetByName(s_uiroot, "Image_jijin"));
	m_imgMinCollect = static_cast<ui::ImageView*>(Tools::getWidgetByName(s_uiroot, "Image_qishou"));
	m_imgMaxCollect = static_cast<ui::ImageView*>(Tools::getWidgetByName(s_uiroot, "Image_fengding"));

	m_imgFund->setVisible(false) ;
	m_imgMinCollect->setVisible(false) ;
	m_imgMaxCollect->setVisible(false) ;

	m_buttonTouchAll = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot, "Button_touch"));
	m_buttonTouchAll->addTouchEventListener(CC_CALLBACK_2(SelfDefinition::OnMenuItemSelect, this));
	
	return true ;
}



void  SelfDefinition::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	cleanTips() ;

	for (int i = 0; i < 7; i++)
	{
		if (editBox == m_EditMod[i])
		{
			char buff[64] ;
			std::string showText = editBox->getText() ;

			m_EditMod[i]->setText("") ;

			std::string showid = "" ;

			//是数字并且 是4位 
			if (Tools::is_number(showText)&&showText.length()>0 && i!=6)
			{
				if (showText.length()>9)
				{
					PopTipWithBt1("数值过大","确定",nullptr) ;
					return ;
				}

				m_text_valueDeflaut[i]->setVisible(false) ;
				m_text_valueMod[i]->setVisible(true) ;
				setUITextString(m_text_valueMod[i],showText.c_str()) ;

				long number = std::atoi( showText.c_str() );

				//起收
				if (i == 0)
				{
					m_minCollectCash = number ;
					//封顶小于最小起收时
					if (m_minCollectCash > m_maxCollectCash)
					{
						m_maxCollectCash = m_minCollectCash ;

						m_text_valueDeflaut[1]->setVisible(false) ;
						m_text_valueMod[1]->setVisible(true) ;
						setUITextString(m_text_valueMod[1],showText.c_str()) ;
					}

				}
				//封顶
				else if (i == 1 )
				{
					//封顶小于最小起收时
					if (number < m_minCollectCash)
					{
						PopTipWithBt1("封顶不得小于起收","确定",nullptr) ;
						sprintf(buff,"%d",m_maxCollectCash) ;
						setUITextString(m_text_valueMod[1],buff) ;
					}
					else
					{
						m_maxCollectCash = number ;
					}
					
					
				}
				
				//最小携带
				else if (i == 2 )
				{
					//m_minCarryCash = number ;


					if (number > m_maxCarryCash)
					{
						m_maxCarryCash = number ;

						m_text_valueDeflaut[3]->setVisible(false) ;
						m_text_valueMod[3]->setVisible(true) ;
						sprintf(buff,"%d",m_maxCarryCash) ;
						setUITextString(m_text_valueMod[3],buff) ;
					}
					else if (number ==0)
					{
						PopTipWithBt1("最小携带不能为0","确定",nullptr) ;
						m_text_valueDeflaut[2]->setVisible(false) ;
						m_text_valueMod[2]->setVisible(true) ;
						sprintf(buff,"%d",m_minCarryCash) ;
						setUITextString(m_text_valueMod[2],buff) ;
					}
					else if (number <(m_anteMoney + m_minBlind*2))
					{
						PopTipWithBt1("最小携带不能小于前注加大盲","确定",nullptr) ;
						m_text_valueDeflaut[2]->setVisible(false) ;
						m_text_valueMod[2]->setVisible(true) ;
						sprintf(buff,"%d",m_minCarryCash) ;
						setUITextString(m_text_valueMod[2],buff) ;
					}
					else {
						m_minCarryCash = number ;
					}
				}
				//最大携带
				else if (i == 3 )
				{

					//最大谢大小于最小携带时
					if (number < m_minCarryCash)
					{
						PopTipWithBt1("最大携带不得小于最小携带","确定",nullptr) ;
						sprintf(buff,"%d",m_maxCarryCash) ;
						setUITextString(m_text_valueMod[3],buff) ;
					}
					else
					{
						m_maxCarryCash = number ;
					}

					
				}
				//盲注
				else if (i == 4 )
				{
					if (number == 0)
					{
						PopTipWithBt1("小盲不能为0","确定",nullptr) ;
						sprintf(buff,"%d/%d",m_minBlind,m_minBlind*2) ;
						setUITextString(m_text_valueMod[4],buff) ;
					}
					else if (number > 1000)
					{
						PopTipWithBt1("小盲不得大于1000万","确定",nullptr) ;
						sprintf(buff,"%d/%d",m_minBlind,m_minBlind*2) ;
						setUITextString(m_text_valueMod[4],buff) ;
					}
		
					else
					{
						m_minBlind = number ;
						char buff[64] ;
						sprintf(buff,"%d/%d",m_minBlind,m_minBlind*2) ;
						setUITextString(m_text_valueMod[i],buff) ;

						if (((m_minBlind*2) + m_anteMoney) > m_minCarryCash)
						{
							m_minCarryCash = (m_minBlind*2) + m_anteMoney ;

							m_text_valueDeflaut[2]->setVisible(false) ;
							m_text_valueMod[2]->setVisible(true) ;
							sprintf(buff,"%d",m_minCarryCash) ;
							setUITextString(m_text_valueMod[2],buff) ;

							if (m_minCarryCash > m_maxCarryCash)
							{
								m_maxCarryCash = m_minCarryCash ;

								m_text_valueDeflaut[3]->setVisible(false) ;
								m_text_valueMod[3]->setVisible(true) ;
								sprintf(buff,"%d",m_maxCarryCash) ;
								setUITextString(m_text_valueMod[3],buff) ;
							}
						}

					}

					
				}
				//前注
				else if (i == 5 )
				{
					m_anteMoney = number ;

					if (((m_minBlind*2) + m_anteMoney) > m_minCarryCash)
					{
						m_minCarryCash = (m_minBlind*2) + m_anteMoney ;

						m_text_valueDeflaut[2]->setVisible(false) ;
						m_text_valueMod[2]->setVisible(true) ;
						sprintf(buff,"%d",m_minCarryCash) ;
						setUITextString(m_text_valueMod[2],buff) ;

						if (m_minCarryCash > m_maxCarryCash)
						{
							m_maxCarryCash = m_minCarryCash ;

							m_text_valueDeflaut[3]->setVisible(false) ;
							m_text_valueMod[3]->setVisible(true) ;
							sprintf(buff,"%d",m_maxCarryCash) ;
							setUITextString(m_text_valueMod[3],buff) ;
						}
					}


				}
				else if (i == 6 )
				{
					//m_anteMoney = number ;
				}

			}
			else
			{
				if (i==6)
				{
					if (showText !=" " && showText.size()>0)
					{
						m_text_valueDeflaut[i]->setVisible(false) ;
						m_text_valueMod[i]->setVisible(true) ;
						setUITextString(m_text_valueMod[i],showText.c_str()) ;
						m_roomName = showText.c_str() ;
					}
					else
					{
						PopTipWithBt1("请输入房间名","确定",nullptr) ;
					}
				
				}
				else
				{
					PopTipWithBt1("请输入正确数值","确定",nullptr) ;
				}
				
			}




		}
	}
}

void SelfDefinition::CreateRoom()
{

}

void SelfDefinition::initWithDefaultValue()
{
	char buff[64] ;
	m_playerNum = 9 ;
	m_defineFundp = 5 ;

	m_minCollectCash = 10 ; //最小起收
	m_maxCollectCash = 100 ; //最大封顶
	m_minCarryCash = 500 ;//最小携带
	m_maxCarryCash = 5000 ;//最大携带
	m_minBlind = 5;//大小盲
	m_anteMoney = 0;//前注


	sprintf(buff,"%d",m_playerNum) ;
	setUITextString(m_text_playerNum,buff);

	sprintf(buff,"%d%%",m_defineFundp) ;
	setUITextString(m_text_jiJin,buff);

	sprintf(buff,"%d",m_minCollectCash) ;
	setUITextString(m_text_valueDeflaut[0],buff);

	sprintf(buff,"%d",m_maxCollectCash) ;
	setUITextString(m_text_valueDeflaut[1],buff);

	sprintf(buff,"%d",m_minCarryCash) ;
	setUITextString(m_text_valueDeflaut[2],buff);

	sprintf(buff,"%d",m_maxCarryCash) ;
	setUITextString(m_text_valueDeflaut[3],buff);

	sprintf(buff,"%d/%d",m_minBlind,m_minBlind*2) ;
	setUITextString(m_text_valueDeflaut[4],buff);

	sprintf(buff,"%d",m_anteMoney) ;
	setUITextString(m_text_valueDeflaut[5],buff);
	for (int i=0 ;i<7; i++)
	{
		m_text_valueDeflaut[i]->setVisible(true) ;
		m_text_valueMod[i]->setVisible(false) ;
	}

	m_slider_playerNum->setPercent(100) ;
	m_slider_gameJijin->setPercent(100) ;



	cleanTips() ;
}

void SelfDefinition::setVisible(bool bVisible)
{
	if (bVisible)
	{
		callShowAction() ;
	}
	else
	{
		callHideAction() ;
	}
}


void SelfDefinition::onSliderChanged(cocos2d::Ref* sender, cocos2d::ui::SliderEventType controlEvent )
{

	if (controlEvent == SLIDER_PERCENTCHANGED )
	{
		cleanTips() ;

		if (sender == m_slider_playerNum)
		{
			int percent = m_slider_playerNum->getPercent() ;

			m_playerNum = 2+ percent*7/100 ;
			char buff[64] ;
			sprintf(buff,"%d",m_playerNum) ;
			setUITextString(m_text_playerNum,buff);

		}else if ( sender == m_slider_gameJijin)
		{

			int percent = m_slider_gameJijin->getPercent() ;

			m_defineFundp =  percent*5/100 ;
			char buff[64] ;
			sprintf(buff,"%d%%",m_defineFundp) ;
			setUITextString(m_text_jiJin,buff);

		}


	}

}

void SelfDefinition::OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type)
{
	if (type == Button::TouchEventType::ENDED)
	{
		if (pSend == m_buttonTouchAll)
		{
			cleanTips() ;
		}

		else if (pSend == m_btCreatRoom)
		{
			cleanTips() ;

			Json::Value param;

			param["data"]["areaId"] = -1;

			param["data"]["kind"] = m_curRoomType+1;
			param["data"]["playerNum"] = m_playerNum;
			param["data"]["fundP"] = m_defineFundp;
			
			param["data"]["minCollect"] = m_minCollectCash;
			param["data"]["maxCollect"] = m_maxCollectCash;

			param["data"]["minCarry"] = m_minCarryCash;
			param["data"]["maxCarry"] = m_maxCarryCash;

			param["data"]["smallBlind"] = m_minBlind;
			param["data"]["bigBlind"] = m_minBlind*2;
			
			param["data"]["anti"] = m_anteMoney;
			param["data"]["roomName"] = m_roomName;
			SendClientRequest(ClientRequest::kSendVipOpenRoomInfo, param);
			cleanTips() ;
			setVisible(false) ;
		}
		else if (pSend == m_btFund )
		{
			m_imgFund->setVisible(true) ;
			m_imgMinCollect->setVisible(false) ;
			m_imgMaxCollect->setVisible(false) ;
		}
		else if (pSend == m_btMinCollect )
		{
			m_imgFund->setVisible(false) ;
			m_imgMinCollect->setVisible(true) ;
			m_imgMaxCollect->setVisible(false) ;
		}
		else if (pSend == m_btMaxCollect )
		{
			m_imgFund->setVisible(false) ;
			m_imgMinCollect->setVisible(false) ;
			m_imgMaxCollect->setVisible(true) ;
		}
	}
}


void SelfDefinition::cleanTips()
{
	m_imgFund->setVisible(false) ;
	m_imgMinCollect->setVisible(false) ;
	m_imgMaxCollect->setVisible(false) ;
}

void SelfDefinition::doUpdateRoomName(std::string name)
{
	m_roomName = name ;
	setUITextString(m_text_valueDeflaut[6],name) ;
}
