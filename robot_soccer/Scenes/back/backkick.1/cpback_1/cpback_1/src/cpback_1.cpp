#include "utils\maths.h"

//�û�ע�⣻�ӿ���Ҫ��������
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const float& dir = model->get_our_player_dir(robot_id);

	task.isChipKick = true;
	task.needKick = true;
	//�������ȸ��ݲ����޸�
	task.kickPower = 70;

	task.orientate = dir;
	task.flag = 1;
	return task;
}