#include "utils\maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const float& dir = model->get_our_player_dir(robot_id);

	task.target_pos = ball + Maths::vector2polar(60, (goal - ball).angle());
	task.orientate = (ball - goal).angle();
	task.flag = 1;
	return task;
}