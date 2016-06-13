#ifndef __RADIOGROUP_H__
#define __RADIOGROUP_H__

#include <vector>
#include <functional>

typedef std::function<void(bool)> FuncActivate;

class RadioGroupItem
{
public:
	RadioGroupItem(FuncActivate funcActivate = nullptr)
		: m_funcActivate(funcActivate)
	{
	}

	void Activate(bool bActivate)
	{
		if (m_funcActivate)
		{
			m_funcActivate(bActivate);
		}
	}

	FuncActivate m_funcActivate;
};

class RadioGroup
{
public:
	RadioGroup()
		: m_iActivated(-1)
	{
	}

	void AddGroupItem(const RadioGroupItem& groupItem)
	{
		m_vecGroupItem.push_back(groupItem);
	}

	void Activate(unsigned uIdx)
	{
		if (uIdx >= m_vecGroupItem.size())
		{
			return;
		}
		if (m_iActivated != uIdx)
		{
			if (m_iActivated >= 0)
			{
				m_vecGroupItem[m_iActivated].Activate(false);
			}
			m_iActivated = uIdx;
			m_vecGroupItem[m_iActivated].Activate(true);
		}
	}

	int GetActivated()
	{
		return m_iActivated;
	}

private:
	int m_iActivated;
	std::vector<RadioGroupItem> m_vecGroupItem;
};

#endif