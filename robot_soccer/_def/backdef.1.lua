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
firstState = "start",
--已转换为比赛脚本
["start"] = {
	switch = function()
		if getOppNum() then
			return "start2"
		end
	end,
	Kicker  = task.KickerTask("kickerdef"),
	Receiver = task.ReceiverTask("connerdefreceiver"),--防人
	Tier = task.TierTask("connerdeftier"),--防人
	Goalie   = task.GoalieTask("goalie")
},

["start2"] = {
	switch = function()
		if Cbuf_cnt(getOppNumKick(),400) then
			return "finish"
		end
	end,
	Kicker  = task.KickerTask("kickerdef"),
	Receiver = task.ReceiverTask("connerdefreceiver"),--防人
	Tier = task.TierTask("connerdeftier"),--防人
	Goalie   = task.GoalieTask("goalie")
},

name = "backdef_1"
}

