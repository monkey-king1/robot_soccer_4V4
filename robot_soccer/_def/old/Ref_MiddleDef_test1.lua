gPlayTable.CreatePlay{
firstState = "doMiddleDef",

["doMiddleDef"] = {
	switch = function()
		if Cbuf_cnt(true,900) then
			return "kickloop"
		end
	end,
	Kicker  = task.RefDef("Kicker"),
	Receiver = task.RefDef("Receiver"),
	Tier = task.TierTask("tierdef"),
	Goalie  = task.Goalie()
},

["kickloop"] = {
	switch = function()
		return "kickloop"
	end,
	Kicker  = task.KickerTask("Shoot_usr"),
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie  = task.Goalie()
},

name = "Ref_MiddleDef_test1"
}