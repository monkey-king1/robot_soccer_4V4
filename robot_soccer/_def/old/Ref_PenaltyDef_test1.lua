kickerdir = function()
	return CRole2BallDir("Kicker")
end

ReceiverDir = function()
	return CRole2BallDir("Receiver")
end

TierDir = function()
	return CRole2BallDir("TierDir")
end

gPlayTable.CreatePlay{
firstState = "PenaltyDef",

["PenaltyDef"] = {
	switch = function()
		return "PenaltyDef"
	end,
	Kicker   = task.GotoPos("Kicker",-230,80,kickerdir),
	Receiver = task.GotoPos("Receiver",-230,-80,ReceiverDir),
	Tier = task.GotoPos("Tier",-150,0,TierDir),
	Goalie   = task.PenaltyDef()
},

name = "Ref_PenaltyDef_test1"
}