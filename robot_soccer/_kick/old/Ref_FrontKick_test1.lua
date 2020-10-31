--desc: 
gPlayTable.CreatePlay{
firstState = "GetBall",

["GetBall"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Kicker"),300)  then -- 延迟时间由测试得出
			return "PassBall"
		end
	end,
	Receiver   = task.ReceiverTask("ReceiveBall_Front"),
	Kicker = task.KickerTask("GetBall_Front"),--拿球加上传球
	Tier = task.TierTask("def"),
	Goalie   = task.Goalie()
},
	
["PassBall"] = {
	switch = function()
		if  CIsGetBall("Kicker")  then 
			return "Shoot"
		end
	end,
	Receiver  = task.ReceiverTask("GoRecPos_Front"),--接球的朝向为射门点
	Kicker = task.KickerTask("receiverdef"),
	Tier = task.TierTask("def"),
	Goalie   = task.Goalie()
},

["Shoot"] = {
	switch = function()
		if CIsBallKick("Kicker") then
			return "finish"
		end
	end,
	Receiver = task.ReceiverTask("Shoot_front"),--该射门为拿球转向
	Kicker = task.KickerTask("receiverdef"),
	Tier = task.TierTask("def"),
	Goalie   = task.Goalie()
},
name = "Ref_FrontKick_test1"
}