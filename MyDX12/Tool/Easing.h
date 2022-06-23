#pragma once
#include <iostream>

namespace Easing
{
	const float PI = 3.141592;
	// Easing‚ÌŠÖ”‚ğì‚é
	float DefaultInCubic(float x);
	float InCubic(float t, float b, float c, float d);

	float DefaultOutCubic(float x);
	float OutCubic(float t, float b, float c, float d);

	float DefaultInOutCubic(float x);
	float InOutCubic(float t, float b, float c, float d);

	float DefaultInElastic(float x);
	float InElastic(float t, float b, float c, float d);
	
	float DefaultOutElastic(float x);
	float OutElastic(float t, float b, float c, float d);

	float DefaultInOutElastic(float x);
	float InOutElastic(float t, float b, float c, float d);

	float DefaultOutBounce(float x);
	float OutBounce(float t, float b, float c, float d);

	float DefaultInBounce(float x);
	float InBounce(float t, float b, float c, float d);
	
	float DefaultInOutBounce(float x);
	float InOutBounce(float t, float b, float c, float d);

	float DefaultInBack(float x);
	float InBack(float t, float b, float c, float d);

	float DefaultOutBack(float x);
	float OutBack(float t, float b, float c, float d);

	float DefaultInOutBack(float x);
	float InOutBack(float t, float b, float c, float d);
}