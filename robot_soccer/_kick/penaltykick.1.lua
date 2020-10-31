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
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie = task.GoalieTask("goalie")
},



["PenaltyKick"] = {
	switch = function()
		if CIsBallKick("Kicker") or CGameOn() then
			return "finish"
		end
	end,
	Kicker   = task.PenaltyKick("Kicker"),
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie = task.GoalieTask("goalie")
},

name = "penaltykick_1"
}