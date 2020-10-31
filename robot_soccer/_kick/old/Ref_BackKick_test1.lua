--desc: 
Receiver2Ball = function ()
	return CRole2BallDir("Receiver")
end

gPlayTable.CreatePlay{
firstState = "GetBall",

["GetBall"] = {
	switch = function()
		if CIsBallKick("Kicker")  then 
			return "PassBall_1"
		end
	end,
	Receiver   = task.ReceiverTask("GoRecPos_Back_0"),
	Kicker = task.KickerTask("GetBall_Back"),--设置的踢球挑射力度是25，方向为为离球门最近的机器人
	Tier = task.TierTask("def"),
	Goalie   = task.Goalie()
},

["PassBall_1"] = {
	switch = function()
		if  CIsGetBall("Receiver") then 
			return "delay1"
		end
	end,
	Receiver   = task.ReceiverTask("ReceiveBall_usr"),--正常接球
	Kicker = task.KickerTask("GoRecPos_Back_1"),--位置是(100，convert*110)
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["delay1"] = {
	switch = function()
		if  Cbuf_cnt(true, 120) then 
			return "PassBall_2"
		end
	end,
	Receiver   = task.ReceiverTask("ChipBall_Back_2"),--正常接球
	Kicker = task.KickerTask("GoRecPos_Back_1"),--位置是(100，convert*110)
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["PassBall_2"] = {
	switch = function()
		if  CIsBallKick("Receiver")  then 
			return "delay2"
		end
	end,
	Receiver   = task.ReceiverTask("ChipBall_Back_2"),--第一次挑球的位置是离球门最近的机器人
	Kicker = task.KickerTask("ReceiveBall_usr"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["delay2"] = {
	switch = function()
		if  Cbuf_cnt(CIsGetBall("Kicker"), 60) then 
			return "delay4"
		end
	end,
	Receiver   = task.ReceiverTask("GoRecPos_Back_2"),--位置在一个区域范围之内,中点坐标是(-30,convert*110)
	Kicker = task.KickerTask("ReceiveBall_usr"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["delay4"] = {
	switch = function()
		if  Cbuf_cnt(true, 80)  then 
			return "delay3"
		end
	end,
	Receiver   = task.ReceiverTask("GoRecPos_Back_2"),--嘴巴朝向对面球门
	Kicker = task.KickerTask("Kp_Back_3"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["delay3"] = {
	switch = function()
		if  CIsGetBall("Receiver")  then --CIsGetBall("Kicker")
			return "Shoot"
		end
	end,
	Receiver   = task.ReceiverTask("ReceiveBall_usr"),--跑到球门旁边
	Kicker = task.KickerTask("receiverdef"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

["Shoot"] = {
	switch = function()
		if CIsBallKick("Receiver") then
			return "finish"
		end
	end,
	Kicker = task.KickerTask("receiverdef"),
	Receiver = task.ReceiverTask("Shoot_Back"),
	Tier = task.TierTask("tierdef"),
	Goalie   = task.Goalie()
},

name = "Ref_BackKick_test1"
}