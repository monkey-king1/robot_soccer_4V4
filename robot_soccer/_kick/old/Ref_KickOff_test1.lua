kickerdir = function()
	return CRole2BallDir("Kicker")
end

gPlayTable.CreatePlay{

firstState = "start",
	
["start"] = {
	switch = function ()
		if CNormalStart() then
			return "getball"
		elseif CGameOn() then
			return "finish"
		end
	end,
	Kicker = task.GotoPos("Kicker",-50,0,kickerdir),
	Receiver = task.Stop("Receiver",3),
	Tier = task.Stop("Tier",5),
	Goalie = task.GoalieTask("goalie")
},

["getball"] = {
	switch = function()
		if CGameOn() then
			return "finish"
		elseif CIsBallKick("Kicker") then
			return "Shoot"
		end
	end,
	Kicker = task.KickerTask("getball_kickoff"),
	Receiver = task.Stop("Receiver",3),
	Tier = task.Stop("Tier",5),
	Goalie = task.GoalieTask("goalie")
},

["Shoot"] = {
	switch = function()
		if CIsBallKick("Kicker")  then
			return "finish"
		end
	end,
	Kicker = task.Stop("Kicker",1),
	Receiver = task.Stop("Receiver",3),
	Tier = task.Stop("Tier",5),
	Goalie = task.GoalieTask("goalie")
},

name = "kickoff_1"
}