--desc: 
--背防脚本
local x=200
local y=105
local dir=1.7
gPlayTable.CreatePlay{
firstState = "judge",
--该场景已转换为比赛脚本
["judge"] = {
	switch = function()
		if CGetBallY()>0  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "GetBall_1"
		else
			return "GetBall_2"
		end
	end,
	Receiver   = task.Stop("Receiver",3),
	Kicker = task.Stop("Kicker",1),--拿球加上传球
	Tier = task.Stop("Tier",5),
	Goalie   = task.GoalieTask("goalie")
},

["GetBall_1"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Kicker"),300)  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "delay"
		end
	end,
	Receiver   = task.GotoPos("Receiver",x,-y,-dir),
	Kicker = task.KickerTask("GetBall_Front"),--拿球加上传球
	Tier = task.TierTask("def"),
	Goalie   = task.GoalieTask("goalie")
},

["GetBall_2"] = {
	switch = function()
		if Cbuf_cnt(CIsGetBall("Kicker"),300)  then -- 改变这个时间可以控制跑位的时间，一定要在8s之内
			return "delay"
		end
	end,
	Receiver   = task.GotoPos("Receiver",x,y,dir),
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
	Receiver   = task.GotoPos("Receiver",x,y,dir),
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
name = "connerkick_1"
}