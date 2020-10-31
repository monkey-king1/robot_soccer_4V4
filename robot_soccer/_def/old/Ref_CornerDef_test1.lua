gPlayTable.CreatePlay{
firstState = "doCornerDef",


["doCornerDef"] = {
	switch = function()
		if  Cbuf_cnt(true,600)  then --CIsGetBall("Kicker")
			return "kickloop"
		end
	end,
	Kicker  = task.KickerTask("def_first"),
	Receive = task.ReceiverTask("def_second"),
	Tier = task.TierTask("def"),
	Goalie   = task.Goalie()
},

["doCornerDef"] = {
	switch = function()
		return "doCornerDef"
	end,
	Kicker  = task.KickerTask("Shoot_usr"),
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("def"),
	Goalie  = task.Goalie()
},

name = "Ref_CornerDef_test1"
}