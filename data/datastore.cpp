#include "datastore.h"

#include <singleton.h>

void Snapshot::clone(const DataStore& source)
{
	memcpy(this->fparam, source.fparam, FPARAM_COUNT * sizeof(float));
	memcpy(this->uparam, source.uparam, UPARAM_COUNT * sizeof(uint32_t));
	watchdogs = source.watchdogs;
	time = Singleton::get()->timer0.get_us_time();
}
