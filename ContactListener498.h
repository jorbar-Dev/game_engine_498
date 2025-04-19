#pragma once
#ifndef CONTACTLISTENER498_H
#define CONTACTLISTENER498_H


#include "box2d/box2d.h"
#include "Actor_C.h";
#include "Rigidbody.h"
#include "collision.h";

class ContactListener498 : public b2ContactListener
{

public:

	void BeginContact(b2Contact* contact) override
	{
		Actor_C* ptr = reinterpret_cast<Actor_C*>(contact->GetFixtureA()->GetUserData().pointer);
		Actor_C* ptr2 = reinterpret_cast<Actor_C*>(contact->GetFixtureB()->GetUserData().pointer);
		collision col;
		col.other = ptr2;
		b2WorldManifold man;
		contact->GetWorldManifold(&man);
		col.point = man.points[0];
		col.normal = man.normal;
		col.relative_velocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity() - contact->GetFixtureB()->GetBody()->GetLinearVelocity();
		if (contact->GetFixtureA()->GetFilterData().categoryBits == 1)
		{

			ptr->OnCollisionEnter(col);
			col.other = ptr;
			ptr2->OnCollisionEnter(col);

		}
		else
		{
			col.point = { -999.0f, -999.0f };
			col.normal = { -999.0f, -999.0f };
			ptr->OnTriggerEnter(col);
			col.other = ptr;
			ptr2->OnTriggerEnter(col);
		}
	}

	void EndContact(b2Contact* contact) override
	{
		Actor_C* ptr = reinterpret_cast<Actor_C*>(contact->GetFixtureA()->GetUserData().pointer);
		Actor_C* ptr2 = reinterpret_cast<Actor_C*>(contact->GetFixtureB()->GetUserData().pointer);
		collision col;
		col.other = ptr2;
		b2WorldManifold man;
		contact->GetWorldManifold(&man);
		col.point = { -999.0f, -999.0f };
		col.normal = { -999.0f, -999.0f };
		col.relative_velocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity() - contact->GetFixtureB()->GetBody()->GetLinearVelocity();

		if (contact->GetFixtureA()->GetFilterData().categoryBits == 1) 
		{

			ptr->OnCollisionExit(col);
			col.other = ptr;
			ptr2->OnCollisionExit(col);
		
		}
		else 
		{
			ptr->OnTriggerExit(col);
			col.other = ptr;
			ptr2->OnTriggerExit(col);
		}


	}

};

/*
* 
*/

#endif