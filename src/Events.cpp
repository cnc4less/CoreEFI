// copyright to me, released under GPL V3

#include "Events.h"
#include "Decoder.h"
#include "Broker.h"
#include "Metrics.h"
#include "Params.h"
#include "GPIO.h"
#include "Schedule.h"
#include "Tasks.h"
#include "Timers.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

enum {
	CountLate0,
	CountLate1,
	CountLate2,
	CountLate3,
	CountLate4,
	CountLateX,
	CountLates, // 6

	CountEvents,
	CountCalls,
	CountSwaps, // 9

	CountPast,
	CountJumps1,
	CountJumps2, // 12

	CountResets,
	CountSyncs,
	CountReturns, // 15

	CountDelays,
	CountPins,

	HistMax,
};

static const uint8_t MAX_EVENTS = MaxCylinders * 4;
static const uint8_t BucketDivisor = 1;

class BitEvent {
public:

	uint32_t act;
	int32_t late;
	int16_t drift;
	uint8_t calcs;
	uint8_t calls;

	inline void clear() volatile {
		act = 0;
		late = 0;
		drift = 0;
		calcs = 0;
		calls = 0;
	}
};

static volatile struct {
	BitEvent queue[MAX_EVENTS];
	BitSchedule schedule1;
	BitSchedule schedule2;
	volatile BitSchedule *current;
	bool swap;

	uint16_t hist[HistMax];

	MetricsMinMax32 late;
	uint32_t next;
	int8_t idx;
	int8_t on;
	bool valid;
	bool sync;

	inline void reset() volatile {
		next = 0;
		on = 0;
		idx = 0;

		valid = false;
		sync = false;
		swap = false;

		current = &schedule1;
	}

	inline void addHist(uint8_t h) volatile {
		if (h < HistMax && hist[h] + 1 != 0)
			hist[h]++;
	}

	inline volatile BitEvent *getEvent() volatile {
		if (idx >= current->size()) {
			sync = false;
			on = 0;
			idx = 0;
			checkSwap();
		}

		return queue + idx;
	}

	inline volatile BitPlan *getPlan() volatile {
		return current->getPlan(idx);
	}

	inline void calcNext(uint32_t now, volatile BitEvent *e, volatile BitPlan *p) volatile {
		uint32_t max = decoder.getTicks();
		uint32_t val = p->calcTicks(max, true);

		e->clear();
		e->calcs++;
		p->setMax(max);

		next = decoder.getTDC() + val;
		int32_t diff = tdiff32(next, now);
		int32_t mid = max >> 1;

		if (idx == 0 && diff < -mid) { // expected phase change, computing first event before tdc
			next += max;
			diff = tdiff32(next, now);
		}

		if (diff > mid) { // phasing, can happen with decel only?
			addHist(CountJumps1);
		} else if (diff < -mid) {
			addHist(CountJumps2);
		}

		if (diff < 1)
			addHist(CountPast);
	}

	inline void calcNext(uint32_t now) volatile {
		volatile BitEvent *e = getEvent();
		volatile BitPlan *p = getPlan();
		calcNext(now, e, p);
	}

	inline void checkSwap() volatile {
		if (swap) {
			current = current == &schedule2 ? &schedule1 : &schedule2;
			addHist(CountSwaps);
			swap = false;
		}
	}

	inline uint32_t runEvent(uint32_t now, uint8_t maxdelay, uint16_t jitter) volatile {
		volatile BitEvent *e = getEvent();
		volatile BitPlan *p = getPlan();

		int32_t delay = tdiff32(next, now);

		if (!e->calcs || (!p->hasOff() && delay > jitter)) { // relative events absolute
			calcNext(now, e, p);
			delay = tdiff32(next, now);
		}

		addHist(CountCalls);

		e->calls++;

		if (delay <= jitter) {
			if (delay > 0 && delay <= maxdelay) {
				//delayTicks(delay);
				//now += delay;
				addHist(CountDelays);
			}

			e->late = tdiff32(now, next);
			late.set(e->late);

			e->act = tdiff32(now, decoder.getTDC());
			e->drift = e->act - p->getPos();

			if (!sync)
				addHist(CountSyncs);

			addHist(CountEvents);

			if (e->late) {
				int16_t idx = abs(e->late);

				if (idx >= (int16_t) (TICKTOUS >> 1)) {
					idx /= MicrosToTicks(BucketDivisor);

					addHist(CountLates);

					if (idx < CountLateX)
						addHist(idx);
					else
						addHist(CountLateX);
				}
			}

			PinId id = (PinId) (p->getPin() + 1);
			bool isSet = gpio.isPinSet(id);

			if (p->isHi() == isSet)
				addHist(CountPins);

			if (p->isHi()) {
				gpio.setPin(id, true);
				on++;
			} else {
				gpio.setPin(id, false);
				on--;
			}

			idx++;

			now = clockTicks();
			calcNext(now);
		}

		return next;
	}
} events;

