kickerdir = function()
	return CRole2BallDir("Kicker")
end

ReceiverDir = function()
	return CRole2BallDir("Receiver")
end

Tierdir = function()
	return CRole2BallDir("Tier")
end

gPlayTable.CreatePlay{
firstState = "PenaltyDef",

["PenaltyDef"] = {
	switch = function()
		return "PenaltyDef"
	end,
	Kicker   = task.GotoPos("Kicker",-130,80,kickerdir),
	Receiver = task.GotoPos("Receiver",-130,-80,ReceiverDir),
	Tier = task.GotoPos("Tier",-130,0,Tierdir),
	Goalie   = task.PenaltyDef()
},

name = "penaltydef_1"
}