#include "utils\maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

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

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	float get_miss = 0;
	point2f opp_goal = -FieldPoint::Goal_Center_Point;
	const point2f& ball = model->get_ball_pos();
	const point2f& runner = model->get_our_player_pos(robot_id);
	const point2f& vel = model->get_ball_vel();
	const float& dir = model->get_our_player_dir(robot_id);
	float r2b_dist = (runner - ball).length();   //球员到球的距离
	float b_interrupt(0);
	float k_slope(0);
	float angle(0);//小球运动的角度
	vector<point2f> ball_points;
	int convert_get = (vel.angle()<PI / 2 && vel.angle()>(-PI / 2)) ? 1 :-1 ;
	get_k_b(model, b_interrupt, k_slope, ball_points);
	get_angle(angle, k_slope, vel);


	point2f y1(-300.0, runner.y);
	point2f y2(300.0, runner.y);
	point2f x1(runner.x, 150.0);
	point2f x2(runner.x, -150.0);
	point2f y3 = ball + Maths::vector2polar(600, angle);
	point2f target = Maths::across_point(y1, y2, ball, y3);
	point2f target_x = Maths::across_point(x1, x2, ball, y3);
	float x_dist = (target - runner).length();//x轴距离
	float y_dist = (target_x - runner).length();//y轴距离
	int convert = runner.y < 0 ? 1: -1;
	float delt_angle = 0.12;

	if (runner.y>0){
		opp_goal.y -= 20;
	}
	else{
		opp_goal.y += 20;
	}

	//离球距离小于200才开始接球，否则保持静止
	if (r2b_dist < 200){
		//变动x坐标接球
		if (x_dist<y_dist){
			task.target_pos.y = runner.y ;
			task.target_pos.x = target.x;
			if (r2b_dist < 10){
				task.target_pos = runner;
			}
		}
		//变动y坐标接球
		else{
			task.target_pos.x = runner.x;
			task.target_pos.y = target_x.y;
			if (r2b_dist < 10){
				task.target_pos = runner;
			}
		}
	}
	else
	{
		task.target_pos = runner;
	}
	

	if (runner.y>0){//在前场
		delt_angle = 0.12;
	}
	else{
		delt_angle = 0.6;
	}

	task.orientate = ((opp_goal - runner).angle() );
	if (r2b_dist<40){
		task.needCb = true;
	}
	task.target_pos.x += convert_get*get_miss;
	return task;
}