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
firstState = "GotoPos",

["GotoPos"] = {
	switch = function ()
		if CNormalStart() then
			return "PenaltyKick"
		elseif CGameOn() then
			return "finish"
		end
	end,
	Kicker = task.GotoPos("Kicker",200,0,kickerdir),
	Receiver = task.NormalDef("Receiver"),
	Tier = task.Stop("Tier",5),
	Goalie = task.GoalieTask("goalie")
},



["PenaltyKick"] = {
	switch = function()
		if CIsBallKick("Kicker") or CGameOn() then
			return "finish"
		end
	end,
	Kicker   = task.PenaltyKick("Kicker"),
	Receiver = task.Stop("Receiver",3),
	Tier = task.Stop("Tier",5),
	Goalie = task.GoalieTask("goalie")
},

name = "Ref_PenaltyKick_test1"
}