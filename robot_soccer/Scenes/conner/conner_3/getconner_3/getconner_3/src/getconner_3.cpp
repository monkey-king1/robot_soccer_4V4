#include "utils\maths.h"

extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{
	float head_len = 7.0;
	//����ʵ��
#define fast_pass 9
}

bool is_ready_pass(const point2f& ball, const point2f& passer, const point2f& receiver){
	float receiver_to_ball = (ball - receiver).angle();
	float ball_to_passer = (passer - ball).angle();
	cout << "�õ���֮��ĽǶȴ�С��  " << (ball - receiver).angle() << "  �� " << (passer - ball).angle() << endl;
	//����ʸ���Ƕ�֮��С��ĳ��ֵ���ж��Ƿ���Դ���
	bool pass;
	pass = fabs(receiver_to_ball - ball_to_passer) < 0.3;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float dir = model->get_our_player_dir(robot_id);

	//�ò����ڱ���ʱ��Ҫ����
	int get_buff = 14;

	//miss������Ҫ�ڱ���ʱʵ�ص���
	float miss = 1.6;
	bool get_ball = (ball - runner).length() < get_ball_threshold - miss && (fabs(anglemod(dir - (ball - runner).angle())) < PI / 4);

	cout << "������ľ�����  " << (ball - runner).length() << "  �ͽǶ��� " << fabs(anglemod(dir - (ball - runner).angle())) << endl;
	point2f pos;
	//���²����ɲ��Եó�
	pos.x = 230;
	pos.y = 0;
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

	//task.needCb = true;
	return task;
}

