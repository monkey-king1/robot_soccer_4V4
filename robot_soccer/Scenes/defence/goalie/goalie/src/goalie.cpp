#include "utils/maths.h"
#include <iostream>
using namespace std;
//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{//实地测试得出
	float headlength = 7;
	float miss = 12;//越大接球条件越宽松
	float getangle = PI / 20;
	float guard_circle = 40;
}

bool is_getball(const point2f& ball, const point2f& runner, const float& dir){
	//miss参数需要在比赛时实地调试
	bool get_ball = (ball - runner).length() < miss && (fabs(anglemod(dir - (ball - runner).angle())) < getangle);
	if (get_ball){
		return true;
	}
	else{
		return false;
	}
}

bool is_ready_pass(const point2f& ball, const point2f& passer, const point2f& receiver){
	float receiver_to_ball = (ball - receiver).angle();
	float ball_to_passer = (passer - ball).angle();
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass;
	pass = fabs(receiver_to_ball - ball_to_passer) < 0.4;
	return pass;
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

void get_k_b(const WorldModel* model, float& b_interrupt, float &k_slope){
	const int nCount = 4;//取帧数作为依据
	vector<float> vResult;
	vector<point2f> ball_points;
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
	cout << "k_slope的大小是" << k_slope << endl;
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
	cout << "vel.angle()的大小是" << vel.angle() << endl;
	cout << "angle的大小是" << angle << endl;
}

void defendpos(const WorldModel* model, const point2f& ball, point2f& defend_pos, float& b_interrupt, float& k_slope,float& angle){
	point2f target;
	vector<point2f> ball_points;
	get_k_b(model, b_interrupt, k_slope);
	target.y = - (k_slope*300) + b_interrupt;
	target.x = -300;
	cout << "球的轨迹和边线的交点是" << target << endl;
	defend_pos = target;
}

bool is_inside_penalty(const point2f& p){
	//a为球门左侧点，b为球门右侧点
	const point2f& a = FieldPoint::Penalty_Arc_Center_Left;
	const point2f& b = FieldPoint::Penalty_Arc_Center_Right;
	if (fabs(p.y) < PENALTY_AREA_L / 2)
		return p.x < -FIELD_LENGTH_H + PENALTY_AREA_R && p.x > -FIELD_LENGTH_H;//PENALTY_AREA_R=80
	else if (p.y < 0)
		return (p - a).length() < PENALTY_AREA_R && p.x > -FIELD_LENGTH_H;
	else
		return (p - b).length() < PENALTY_AREA_R&& p.x > -FIELD_LENGTH_H;
}

void get_cross(point2f& temp, const point2f& runner,const float& b_interrupt,const float& k_slope){
	//移动y轴去接球
	temp.y = k_slope*runner.x + b_interrupt;
	temp.x = runner.x;
	cout << "temp的大小是" << temp<<endl;
}

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	point2f middle(0, 0);
	const point2f& ball = model->get_ball_pos();
	const point2f& last_ball = model->get_ball_pos(1);
	const point2f& golie_pos = model->get_our_player_pos(robot_id);
	const point2f goal(-300,0);
	const point2f& right_goal = FieldPoint::Goal_Center_Right_Two_Point;
	const point2f& left_goal = FieldPoint::Goal_Center_Left_Two_Point;
	const float& dir = model->get_our_player_dir(robot_id);
	float b_interrupt(0);
	float k_slope(0);
	float angle(0);//小球运动的角度
	float robot_head = 7;
	bool is_readeykick = is_ready_pass(ball, golie_pos, middle);
	bool ball_inside_penalty = is_inside_penalty(ball);
	bool ismove = (ball - last_ball).length() > 0.8 ? true:false ;
	point2f defend_pos;
	point2f temp(0,0);
	float get_miss = 10;
	float predict_dist = 20;
	float in_predict_dist = 40;

	if (ball_inside_penalty){
		//判断小球在控球嘴上执行挑球
		if (is_getball(ball, golie_pos, dir)){
			task.kickPower = 127;
			task.needKick = true;
			task.isChipKick = true;
			task.target_pos = golie_pos;
			task.orientate = dir;
		}
		else{
			if (ismove){
				defendpos(model, ball, defend_pos,b_interrupt,k_slope,angle);
				if (defend_pos.y > (left_goal.y - get_miss) && defend_pos.y < (right_goal.y + get_miss)){//交点在门框内
					get_cross(temp, golie_pos, b_interrupt,k_slope);
					task.target_pos = temp;
					task.orientate = (ball - temp).angle();
				}
				else{
					temp = ball + Maths::vector2polar(in_predict_dist, angle);
					task.orientate = (temp - goal).angle();
					task.target_pos = goal + Maths::vector2polar(guard_circle, (temp - goal).angle());
				}
			}
			else{

				task.target_pos = ball;
				task.orientate = (ball - golie_pos).angle();
			}
		}
	}
	//小球不在禁区内执行守门员防守
	else
	{
		if (ismove){
			defendpos(model, ball, defend_pos, b_interrupt, k_slope, angle);
			if (defend_pos.y > (left_goal.y - get_miss) && defend_pos.y < (right_goal.y + get_miss)){//球的轨迹在门内
				get_cross(temp, golie_pos, b_interrupt, k_slope);
				task.target_pos = temp;
				task.orientate = (ball - temp).angle();
			}
			else{
				temp = ball + Maths::vector2polar(predict_dist, angle);
				task.orientate = (temp - goal).angle();
				task.target_pos = goal + Maths::vector2polar(guard_circle, (temp - goal).angle());
			}
		}
		else{
			task.orientate = (ball - goal).angle();
			task.target_pos = goal + Maths::vector2polar(guard_circle, (ball - goal).angle());
		}
	}

	if ((ball-golie_pos).length()<40){
		task.needCb = true;
	}

	task.flag = 1;
	return task;
}