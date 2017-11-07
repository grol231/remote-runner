#ifndef __Application_H__
#define __Application_H__

#include <memory>
#include "config.h"

namespace Application
{
	void Initialize(std::shared_ptr<Config> config);
	void DoInitialize(std::shared_ptr<Config> config);
    const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;
}
#endif
