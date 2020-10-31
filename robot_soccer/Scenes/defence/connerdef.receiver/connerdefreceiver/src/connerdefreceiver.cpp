#include "utils/maths.h"
#include <iostream>
using namespace std;
//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

enum PenaltyArea
{
	RightArc,
	MiddleRectangle,
	LeftArc
};

namespace{//实地测试得出
	float headlength = 7;
	float miss = 12;//越大接球条件越宽松
	float getangle = PI / 20;
	int out_area = 0;
	int offset = 5;
	int block_dist = 130;
	int outdist = 96;
	float deltangle = 0.1;
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

void judgearea(const point2f& opp_pos, const int& convert, int& area){
	if (convert == -1){//球在y轴正方向
		if (opp_pos.y<0){
			area = 1;
		}
		else{
			area = 2;
		}
	}
	else{//球在y轴负方向
		if (opp_pos.y>0){
			area = 1;
		}
		else{
			area = 2;
		}
	}
}

bool opp_behideus(const WorldModel* model, int& block_id, const point2f& ball){
	//初始化判断变量is_block
	bool is_block = false;
	bool exit = false;
	int area = -7;//1表示离球最远
	point2f opp_pos(0, 0);
	const point2f& goal = FieldPoint::Goal_Center_Point;
	int convert = ball.y > 0 ? -1 : 1;
	const bool* exist_id = model->get_opp_exist_id();
	for (int i = 0; i < MAX_ROBOTS; i++){
		area = -7;
		if (exist_id[i]){
			opp_pos = model->get_opp_player_pos(i);
			judgearea(opp_pos, convert, area);
			if (area == 1 && opp_pos.x<0){
				exit = true;
				block_id = i;
				is_block = true;
				goto idx;
			}
		}
	}

idx:
	if (exit){
		return is_block;
	}
	else{
		for (int i = 0; i < MAX_ROBOTS; i++){
			area = -7;
			if (exist_id[i]){
				opp_pos = model->get_opp_player_pos(i);
				if (opp_pos.x<0 && (opp_pos-goal).length()< 160){
					block_id = i;
					is_block = true;
					return is_block;
				}
			}
		}
	}

}


PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float& dir = model->get_our_player_dir(robot_id);
	const point2f& ball = model->get_ball_pos();
	const point2f& last_ball = model->get_ball_pos(1);
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	float predict_dist(0);
	point2f predict_pos(0, 0);
	point2f guardpos(0, 0);
	int block_id(-7);
	float movedist = (ball - last_ball).length();
	bool ismoving = movedist > 0.8 ? true : false;
	cout << "last_ball的大小是" << last_ball << endl;
	cout << "(ball - last_ball).length()的大小是" << (ball - last_ball).length() << endl;
//	predict_dist = movedist * 50;
	if (ismoving){
		//预判球的位置
		predict_pos = ball + Maths::vector2polar(predict_dist, (ball - last_ball).angle());
	}
	else{
		//不预判，正常防御
		predict_pos = ball;
	}

	if (opp_behideus(model, block_id, ball)){
		guardpos = model->get_opp_player_pos(block_id);
		task.orientate = (guardpos - goal).angle();
		task.target_pos = goal + Maths::vector2polar(outdist, task.orientate - deltangle);
	}
	else{
		task.target_pos.x = -210;
		task.target_pos.y = -10;
		task.orientate = (opp_goal - goal).angle();
	}

	if (is_getball(ball, runner, dir)){
		task.needKick = true;
		task.kickPower = 127;
	}

	if ((runner - ball).length()<30){
		task.needCb = true;
	}

	task.flag = 1;
	return task;
}