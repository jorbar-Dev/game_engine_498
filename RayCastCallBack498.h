#pragma once
#ifndef RAYCASTCALLBACK498_H
#define RAYCASTCALLBACK498_H





#include "box2d/box2d.h"
#include <vector>

class callBack 
{
public:
	b2Vec2 point;
	b2Vec2 normal;
	b2Fixture* callback;
	float fraction;
};

class RayCastCallBack498 : public b2RayCastCallback
{
public: 
	bool return_all = false;
	std::vector<callBack> myCallBacks;
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction ) override 
	{
		if (fixture->GetFilterData().maskBits == 0 || fixture->GetUserData().pointer == NULL) 
		{
			return -1;
		}
		callBack tmp;
		tmp.callback = fixture;
		tmp.point = point;
		tmp.normal = normal;
		tmp.fraction = fraction;
		myCallBacks.push_back(tmp);
		if (return_all) 
		{
			return 1;
		}

		return fraction;
	}



};
#endif // !RAYCASTCALLBACK498_H

