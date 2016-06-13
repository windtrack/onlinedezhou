#ifndef __SCOPEGUARD_H__
#define __SCOPEGUARD_H__

#include <functional>

class ScopeGuard
{
public:
    explicit ScopeGuard(std::function<void()> onExitScope)
        : m_onExitScope(onExitScope)
		, m_bDismissed(false)
    { }
 
    ~ScopeGuard()
    {
        if(!m_bDismissed)
        {
            m_onExitScope();
        }
    }
 
    void Dismiss()
    {
        m_bDismissed = true;
    }
 
private:
    std::function<void()> m_onExitScope;
    bool m_bDismissed;
 
private: // noncopyable
    ScopeGuard(ScopeGuard const&);
    ScopeGuard& operator=(ScopeGuard const&);
};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)
#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

#endif