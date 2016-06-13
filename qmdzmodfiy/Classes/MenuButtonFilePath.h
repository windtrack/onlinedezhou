#ifndef _MENUBUTTONFILEPATH_H_

#define _MENUBUTTONFILEPATH_H_
#include "cocos2d.h"
static const char* sButtonPath[11][3] =
{
    ////通用按钮
	{
		"commonbt/1.png","commonbt/2.png","commonbt/3.png",
	},
    {
        "commonbt/4.png","commonbt/5.png","commonbt/6.png",
    },
    {
        "commonbt/7.png","commonbt/8.png","commonbt/9.png",
    },
    {
        "commonbt/10.png","commonbt/11.png","commonbt/9.png",
    },
	{
		"commonbt/12.png","commonbt/13.png","commonbt/14.png",
	},
    {
        "commonbt/1.png","commonbt/2.png","commonbt/30.png",
    }
} ;

static const cocos2d::Rect sBtRect[11][2]= 
{
    //通用按钮
	{
		cocos2d::Rect(0,0,50,72),cocos2d::Rect(15,15,20,42)
	},
    {
        cocos2d::Rect(0,0,50,72),cocos2d::Rect(15,15,20,42)
    },
    {
        cocos2d::Rect(0,0,90,86),cocos2d::Rect(27,27,36,32)
    },
    {
        cocos2d::Rect(0,0,90,86),cocos2d::Rect(27,27,36,32)
    },
	{
		cocos2d::Rect(0,0,52,74),cocos2d::Rect(15,15,20,42)
	},
} ;
#endif