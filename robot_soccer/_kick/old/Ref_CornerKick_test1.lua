--desc: 
gPlayTable.CreatePlay{
firstState = "GetBall",

["GetBall"] = {
	switch = function()
		if CIsBallKick("Receiver")  then -- 可根据情况加上 and Cbuf_cnt(true,120)
			return "PassBall"
		end
	end,
	Kicker   = task.KickerTask("GoRecPos_Front"),
	Receiver = task.ReceiverTask("GetBall_Front"),--拿球加上传球
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},
	
["PassBall"] = {
	switch = function()
		if  CIsGetBall("Kicker")  then --CIsGetBall("Kicker")
			return "Shoot"
		end
	end,
	Kicker   = task.KickerTask("ReceiveBall_Front"),--接球的朝向为射门点
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["Shoot"] = {
	switch = function()
		if CIsBallKick("Kicker") then
			return "finish"
		end
	end,
	Kicker = task.KickerTask("Shoot_front"),--该射门为拿球转向
	Receiver = task.ReceiverTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},
name = "Ref_CornerKick_test1"
}