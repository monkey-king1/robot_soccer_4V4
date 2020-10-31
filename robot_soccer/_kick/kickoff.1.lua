kickerdir = function()
	return CRole2BallDir("Kicker")
end

gPlayTable.CreatePlay{

--该场景已转换为比赛脚本
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
	Receiver = task.ReceiverTask("receiverdef"),--拿球加上传球
	Tier = task.TierTask("tierdef"),
	Goalie = task.GoalieTask("goalie")
},

["getball"] = {
	switch = function()
		if CGameOn() then
			return "finish"
		elseif CIsBallKick("Kicker") then
			return "finish"
		end
	end,
	Kicker = task.KickerTask("getball_kickoff"),
	Receiver = task.ReceiverTask("receiverdef"),--拿球加上传球
	Tier = task.TierTask("tierdef"),
	Goalie = task.GoalieTask("goalie")
},

name = "kickoff_1"
}