#include "utils\maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();      //球的坐标
	const point2f& runner = model->get_our_player_pos(robot_id); //球员的坐标
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	const float& dir = model->get_our_player_dir(robot_id);

	int  convert = ball.y > 0 ? -1 : 1;


	/****************************/
	//下面三个参数由测试得出
	point2f pos(210, 105);
	pos.y *= convert;
	float circleR = 18;
	float DetAngle = 0.9;
	float dir_angle = 0.7;
	/****************************/

	task.target_pos = pos + Maths::vector2polar(circleR, (runner - pos).angle() + convert*DetAngle);

	task.orientate = dir + convert*dir_angle;

	task.flag = 1;
	return task;
}