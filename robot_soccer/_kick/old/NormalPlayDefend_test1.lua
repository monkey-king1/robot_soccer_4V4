gPlayTable.CreatePlay{

firstState = "initState",

["initState"] = {
	switch = function()
		if CIsGetBall("Kicker") then
			return "shoot"
		end
	end,
	Kicker  = task.KickerTask("ReceiveBall_usr"),
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie  = task.Goalie()
},

["shoot"] = {
	switch = function()
		if CIsGetBall("Kicker") then
			return "initState"
		end
	end,
	Kicker  = task.KickerTask("Shoot_usr"),
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie  = task.Goalie()
},


name = "NormalPlayDefend_test1"
}