#include "utils\maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float& dir = model->get_our_player_dir(robot_id);

	task.isChipKick = true;
	task.needKick = true;
	//射球力度根据测试修改
	task.kickPower = 70;

	task.orientate = dir;
	task.flag = 1;
	return task;
}