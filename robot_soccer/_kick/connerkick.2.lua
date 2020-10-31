--desc: 
--转圈甩人
gPlayTable.CreatePlay{
firstState = "GetBall",
--该场景已转换为比赛脚本
["GetBall"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Kicker"),100)  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "delay"
		end
	end,
	Receiver   = task.ReceiverTask("archimedes"),
	Kicker = task.KickerTask("GetBall_Front"),--拿球加上传球
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},
	
["delay"] = {
	switch = function()
		if CIsBallKick("Kicker") then -- 延迟时间由测试得出
			return "PassBall"
		end
	end,
	Receiver   = task.ReceiverTask("archimedes"),
	Kicker = task.KickerTask("cpkick_c1"),--拿球加上传球
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},

["PassBall"] = {
	switch = function()
		if  CIsGetBall("Receiver")  then --CIsGetBall("Kicker")
			return "Shoot"
		end
	end,
	Receiver  = task.ReceiverTask("GoRecPos_Front"),--接球的朝向为射门点
	Kicker = task.GotoPos("Kicker",150,0,0),
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},

["Shoot"] = {
	switch = function()
		if CIsBallKick("Receiver") then
			return "finish"
		end
	end,
	Receiver = task.Shoot("Receiver"),--该射门为拿球转向
	Kicker = task.GotoPos("Kicker",150,0,0),
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},
name = "connerkick_2"
}