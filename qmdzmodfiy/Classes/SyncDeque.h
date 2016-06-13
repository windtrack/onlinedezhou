#ifndef __SYNCDEQUE_H__
#define __SYNCDEQUE_H__

#include <deque>
#include <mutex>

template <typename Element>
class SyncDeque
{
public:
	~SyncDeque()
	{
		Clear();
	}

	void Push(const Element& element)
	{
		m_mutex.lock();
		m_deque.push_back(element);
		m_mutex.unlock();
	}

	bool TryPush(const Element& element)
	{
		if (m_mutex.try_lock())
		{
			m_deque.push_back(element);
			m_mutex.unlock();
			return true;
		}
		return false;
	}

	bool Pop(Element& element)
	{
		bool bPoped = false;
		m_mutex.lock();
		if (!m_deque.empty())
		{
			element = m_deque.front();
			m_deque.pop_front();
			bPoped = true;
		}
		m_mutex.unlock();
		return bPoped;
	}

	bool TryPop(Element& element)
	{
		bool bPoped = false;
		if (m_mutex.try_lock())
		{
			if (!m_deque.empty())
			{
				element = m_deque.front();
				m_deque.pop_front();
				bPoped = true;
			}
			m_mutex.unlock();
		}
		return bPoped;
	}

	void Clear()
	{
		m_mutex.lock();
		m_deque.clear();
		m_mutex.unlock();
	}


private:
	std::deque<Element> m_deque;
	std::mutex m_mutex;
};

#endif