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
	float miss = 14;//越大接球条件越宽松
	float getangle = PI / 4;
	int outpently = 96;
	float outangle = 0.1;//测试
	int offset = 9;
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

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float& dir = model->get_our_player_dir(robot_id);
	const point2f& ball = model->get_ball_pos();
	const point2f& last_ball = model->get_ball_pos(1);
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& arc_center_right = FieldPoint::Penalty_Arc_Center_Right;
	const point2f& arc_center_left = FieldPoint::Penalty_Arc_Center_Left;
	const point2f& rectangle_left = FieldPoint::Penalty_Rectangle_Left;
	const point2f& rectangle_right = FieldPoint::Penalty_Rectangle_Right;
	const point2f& goal_right = FieldPoint::Goal_Center_Right_Two_Point;
	float predict_dist(0);
	point2f predict_pos(0, 0);
	float movedist = (ball - last_ball).length();
	bool ismoving = movedist > 0.8 ? true : false;
	cout << "last_ball的大小是" << last_ball << endl;
	cout << "(ball - last_ball).length()的大小是" << (ball - last_ball).length() << endl;
	//predict_dist = movedist * 50;
	if (ismoving){
		//预判球的位置
		predict_pos = ball + Maths::vector2polar(predict_dist, (ball - last_ball).angle());
	}
	else{
		//不预判，正常防御
		predict_pos = ball;
	}
	cout << "receiverdef里的predict_pos" << predict_pos << endl;
	PenaltyArea area;
	if (predict_pos.y > arc_center_right.y)
		area = RightArc;
	else if (predict_pos.y < arc_center_left.y)
		area = LeftArc;
	else
		area = MiddleRectangle;

	point2f term(0, 0);
	switch (area)
	{
	case RightArc:
		task.orientate = (predict_pos - goal).angle();
		task.target_pos = goal + Maths::vector2polar(outpently, task.orientate + outangle);
		if ((runner - ball).length()<20){
			task.target_pos = goal + Maths::vector2polar(outpently, task.orientate + outangle / 2);
		}
		break;
	case MiddleRectangle:
		task.orientate = (predict_pos - goal).angle();
		term = Maths::across_point(rectangle_left, rectangle_right, ball, goal);
		task.target_pos.x = term.x ;
		task.target_pos.y = term.y + offset;
		if ((runner - ball).length()<20){
			task.target_pos.y = term.y + offset/2;
		}
		break;
	case LeftArc:
		task.orientate = (predict_pos - goal).angle();
		task.target_pos = goal + Maths::vector2polar(outpently, task.orientate + outangle);
		if ((runner - ball).length()<20){
			task.target_pos = goal + Maths::vector2polar(outpently, task.orientate + outangle / 2);
		}
		break;
	default:
		break;
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