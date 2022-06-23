#include "Easing.h"

float Easing::DefaultInCubic(float x)
{
	return x * x * x;
}

float Easing::InCubic(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInCubic(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultOutCubic(float x)
{
	return 1 - std::pow(1 - x, 3);
}

float Easing::OutCubic(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultOutCubic(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInOutCubic(float x)
{
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

float Easing::InOutCubic(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInOutCubic(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInElastic(float x)
{
	float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

float Easing::InElastic(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInElastic(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultOutElastic(float x)
{
	float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

float Easing::OutElastic(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultOutElastic(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInOutElastic(float x)
{
	float c5 = (2 * PI) / 4.5;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
		: (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}

float Easing::InOutElastic(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInOutElastic(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultOutBounce(float x)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	return x < 1 / d1 ?
		n1 * x * x :
		x < 2 / d1 ?
		n1 * (x -= 1.5 / d1) * x + 0.75 :
		x < 2.5 / d1 ?
		n1 * (x -= 2.25 / d1) * x + 0.9375 : n1 * (x -= 2.625 / d1) * x + 0.984375;
}

float Easing::OutBounce(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultOutBounce(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInBounce(float x)
{
	return 1 - DefaultOutBounce(1 - x);
}

float Easing::InBounce(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInBounce(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInOutBounce(float x)
{
	return x < 0.5
		? (1 - DefaultOutBounce(1 - 2 * x)) / 2
		: (1 + DefaultOutBounce(2 * x - 1)) / 2;
}

float Easing::InOutBounce(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInOutBounce(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInBack(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	float x2 = x * x;
	float x3 = x2 * x;

	return c3 * x3 - c1 * x2;
}

float Easing::InBack(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInBack(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultOutBack(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	float pow2 = (x - 1) * (x - 1);
	float pow3 = pow2 * (x - 1);
	
	return 1 + c3 * pow3 + c1 * pow2;
}

float Easing::OutBack(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultOutBack(x);
	float ret = c * v + b;
	return ret;
}

float Easing::DefaultInOutBack(float x)
{
	const float c1 = 1.70158;
	const float c2 = c1 * 1.525;

	return x < 0.5
		? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float Easing::InOutBack(float t, float b, float c, float d)
{
	float x = t / d;
	float v = DefaultInOutBack(x);
	float ret = c * v + b;
	return ret;
}
