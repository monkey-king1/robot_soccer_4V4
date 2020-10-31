#include "utils\maths.h"

extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{//实地测试得出
	float head_len = 7;
	float miss = 12;//越大接球条件越宽松
	float getangle = PI / 20;
#define fast_pass  9
}

bool is_getball(const point2f& ball, const point2f& runner, const float& dir){
	//miss参数需要在比赛时实地调试
	bool get_ball = (ball - runner).length() < miss && (fabs(anglemod(dir - (ball - runner).angle())) < getangle);
	cout << "前场的拿球距离参数是  " << (ball - runner).length() << endl;
	cout << "前场拿球的角度参数是  " << fabs(anglemod(dir - (ball - runner).angle())) << endl;
	
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

	cout << "拿到球之后的角度大小是  " << (ball - receiver).angle() << "  和 " << (passer - ball).angle() << endl;

	pass = fabs(receiver_to_ball - ball_to_passer) < 0.3;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	point2f opp_goal = -FieldPoint::Goal_Center_Point;
	ball.y>0 ? (opp_goal.y += 20) : (opp_goal.y -= 20);
	const int get_buff = 30;
	const float dir = model->get_our_player_dir(robot_id);
	int convert = ball.y>0 ? 1:-1 ;
	point2f pos(300, 20);
	pos.y *= convert;
	//以下参数由测试得出
	float rece_to_ball = (ball - pos).angle();

	//判断小车已经可以传球了
	if (is_ready_pass(ball, runner, pos)){
		task.target_pos = ball + Maths::vector2polar(fast_pass, rece_to_ball);
		task.orientate = (pos - ball).angle();
		task.needCb = true;
	}
	else{
		if (ball.y>0){//球在y轴正向
			if (runner.y < ball.y){
				task.target_pos.x = runner.x;
				task.target_pos.y = runner.y + get_buff;
			}
			else{
				task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 10, rece_to_ball);
			}
		}
		else{//球在y轴负向
			if (runner.y>ball.y){
				task.target_pos.x = runner.x;
				task.target_pos.y = runner.y - get_buff;
			}
			else{
				task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 10, rece_to_ball);
			}
		}
		task.orientate = (pos - ball).angle();
	}

	return task;
}

