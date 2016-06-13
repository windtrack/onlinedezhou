#include "GameRecordList.h"

USING_NS_CC;

GameRecordList::GameRecordList()
{
}

GameRecordList::~GameRecordList()
{
}

bool GameRecordList::init()
{
	if (!GameDialog::init("image/bggamerecordlist.png", true))
	{
		return false;
	}
	return true;
}