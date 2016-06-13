#include "GameTipsManager.h"


USING_NS_CC ;

GameTipsManager* GameTipsManager::_instance = 0;
GameTipsManager* GameTipsManager::getInstance()
{
	if (_instance == 0)
	{
		_instance = new GameTipsManager() ;
		_instance->init() ;
	}

	return _instance ;
}
	

void GameTipsManager::init()
{
	m_isShowTips = false ;
}

void GameTipsManager::addTips(cocos2d::Node* tips)
{
	tips->setVisible(false) ;
	m_verTips.pushBack(tips) ;
}

void GameTipsManager::callNextTips()
{
	if (!m_isShowTips)
	{
		if (m_verTips.size()>0)
		{
			m_isShowTips = true ;
			//新提示展示  层级高的 优先展示
			int minorder = -999 ;
			int index ;
			for (int i=0;i<m_verTips.size();i++)
			{
				Node* tips = m_verTips.at(i) ;
				if (tips->getZOrder()>minorder)
				{
					minorder = tips->getZOrder() ;
					index = i ;
				}
			}

			Node* tips = m_verTips.at(index) ;
			tips->setVisible(true) ;
			m_verTips.eraseObject(tips) ;
		}
	}

}

void GameTipsManager::hideTips()
{

	m_isShowTips = false ;
	callNextTips() ;
	
}

void GameTipsManager::clean()
{
	init() ;
	for (int i=0;i<m_verTips.size();i++)
	{
		Node* node = m_verTips.at(i) ;
		node->setVisible(false) ;
	}
	m_verTips.clear() ;
}