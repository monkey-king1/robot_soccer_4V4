#include "utils\maths.h"

extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{//ʵ�ز��Եó�
	float head_len = 7;
	float miss = 12;//Խ���������Խ����
	float getangle = PI / 20;
#define fast_pass  9
}

bool is_getball(const point2f& ball, const point2f& runner, const float& dir){
	//miss������Ҫ�ڱ���ʱʵ�ص���
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
	//����ʸ���Ƕ�֮��С��ĳ��ֵ���ж��Ƿ���Դ���
	bool pass;
	pass = fabs(receiver_to_ball - ball_to_passer) < 0.25;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const int get_buff = 30;
	const float dir = model->get_our_player_dir(robot_id);
	int convert = ball.y>0 ? -1 : 1;
	point2f pos(300, 200);
	pos.y *= convert;
	//���²����ɲ��Եó�
	float rece_to_ball = (ball - pos).angle();

	//�ж�С���Ѿ����Դ�����
	if (is_ready_pass(ball, runner, pos)){
		task.target_pos = ball + Maths::vector2polar(fast_pass, rece_to_ball);
		task.orientate = (pos - ball).angle();
		task.needCb = true;
	}
	else{
		if (ball.y>0){//����y������
			if (runner.y < ball.y){
				task.target_pos.x = runner.x;
				task.target_pos.y = runner.y + get_buff;
			}
			else{
				task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 15, rece_to_ball);
			}
		}
		else{//����y�Ḻ��
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

	return task;
}

