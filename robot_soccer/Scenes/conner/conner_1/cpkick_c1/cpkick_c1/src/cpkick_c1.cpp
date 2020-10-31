#include "utils\maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{
	float head_len = 7.0;
#define fast_pass 9
}

//判断是否可以射门
bool is_ready_kick(const point2f& ball, const point2f& passer, const point2f& receiver){
	float receiver_to_ball = (ball - receiver).angle();
	float ball_to_passer = (passer - ball).angle();
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass;
	if ((passer - receiver).length()>80)
		pass = fabs(receiver_to_ball - ball_to_passer) < 0.15;
	else
		pass = fabs(receiver_to_ball - ball_to_passer) < 0.18;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float& dir = model->get_our_player_dir(robot_id);

	task.isChipKick = true;
	task.needKick = true;
	//射球力度根据测试修改
	task.kickPower = 30;

	task.orientate = dir;
	task.flag = 1;
	return task;
}