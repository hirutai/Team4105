#pragma once
#include <iostream>

namespace Easing
{
	const double PI = 3.14159265358979323846;

	// EasingÇÃä÷êîÇçÏÇÈ
	double ease_in_cubic(double x);

	double EaseInCubic(double t, double b, double c, double d);

	double ease_out_cubic(double x);

	double EaseOutCubic(double t, double b, double c, double d);

	double ease_inout_cubic(double x);
	
	double EaseInOutCubic(double t, double b, double c, double d);
	
	double ease_in_elastic(double x);

	double EaseInElastic(double t, double b, double c, double d);
	
	double ease_out_elastic(double x);

	double EaseOutElastic(double t, double b, double c, double d);

	double ease_inout_elastic(double x);
	
	double EaseInOutElastic(double t, double b, double c, double d);

	double ease_out_bounce(double x);
	
	double EaseOutBounce(double t, double b, double c, double d);

	double ease_in_bounce(double x);

	double EaseInBounce(double t, double b, double c, double d);
	
	double ease_inout_bounce(double x);
	
	double EaseInOutBounce(double t, double b, double c, double d);

	double ease_in_back(double x);

	double EaseInBack(double t, double b, double c, double d);

	double ease_out_back(double x);

	double EaseOutBack(double t, double b, double c, double d);

	double ease_inout_back(double x);
	
	double EaseInOutBack(double t, double b, double c, double d);
	
}