volatile BitSchedule *BitSchedule::getSchedule() {
	if (events.swap)
		return 0;
	return events.current == &events.schedule2 ? &events.schedule1 : &events.schedule2;
}

void BitSchedule::swapSchedule() {
	events.swap = true;

	if (!events.current->size())
		events.checkSwap();
}

void setEventTDC(uint32_t tdc) {
	if (events.current->size()) {
		int32_t diff = tdiff32(events.next, tdc);

		if (!events.valid || events.idx != 0 || diff < 0) {
			events.idx = 0;
			events.calcNext(tdc);
			events.addHist(CountResets);
		}

		events.valid = true;
	}

	events.sync = true;
}

uint32_t BitPlan::runEvents(uint32_t now, uint8_t maxdelay, uint16_t jitter) {
	if (events.valid)
		return events.runEvent(now, maxdelay, jitter);

	events.addHist(CountReturns);

	return now + MicrosToTicks(10000);
}

void BitPlan::refreshEvents() {
	if (events.valid && getParamUnsigned(SensorRPM) <= 0)
		events.reset();
}

void BitPlan::sendEventStatus(Buffer &send) {
	int16_t total = events.hist[CountEvents];

	if (total) {
		send.p1(F("events"));

		send.json(F("idx"), events.idx);
		send.json(F("on"), events.on);
		send.json(F("rpm"), getParamUnsigned(SensorRPM));

		if (events.late.isValid()) {
			float minlate = TicksToMicrosf(events.late.getMin());
			float maxlate = TicksToMicrosf(events.late.getMax());
			send.jsonf(F("-late"), minlate);
			send.jsonf(F("+late"), maxlate);

			uint16_t us = TicksToMicros(decoder.getTicks() >> 1);
			send.jsonf(F("-deg"), us == 0 || events.late.getMin() == 0 ? 0 : 360.0f * minlate / us);
			send.jsonf(F("+deg"), us == 0 || events.late.getMax() == 0 ? 0 : 360.0f * maxlate / us);
		}

		if (total && events.valid) {
			int16_t lates = events.hist[CountLates];
			send.jsonf(F("late%"), 100.0f * lates / total);
			uint8_t bucket = BucketDivisor;

			for (uint8_t i = CountLate0; i <= CountLate4; i++) {
				lates -= events.hist[i];

				if (lates)
					send.json(bucket, 100.0f * lates / total);

				bucket += BucketDivisor;
			}
		}

		MetricsHist::sendHist(send, F("counts"), events.hist, HistMax);
		events.late.reset();

		send.p2();
	}
}

void BitPlan::sendEventList(Buffer &send) {
	volatile BitSchedule *current = events.current;
	uint32_t last = 0;

	bool monitoring = isParamSet(FlagIsMonitoring);

	for (uint8_t i = 0; i < events.current->size(); i++) {
		volatile BitEvent *e = events.queue + i;
		volatile BitPlan *p = current->getPlan(i);

		send.p1(F("evt"));

		send.json(F("i"), i);
		send.json(F("cyl"), p->getCyl());
		send.json(F("pin"), p->getPin());
		send.json(F("hi"), p->isHi());
		send.json(F("max"), p->getMax());
		send.json(F("pos"), p->getPos());

		if (!monitoring) {
			uint32_t rel = p->getPos() - last;

			send.json(F("rel"), rel);
			send.json(F("id"), p->getId());
			send.json(F("ang"), p->getAng());
			send.json(F("off"), p->getOff());

			send.json(F("calls"), e->calls);
			send.json(F("calcs"), e->calcs);
			send.json(F("drift"), e->drift);
			send.json(F("late"), e->late);
			send.json(F("act"), e->act);
			send.json(F("events"), events.hist[CountEvents]);

			last = p->getPos();
		}

		send.p2();
	}
}

static uint32_t timercb(TimerId id, uint32_t now, void *data) {
	return BitPlan::runEvents(now, 0, 0);
}

static void brokercb(Buffer &send, BrokerEvent &be, void *data) {
	if (be.isMatch(F("s")))
		BitPlan::sendEventStatus(send);
	else if (be.isMatch(F("l")))
		BitPlan::sendEventList(send);
	else
		send.nl("status|list");
}

#ifdef linux
static uint32_t taskcb(uint32_t now, void *data) {
	int32_t ticks = BitPlan::runEvents(now, 0, 5);
	ticks = tdiff32(ticks, clockTicks());
	ticks = max(ticks, 1);
	return TicksToMicros(ticks);
}
#endif

void BitPlan::init() {
	events.reset();

	events.schedule1.reset();
	events.schedule2.reset();

	for (uint8_t i = 0; i < MAX_EVENTS; i++)
		events.queue[i].clear();

	refreshEvents();

#ifdef linux
	taskmgr.addTask(F("Events"), taskcb, 0, 1000);
#endif

	broker.add(brokercb, 0, F("e"));

	Timers::initTimer(TimerId5, timercb, 0);
}

uint16_t BitPlan::mem(bool alloced) {
	return alloced ? 0 : sizeof(events);
}
