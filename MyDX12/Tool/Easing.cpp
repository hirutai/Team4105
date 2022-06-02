#include "Easing.h"

double Easing::ease_in_cubic(double x)
{
	return x * x * x;
}

double Easing::EaseInCubic(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_in_cubic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_out_cubic(double x)
{
	return 1 - std::pow(1 - x, 3);
}

double Easing::EaseOutCubic(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_out_cubic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_inout_cubic(double x)
{
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

double Easing::EaseInOutCubic(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_inout_cubic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_in_elastic(double x)
{
	double  c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

double Easing::EaseInElastic(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_in_elastic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_out_elastic(double x)
{
	double c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

double Easing::EaseOutElastic(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_out_elastic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_inout_elastic(double x)
{
	double c5 = (2 * PI) / 4.5;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
		: (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}

double Easing::EaseInOutElastic(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_inout_elastic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_out_bounce(double x)
{
	const double n1 = 7.5625;
	const double d1 = 2.75;

	return x < 1 / d1 ?
		n1 * x * x :
		x < 2 / d1 ?
		n1 * (x -= 1.5 / d1) * x + 0.75 :
		x < 2.5 / d1 ?
		n1 * (x -= 2.25 / d1) * x + 0.9375 : n1 * (x -= 2.625 / d1) * x + 0.984375;
}

double Easing::EaseOutBounce(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_out_bounce(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_in_bounce(double x)
{
	return 1 - ease_out_bounce(1 - x);
}

double Easing::EaseInBounce(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_in_bounce(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_inout_bounce(double x)
{
	return x < 0.5
		? (1 - ease_out_bounce(1 - 2 * x)) / 2
		: (1 + ease_out_bounce(2 * x - 1)) / 2;
}

double Easing::EaseInOutBounce(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_inout_bounce(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_in_back(double x)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	double x2 = x * x;
	double x3 = x2 * x;

	return c3 * x3 - c1 * x2;
}

double Easing::EaseInBack(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_in_back(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_out_back(double x)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	double pow2 = (x - 1) * (x - 1);
	double pow3 = pow2 * (x - 1);
	
	return 1 + c3 * pow3 + c1 * pow2;
}

double Easing::EaseOutBack(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_out_back(x);
	double ret = c * v + b;
	return ret;
}

double Easing::ease_inout_back(double x)
{
	const double c1 = 1.70158;
	const double c2 = c1 * 1.525;

	return x < 0.5
		? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

double Easing::EaseInOutBack(double t, double b, double c, double d)
{
	double x = t / d;
	double v = ease_inout_back(x);
	double ret = c * v + b;
	return ret;
}
