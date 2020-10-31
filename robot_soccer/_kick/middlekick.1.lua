--desc: 
--转圈
kickerdir = function()
	return CRole2BallDir("Kicker")
end

ballX = function ()
	return (CGetBallX()+40)
end

gPlayTable.CreatePlay{
firstState = "judge",

["judge"] = {
	switch = function()
		if CGetBallY()<0  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "GetBall_1"
		else
			return "GetBall_2"
		end
	end,
	Receiver   = task.Stop("Receiver",3),
	Kicker = task.Stop("Kicker",1),--拿球加上传球
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},

["GetBall_1"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Receiver"),300)  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "delay_1"
		end
	end,
	Receiver   = task.ReceiverTask("get_mid1"),
	Kicker   = task.GotoPos("Kicker",ballX,100,kickerdir),
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},
	
["delay_1"] = {
	switch = function()
		if CIsBallKick("Receiver") then -- 延迟时间由测试得出
			return "finish"
		end
	end,
	Receiver   = task.ReceiverTask("cpmid_1"),
	Kicker   = task.GotoPos("Kicker",ballX,100,kickerdir),
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},

["GetBall_2"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Receiver"),300)  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "delay_2"
		end
	end,
	Receiver   = task.ReceiverTask("get_mid1"),
	Kicker   = task.GotoPos("Kicker",ballX,-100,kickerdir),
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},
	
["delay_2"] = {
	switch = function()
		if CIsBallKick("Receiver") then -- 延迟时间由测试得出
			return "finish"
		end
	end,
	Receiver   = task.ReceiverTask("cpmid_1"),
	Kicker   = task.GotoPos("Kicker",ballX,-100,kickerdir),
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},
name = "middlekick_1"
}


