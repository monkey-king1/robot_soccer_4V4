function getOppNum()
	local oppTable = CGetOppNums()
	for i,val in pairs(oppTable) do 
		num = tonumber(val)
		if COppIsGetBall(num-1) then
			return true
		end
	end
end

function getOppNumKick()
	local oppTable = CGetOppNums()
	for i,val in pairs(oppTable) do 
		num = tonumber(val)
		if COppIsBallKick(num-1) then
			return true
		end
	end
end

gPlayTable.CreatePlay{
firstState = "kickdef",
--已转换为比赛脚本
["kickdef"] = {
	switch = function()
		if getOppNum() then
			return "kickdef2"
		end
	end,
	Kicker  = task.KickerTask("kickerdef"),
	Receiver = task.ReceiverTask("receiverdef"),--拿球加上传球
	Tier = task.TierTask("tierdef"),
	Goalie   = task.GoalieTask("goalie")
},

["kickdef2"] = {
	switch = function()
		if getOppNumKick() then
			return "finish"
		end
	end,
	Kicker  = task.KickerTask("kickerdef"),
	Receiver = task.ReceiverTask("receiverdef"),--拿球加上传球
	Tier = task.TierTask("tierdef"),
	Goalie   = task.GoalieTask("goalie")
},

name = "kickoffdef_1"
}