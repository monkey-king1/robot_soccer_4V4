--desc: 
gPlayTable.CreatePlay{
firstState = "GetBall",

["GetBall"] = {
	switch = function()
		if CIsBallKick("Kicker")  then 
			return "delay"
		end
	end,
	Receiver   = task.ReceiverTask("GoRecPos_Back_0"),
	Kicker = task.KickerTask("GetBall_Back"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["delay"] = {
	switch = function()
		if  CIsGetBall("Receiver")  then --CIsGetBall("Kicker")
			return "Shoot"
		end
	end,
	Receiver   = task.ReceiverTask("ReceiveBall_usr"),
	Kicker = task.KickerTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["Shoot"] = {
	switch = function()
		if CIsBallKick("Receiver") then
			return "finish"
		end
	end,
	Kicker = task.KickerTask("receiverdef"),
	Receiver = task.ReceiverTask("Shoot_Back"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

name = "Ref_MiddleKick_test1"
}