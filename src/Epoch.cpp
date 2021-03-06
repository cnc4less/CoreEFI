#include "Epoch.h"
#include "System.h"
#include "Tasks.h"

#include "Params.h"
#include "Strategy.h"

uint32_t Epoch::last;
uint32_t Epoch::counter;

static uint32_t runClock(uint32_t t0, void *data) {
	Epoch::tick(t0);
	return 0;
}

void Epoch::tick(uint32_t t0) {
	uint32_t seconds = TicksToMicros(tdiff32(t0, last)) / 1000000L;

	if (seconds > 0) {
		last += MicrosToTicks(1000000L) * seconds;

#ifndef NOEFI
		uint16_t rpm = getParamUnsigned(SensorRPM);
		uint16_t cranking = getParamUnsigned(ConstCrankingRPM);
		Strategy::setTimer(TimeRunSeconds, rpm >= cranking);
		Strategy::setTimer(TimeIdleSeconds, getParamFloat(SensorTPS) <= 20);
#endif
	}
}

void Epoch::init() {
	last = 0;
	counter = 0;

	taskmgr.addTask(F("Clock"), runClock, 0, 1000);
}

uint16_t Epoch::mem(bool alloced) {
	return alloced ? 0 : sizeof(last) + sizeof(counter);
}

uint32_t Epoch::seconds() {
	return counter;
}

uint32_t Epoch::millis() {
#ifdef ARDUINO
	return ::millis();
#else
	return micros() / 1000;
#endif
}

uint32_t Epoch::micros() {
#ifdef ARDUINO
	return ::micros();
#else
	return TicksToMicros(ticks());
#endif
}

uint32_t Epoch::ticks() {
	return clockTicks();
}
