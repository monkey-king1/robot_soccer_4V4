#include "utils\maths.h"

extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{
	float head_len = 7.0;
	//场地实调
#define fast_pass 9
}

bool is_ready_pass(const point2f& ball, const point2f& passer, const point2f& receiver){
	float receiver_to_ball = (ball - receiver).angle();
	float ball_to_passer = (passer - ball).angle();
	cout << "拿到球之后的角度大小是  " << (ball - receiver).angle() << "  和 " << (passer - ball).angle() << endl;
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass;
	pass = fabs(receiver_to_ball - ball_to_passer) < 0.3;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float dir = model->get_our_player_dir(robot_id);

	//该参数在比赛时需要调试
	int get_buff = 14;

	//miss参数需要在比赛时实地调试
	float miss = 1.6;
	bool get_ball = (ball - runner).length() < get_ball_threshold - miss && (fabs(anglemod(dir - (ball - runner).angle())) < PI / 4);

	cout << "角球里的距离是  " << (ball - runner).length() << "  和角度是 " << fabs(anglemod(dir - (ball - runner).angle())) << endl;
	point2f pos;
	//以下参数由测试得出
	pos.x = 230;
	pos.y = 0;
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
				task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 15, rece_to_ball);
			}
		}
		else{//球在y轴负向
			if (runner.y>ball.y){
				task.target_pos.x = runner.x;
				task.target_pos.y = runner.y - get_buff;
			}
			else{
				task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 15, rece_to_ball);
			}
		}
		task.orientate = (pos - ball).angle();
	}

	//task.needCb = true;
	return task;
}

