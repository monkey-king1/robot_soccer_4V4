#include "utils\maths.h"
#include "superforward.h"
//用户注意；接口需要如下声明
int flag = 0;
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

bool is_getball(const point2f& ball, const point2f& runner, const float& dir){
	int miss = 20;
	float getangle = PI / 4;
	bool get_ball = (ball - runner).length() < miss && (fabs(anglemod(dir - (ball - runner).angle())) < getangle);
	if (get_ball){
		return true;
	}
	else{
		return false;
	}
}

bool get_id_dir(const WorldModel* model, int& getid, float& getdir, const point2f& runner, const point2f& ball){
	bool isgetball = false;
	const bool* exist_id = model->get_opp_exist_id();
	bool ready = false;
	for (int i = 0; i < MAX_ROBOTS; i++){
		//判断敌方车号
		if (exist_id[i]){
			const point2f& pos = model->get_opp_player_pos(i);
			const float& dir = model->get_opp_player_dir(i);
			if (ball.y >0){
				if (dir<-PI/4 && dir>-(PI*3)/4){
					ready = true;
				}
			}
			else{
				if (dir>PI / 4 && dir<(PI * 3) / 4){
					ready = true;
				}
			}
			if (is_getball(ball, pos, dir) || ready){
				cout << "有车持球" << endl;
				getid = i;
				getdir = dir;
				isgetball = true;
				break;
			}
		}
	}
	return isgetball;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const point2f& runner = model->get_our_player_pos(robot_id);
	const point2f& ball = model->get_ball_pos();
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& last_ball = model->get_ball_pos(1);
	bool is_move = (ball - last_ball).length() > 1.5 ? true:false ;
	int guarddist (0) ;
	point2f opp_getpos(0,0);
	int getid(-7);
	float getdir(0);

	if (ball.x > 180){
		guarddist = 250;
	}
	else if (ball.x > -180){
		guarddist = 200;
	}
	else if (ball.x > -250){
		guarddist = 60;
	}
	else{
		guarddist = 60;
	}

	if (get_id_dir(model, getid, getdir, runner, ball)){//有车持球
		cout << "有车持球" << endl;
		opp_getpos = model->get_opp_player_pos(getid);
		task.target_pos = opp_getpos + Maths::vector2polar(guarddist, getdir);
		task.orientate = (ball - runner).angle();
	}
	else{//没车持球
		task.target_pos = ball + Maths::vector2polar(60, (goal - ball).angle());
		task.orientate = (ball - goal).angle();
	}
	
	return task;
}