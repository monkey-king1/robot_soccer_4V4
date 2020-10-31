#include "utils\maths.h"
#include "utils\constants.h"
#include "utils\worldmodel.h"
#include "utils\PlayerTask.h"


extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{
	float headlength = 7;
	float miss = 12;//越大接球条件越宽松
	float getangle = PI / 17;
	int magnification = 50;
	float ball_movingdist = 1.2;
#define fast_pass 8
}

bool is_getball(const point2f& ball, const point2f& runner, const float& dir){
	bool get_ball = (ball - runner).length() < miss && (fabs(anglemod(dir - (ball - runner).angle())) < getangle);
	if (get_ball){
		return true;
	}
	else{
		return false;
	}
}

bool is_ready_pass(const point2f& ball, const point2f& passer, const point2f& receiver){
	float receiver_to_ball = (ball - receiver).angle();
	float ball_to_passer = (passer - ball).angle();
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass;
	pass = fabs(receiver_to_ball - ball_to_passer) < 0.4;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& executer = model->get_our_player_pos(robot_id);
	const float& dir = model->get_our_player_dir(robot_id);

	point2f opp_goal(300, 200);
	const float excute_dir = model->get_our_player_dir(robot_id);

	float rece_to_ball = (ball - opp_goal).angle();    //接球者朝向球的夹角

	if (is_ready_pass(ball, executer, opp_goal)){
		if (is_getball( ball, executer, dir)){
			task.isChipKick = true;
			task.needKick = true;
			task.kickPower = 127;
		}
		task.target_pos = ball + Maths::vector2polar(fast_pass, rece_to_ball);
		task.orientate = (opp_goal - ball).angle();
	}
	else
	{
		task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 15, rece_to_ball);
		task.orientate = (opp_goal - ball).angle();
		//Maths::circle_move(model, robot_id, task, executer, ball, 15, (opp_goal - ball).angle());
	}

	task.needCb = true;
	return task;
}

