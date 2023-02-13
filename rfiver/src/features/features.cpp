#include <include.h>

c_feature_mgr* f::mgr{ };

void f::start()
{
	mgr->add(f::esp);
	mgr->add(f::aim);
	mgr->add(f::trigger);
	mgr->add(f::misc);
	mgr->add(f::rapidfire);
	mgr->add(f::specs);
}
