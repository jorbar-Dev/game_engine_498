CollisionResponder = {

	OnCollisionEnter = function(self, collision)
		Debug.Log("OnCollisionEnter() called on frame " .. Application.GetFrame() .. " on actor " .. self.actor:GetName())
		Debug.Log("collision other : " .. collision.other:GetName())
		Debug.Log("collision point : (" .. collision.point.x .. "," .. collision.point.y .. ")")
		Debug.Log("collision relative_velocity : (" .. collision.relative_velocity.x .. "," .. collision.relative_velocity.y .. ")")
		Debug.Log("collision normal : (" .. collision.normal.x .. "," .. collision.normal.y .. ")")
		Debug.Log("")
	end,

	OnCollisionExit = function(self, collision)
		Debug.Log("OnCollisionExit() called on frame " .. Application.GetFrame() .. " on actor " .. self.actor:GetName())
		Debug.Log("collision other : " .. collision.other:GetName())
		Debug.Log("collision point : (" .. collision.point.x .. "," .. collision.point.y .. ")")
		Debug.Log("collision relative_velocity : (" .. collision.relative_velocity.x .. "," .. collision.relative_velocity.y .. ")")
		Debug.Log("collision normal : (" .. collision.normal.x .. "," .. collision.normal.y .. ")")
		Debug.Log("")
	end
}

