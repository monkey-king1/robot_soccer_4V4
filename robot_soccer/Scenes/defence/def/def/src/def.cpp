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

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;

	const point2f& ball = model->get_ball_pos(robot_id);
	point2f goal = FieldPoint::Goal_Center_Point;
	if (ball.y>0){
		goal.y += 20;
	}
	else{
		goal.y -= 20;
	}

	const point2f& arc_center_right = FieldPoint::Penalty_Arc_Center_Right;
	const point2f& arc_center_left = FieldPoint::Penalty_Arc_Center_Left;
	const point2f& rectangle_left = FieldPoint::Penalty_Rectangle_Left;
	const point2f& rectangle_right = FieldPoint::Penalty_Rectangle_Right;

	//PenaltyArea area;
	//if (theat_pos.y > arc_center_right.y)
	//	area = RightArc;
	//else if (theat_pos.y < arc_center_left.y)
	//	area = LeftArc;
	//else
	//	area = MiddleRectangle;
	PenaltyArea area;
	if (ball.y > arc_center_right.y)
		area = RightArc;
	else if (ball.y < arc_center_left.y)
		area = LeftArc;
	else
		area = MiddleRectangle;

	switch (area)
	{
	case RightArc:
		task.orientate = (ball - goal).angle();
		task.target_pos = goal + Maths::vector2polar(PENALTY_AREA_R + MAX_ROBOT_SIZE , task.orientate);
		break;
	case MiddleRectangle:
		task.orientate = (ball - goal).angle();
		task.target_pos = Maths::across_point(rectangle_left, rectangle_right, ball, goal);
		break;
	case LeftArc:
		task.orientate = (ball - goal).angle();
		task.target_pos = goal + Maths::vector2polar(PENALTY_AREA_R + MAX_ROBOT_SIZE  , task.orientate);
		break;
	default:
		break;
	}

	task.flag = 1;

	return task;
}