#include "utils\maths.h"

//�û�ע�⣻�ӿ���Ҫ��������
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();      //�������
	const point2f& runner = model->get_our_player_pos(robot_id); //��Ա������
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	const float& dir = model->get_our_player_dir(robot_id);

	int  convert = ball.y > 0 ? 1 : -1;


	/****************************/
	//�������������ɲ��Եó�
	float circleR = 160;
	float DetAngle = 0.3;
	float dir_angle = 2.1;
	/****************************/

	task.target_pos = opp_goal + Maths::vector2polar(circleR, (runner - opp_goal).angle() + convert*DetAngle);

	task.orientate = dir + convert*dir_angle;

	task.flag = 1;
	return task;
}