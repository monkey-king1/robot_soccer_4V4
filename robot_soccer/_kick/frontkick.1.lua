--desc: 
gPlayTable.CreatePlay{
firstState = "GetBall",

--该场景已转换为比赛脚本
["GetBall"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Kicker"),300)  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "delay"
		end
	end,
	Kicker   = task.KickerTask("frontget_2"),
	Receiver = task.ReceiverTask("receiverdef"),--拿球加上传球
	Tier = task.TierTask("tierdef"),
	Goalie   = task.GoalieTask("goalie")
},
	
["delay"] = {
	switch = function()
		if CIsBallKick("Kicker") then -- 延迟时间由测试得出
			return "finish"
		end
	end,
	Kicker   = task.KickerTask("cpfront_2"),
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.GoalieTask("goalie")
},

name = "frontkick_2"
}