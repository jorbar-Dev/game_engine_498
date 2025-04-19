Mover = {
	
	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
	end,

	OnUpdate = function(self)
		self.rb:SetPosition(Vector2(Application.GetFrame() * 0.02, Application.GetFrame() * -0.02))
		self.rb:SetRotation(Application.GetFrame() * 4)

		local pos = self.rb:GetPosition()
		local rot = self.rb:GetRotation()

		Debug.Log("frame " .. Application.GetFrame() .. " : pos=(" .. pos.x .. "," .. pos.y .. ") rot=" .. rot)
	end
}

