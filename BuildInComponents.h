#pragma once
#include <string>
class BuildInComponents
{
public:
	static inline const std::string Rigidbody_Dec =R"(Rigidbody = {
	OnStart = function(self)
		print("Hello There")
	end,
	})";

};

