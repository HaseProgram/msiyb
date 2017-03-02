#include "stdafx.h"
#include "thread.h"

Thread::Thread()
{
	this->thread = new OSThread;
}
