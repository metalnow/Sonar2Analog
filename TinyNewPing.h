#ifndef TinyNewPing_h
#define TinyNewPing_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// Shoudln't need to changed these values unless you have a specific need to do so.
#define MAX_SENSOR_DISTANCE 450 // Maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back.
#define US_ROUNDTRIP_IN 146     // Microseconds (uS) it takes sound to travel round-trip 1 inch (2 inches total), uses integer to save compiled code space.
#define US_ROUNDTRIP_CM 57      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
//#define DISABLE_ONE_PIN false   // Set to "true" to save up to 26 bytes of compiled code space if you're not using one pin sensor connections.
#define DISABLE_ONE_PIN true   // Set to "true" to save up to 26 bytes of compiled code space if you're not using one pin sensor connections.

// Probably shoudln't change these values unless you really know what you're doing.
#define NO_ECHO 255               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance.
#define MAX_SENSOR_DELAY 18000  // Maximum uS it takes for sensor to start the ping (SRF06 is the highest measured, just under 18ms).
#define ECHO_TIMER_FREQ 24      // Frequency to check for a ping echo (every 24uS is about 0.4cm accuracy).
#define PING_MEDIAN_DELAY 29    // Millisecond delay between pings in the ping_median method.

// Conversion from uS to distance (round result to nearest cm or inch).
#define NewPingConvert(echoTime, conversionFactor) (max((echoTime + conversionFactor / 2) / conversionFactor, (echoTime ? 1 : 0)))


class NewPing {
	public:
		NewPing(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance = MAX_SENSOR_DISTANCE);
		unsigned int ping();
		unsigned int ping_in();
		unsigned int ping_cm();
		unsigned int ping_median(uint8_t it = 5);
		unsigned int convert_in(unsigned int echoTime);
		unsigned int convert_cm(unsigned int echoTime);
		void ping_timer(void (*userFunc)(void));
		boolean check_timer();
		unsigned long ping_result;
		static void timer_us(unsigned int frequency, void (*userFunc)(void));
		static void timer_ms(unsigned long frequency, void (*userFunc)(void));
		static void timer_stop();
	private:
		boolean ping_trigger();
		boolean ping_wait_timer();
		uint8_t _triggerBit;
		uint8_t _echoBit;
		volatile uint8_t *_triggerOutput;
		volatile uint8_t *_triggerMode;
		volatile uint8_t *_echoInput;
		unsigned int _maxEchoTime;
		unsigned long _max_time;
		static void timer_setup();
		static void timer_ms_cntdwn();
};

#endif




