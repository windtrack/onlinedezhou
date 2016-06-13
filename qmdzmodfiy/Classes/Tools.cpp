#include "Tools.h"
#include "LogonLayerEx.h"
USING_NS_CC;

Node* Tools::getWidgetByName(cocos2d::Node* root, const std::string& name)
{
	CCASSERT(name.length() != 0, "Invalid name");
	if( root->getName().compare(name) == 0)
	{
		return root;
	}	
	for (const auto& child : root->getChildren())
	{
		if( child->getName().compare(name) == 0)
		{
			return child;
		}
		for (const auto& cchild : child->getChildren())
		{
			Node* node = getWidgetByName(cchild,name);
			if (node)
			{
				return node ;
			}	
		}
	}
	return nullptr;
}



void Tools::callScaleBig(cocos2d::Node* node)
{
	//ActionInterval* scaleTO = ScaleTo::create(0.5,1.2) ;

	//CallFunc* callFun = CallFunc::create(this,CC_CALLBACK_0(Tools::callScaleSmall,this,node))


}


void Tools::callScaleSmall(cocos2d::Node* node)
{

}

DataLoading* Tools::creatDataLoading(cocos2d::Node* node,float x,float y,int zOrder)
{

	DataLoading*	s_dataLoading = DataLoading::create();
	s_dataLoading -> ignoreAnchorPointForPosition(false);
	s_dataLoading -> setAnchorPoint(Point(0.5,0.5));
	s_dataLoading->setPosition(x, y);
	s_dataLoading->setVisible(false);
	node->addChild(s_dataLoading, 99);

	return s_dataLoading ;
}

void Tools::callScaleAction(cocos2d::Node* node)
{
	node->setScale(0.85f) ;
	ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
	CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
	node->setCascadeOpacityEnabled(true) ;
	node->setOpacity(0);
	ActionInterval * fadein = FadeTo::create(0.3f,255) ;
	node->runAction(Spawn::create(easeBounceOut,fadein,NULL)) ;

}

void Tools::callHideScaleAction(cocos2d::Node* node, std::function<void()> &funcation)
{
	ActionInterval* scaleTo = ScaleTo::create(0.3f,0.85f) ;
	CCActionInterval * easeBounceOut = CCEaseBackIn ::create(scaleTo);
	node->setCascadeOpacityEnabled(true) ;
	node->setOpacity(255);
	ActionInterval * fadeout = FadeTo::create(0.3f,0) ;

	auto fun = funcation ;
	auto callback = [fun](){
		fun ;
	};

	node->runAction(Sequence::create(Spawn::create(easeBounceOut,fadeout,NULL),CallFunc::create(callback),NULL)) ;

}

void Tools::callScaleAction(cocos2d::Node* node,bool useBig,float bigtime,float smalltime,float scaleBig,float scalesmall)
{

	ActionInterval* scaleTo = ScaleTo::create(useBig?bigtime:smalltime,useBig?scaleBig:scalesmall) ;
	node->runAction(Sequence::create(scaleTo, CallFunc::create([node,useBig,bigtime,smalltime,scaleBig,scalesmall](){
		callScaleAction(node,!useBig,bigtime,smalltime,scaleBig,scalesmall);
	}), nullptr)) ;

}

/************************************************************************/
/* 路径  父节点 坐标  层级                                                                     */
/************************************************************************/
void Tools::creatParticleAddTo(std::string path ,cocos2d::Node* nodep,cocos2d::Point pos,int Zorder)
{
	ParticleSystem* m_emitter1 = ParticleSystemQuad::create(path);
	m_emitter1->retain();
	ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(m_emitter1->getTexture());
	batch->addChild(m_emitter1);
	nodep->addChild(batch, Zorder);
	batch->setPosition(pos);
	m_emitter1->release();
}





bool Tools::IsValidChar(char ch)
 {
	 if( (ch>=97) && (ch<=122) ) //26个小写字母
		 return true;
	 if( (ch>=65) && (ch<=90) ) //26个大写字母
		 return true;
	 if((ch>=48) && (ch<=57)) //0~9
		 return true;
	 if( ch==95 || ch==45 || ch==46 || ch==64 ) //_-.@
		 return true;
	 return false;
 }
 bool Tools::IsValidEmail(std::string strEmail)
 {
	 if( strEmail.length()<5 ) //26个小写字母
		 return false;

	 char ch = strEmail[0];
	 if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) )
	 {
		 int atCount =0;
		 int atPos = 0;
		 int dotCount = 0;
		 for(int i=1;i<strEmail.length();i++) //0已经判断过了，从1开始
		 {
			 ch = strEmail[i];
			 if(IsValidChar(ch))
			 {
				 if(ch==64) //"@"
				 {
					 atCount ++;
					 atPos = i;
				 }
				 else if( (atCount>0) && (ch==46) )//@符号后的"."号
					 dotCount ++;
			 }
			 else
				 return false;
		 }
		 //6. 结尾不得是字符“@”或者“.”
		 if( ch == 46 )
			 return false;
		 //2. 必须包含一个并且只有一个符号“@”
		 //3. @后必须包含至少一个至多三个符号“.”
		 if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
			 return false;
		 //5. 不允许出现“@.”或者.@
		 int x,y;
		 x=strEmail.find("@.");
		 y=strEmail.find(".@");
		 if( x>0 || y>0 )
		 {
			 return false;
		 }
		 return true;
	 }
	 return false;
 }



 bool Tools::is_number(std::string str)
 {

	 if( str.length()>=12 ) 
		 return false;
	 if(str.c_str()[0]!=45)
	 {
		 for(int i = 0; i < str.length(); i++)
		 {
			 if( str.c_str()[i] < '0' || str.c_str()[i] > '9' )
			 { return false;}
		 }
		 return true;
	 }
	 else
	 {
		 for(int i = 1; i < str.length(); i++)
		 {
			 if( str.c_str()[i] < '0' || str.c_str()[i] > '9' )
			 { return false;}
		 }
		 return true;
	 }
 }