gPlayTable.CreatePlay{
--该脚本已经转换为比赛脚本
firstState = "initState",

["initState"] = {
	switch = function()
		if CIsGetBall("Kicker") then
			return "initState"
		end
	end,
	Kicker  = task.KickerTask("superforward"),
	Receiver = task.ReceiverTask("receiverdef"),--预判球运动的位置
	Tier = task.TierTask("tierdef"),--预判球运动的位置
	Goalie  = task.GoalieTask("goalie")
},

name = "normalplay_1"
}