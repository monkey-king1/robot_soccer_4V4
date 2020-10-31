#include "utils\maths.h"
#include <vector>
#include <iostream>
using namespace std;

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{//实地测试得出
	float headlength = 7;
	float miss = 12;//越大接球条件越宽松
	float getangle = PI / 15;
	int magnification = 50;
	float ball_movingdist = 0.9;
	float runcircle = 70;
}

bool is_getball(const point2f& ball, const point2f& runner, const float& dir){
	//miss参数需要在比赛时实地调试
	cout << "进入is_getball的判断" << endl;
	bool get_ball = (ball - runner).length() < miss && (fabs(anglemod(dir - (ball - runner).angle())) < getangle);
	cout << "前锋里的(ball - runner).length()   " << (ball - runner).length() << endl;
	cout << "前锋里的fabs(anglemod(dir - (ball - runner).angle())   " << fabs(anglemod(dir - (ball - runner).angle())) << endl;
	if (get_ball){
		return true;
	}
	else{
		return false;
	}
}

void Min2Method(float *data_x, float *data_y, int data_n, vector<float> &vResult)
{
	float A = 0.0;
	float B = 0.0;
	float C = 0.0;
	float D = 0.0;
	float E = 0.0;
	float F = 0.0;

	for (int i = 0; i<data_n; i++)
	{
		A += data_x[i] * data_x[i];
		B += data_x[i];
		C += data_x[i] * data_y[i];
		D += data_y[i];
	}

	// 计算斜率a和截距b
	float a, b, temp = 0;
	if (temp = (data_n*A - B*B))// 判断分母不为0
	{
		a = (data_n*C - B*D) / temp;
		b = (A*D - B*C) / temp;
	}
	else
	{
		a = 1;
		b = 0;
	}

	// 计算相关系数r
	float Xmean, Ymean;
	Xmean = B / data_n;
	Ymean = D / data_n;

	float tempSumXX = 0.0, tempSumYY = 0.0;
	for (int i = 0; i<data_n; i++)
	{
		tempSumXX += (data_x[i] - Xmean) * (data_x[i] - Xmean);
		tempSumYY += (data_y[i] - Ymean) * (data_y[i] - Ymean);
		E += (data_x[i] - Xmean) * (data_y[i] - Ymean);
	}
	F = sqrt(tempSumXX) * sqrt(tempSumYY);

	float r;
	r = E / F;

	vResult.push_back(a);
	vResult.push_back(b);
	vResult.push_back(r*r);
}

void get_k_b(const WorldModel* model, float& b_interrupt, float &k_slope, vector<point2f>& ball_points){
	const int nCount = 4;//取帧数作为依据
	vector<float> vResult;
	float data_x[nCount];
	float data_y[nCount];
	for (int i = 0; i<nCount; i++){
		ball_points.push_back(model->get_ball_pos(i));
		data_x[i] = ball_points[i].x;
		data_y[i] = ball_points[i].y;
		cout << "ball_points[" << i << "]的坐标是" << ball_points[i] << endl;
	}
	Min2Method(data_x, data_y, nCount, vResult);//获得曲线的斜率和截距
	k_slope = vResult[0];
	b_interrupt = vResult[1];
	cout << "k_slope的大小是   " << k_slope << endl;
}

void get_angle(float& angle, const float& k_slope, const point2f& vel){
	if (vel.angle() <= PI / 2 && vel.angle() >= (-PI / 2)){//在1，4象限
		angle = atan(k_slope);
	}
	else if (vel.angle() >= PI / 2 && vel.angle() <= PI){//在第2象限
		angle = atan(k_slope) + PI;
	}
	else if (vel.angle() <= (-PI / 2) && vel.angle() >= -PI){//在第3象限
		angle = atan(k_slope) - PI;
	}
	cout << "vel.angle()的大小是   " << vel.angle() << endl;
	cout << "angle的大小是   " << angle << endl;
}

bool is_ready_pass(const point2f& ball, const point2f& passer, const point2f& receiver,const float& angle){
	float ball_to_passer = (passer - ball).angle();
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass;
	pass = fabs(angle - ball_to_passer) < 0.4;
	return pass;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	const float& dir = model->get_our_player_dir(robot_id);
	const point2f& vel = model->get_ball_vel();
	const point2f& ball = model->get_ball_pos();
	const point2f& last_ball = model->get_ball_pos(1);
	const point2f& runner = model->get_our_player_pos(robot_id);
	const point2f opp_right(300,25);
	const point2f opp_left(300, -25);
	point2f opp_goal = -FieldPoint::Goal_Center_Point;
	const point2f& goalie_pos = model->get_our_player_pos(model->get_our_goalie());
	point2f temp(0, 0);
	vector<point2f> ball_points;
	float b_interrupt(0);
	float k_slope(0);
	float angle (0);//小球运动的角度
	bool ismove = (ball - last_ball).length() > ball_movingdist ? true : false;
	cout << "一帧移动的距离的大小是  " << (ball - last_ball).length() << endl;
	float r2b_dist = (runner - ball).length();
	float change_track = (ball-last_ball).length();

	get_k_b(model, b_interrupt, k_slope, ball_points);
	get_angle(angle , k_slope, vel);
	//get_cross(temp, runner, b_interrupt, k_slope);

	if (is_getball(ball,runner, dir) ){
		if (dir > (opp_left-runner).angle() && dir < (opp_right-runner).angle()){
			task.needKick = true;
			task.kickPower = 127;
		}
		task.target_pos = runner + Maths::vector2polar(runcircle, dir);
		task.orientate = (opp_goal-runner).angle();
	}
	else{
		if (ismove){
			if (is_ready_pass(ball, runner, opp_goal,angle) && r2b_dist < 60 ){
				task.target_pos = runner;
				task.orientate = (ball-runner).angle();
			}
			else{
				task.target_pos = ball + Maths::vector2polar(change_track*magnification, angle);
				task.orientate = (ball - runner).angle();
			}
		}
		else{
			task.orientate = (ball - runner).angle();
			task.target_pos = ball;
		}
	}

	if (r2b_dist<50){
		task.needCb = true;
	}

	return task;
}