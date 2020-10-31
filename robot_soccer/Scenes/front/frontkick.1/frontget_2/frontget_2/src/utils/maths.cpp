#include "maths.h"
#include "PlayerTask.h"
#include "worldmodel.h"
//#include "A_Star.h"
namespace FieldPoint{
	point2f Goal_Center_Point(-FIELD_LENGTH / 2, 0.0);//敌方球门终点
	point2f Penalty_Kick_Point(-FIELD_LENGTH / 2 + PENALTY_KICKER_L, 0.0);//罚球点
	point2f Goal_Left_Point(-FIELD_LENGTH_H, -GOAL_WIDTH_H);//球门负边界
	point2f Goal_Right_Point(-FIELD_LENGTH_H, GOAL_WIDTH_H);//球门正边界
	point2f Goal_Center_Left_One_Point(-FIELD_LENGTH / 2, -PENALTY_BISECTOR);
	point2f Goal_Center_Left_Two_Point(-FIELD_LENGTH / 2, -PENALTY_BISECTOR * 2);
	point2f Goal_Center_Right_One_Point(-FIELD_LENGTH / 2, PENALTY_BISECTOR);
	point2f Goal_Center_Right_Two_Point(-FIELD_LENGTH / 2, PENALTY_BISECTOR * 2);
	point2f Goal_Penalty_Area_L_Right(-FIELD_LENGTH, PENALTY_AREA_L / 2);//禁区前面横线的正边
	point2f Goal_Penalty_Area_L_Left(-FIELD_LENGTH, -PENALTY_AREA_L / 2);//禁区前面横线的负边
	point2f Penalty_Area_L_A(PENALTY_AREA_R - FIELD_LENGTH_H, PENALTY_AREA_L / 2);//禁区前面横线的正边
	point2f Penalty_Area_L_B(PENALTY_AREA_R - FIELD_LENGTH_H, -PENALTY_AREA_L / 2);//禁区前面横线的负边
	point2f Penalty_Arc_Center_Right(-FIELD_LENGTH_H, PENALTY_AREA_L/2);
	point2f Penalty_Arc_Center_Left(-FIELD_LENGTH_H,-PENALTY_AREA_L/2);
	point2f Penalty_Rectangle_Left(-FIELD_LENGTH_H + PENALTY_AREA_R, -PENALTY_AREA_L / 2);
	point2f Penalty_Rectangle_Right(-FIELD_LENGTH_H + PENALTY_AREA_R, PENALTY_AREA_L / 2);
}
 namespace Maths{
	  float normalize(float theta)
	 {
		 if (theta >= -PI && theta < PI)
			 return theta;
		 int multiplier = (int)(theta / (2 * PI));
		 theta = theta - multiplier * 2 * PI;
		 if (theta >= PI)
			 theta -= 2 * PI;
		 if (theta < -PI)
			 theta += 2 * PI;
		 return theta;
	 }

	 inline point2f vector2polar(float length, float dir){
		 return point2f(length*cos(dir), length*sin(dir));
	 }

	 point2f circle_segment_intersection(const point2f& start_point, const double circle_r, const  point2f& end_point){
		 float orientation_ang = (end_point - start_point).angle();
		 return start_point + vector2polar(circle_r, orientation_ang);
	 }

	 point2f archimedes_spiral(const point2f& spiral_center, float spiral_center_size, const point2f& pos, float spiral_buff){
		 point2f spiral_point;
		 float dist = (pos - spiral_center).length();
		 float angle = (pos - spiral_center).angle();
		 float dist_step = 8.0f;           //依据旋转180，5次旋转成功，对应dist_step 为40/5-- fuck 执行和思路有点偏差 先不管
		 float ang_step = PI / 180 * 36;
		 float ang_deta = Maths::normalize(angle - ang_step);
		 float dist_deta = dist - dist_step;
		 if (dist_deta < spiral_center_size + spiral_buff) dist_deta = spiral_center_size + spiral_buff;
		 spiral_point = spiral_center + Maths::vector2polar(dist_deta,ang_deta);
		 return spiral_point;
	 }

	  int sign(float d){ return (d >= 0) ? 1 : -1; }

	  float vector_angle(const point2f& a, const point2f& b, const point2f& c){
		  return  (angle_mod((a - b).angle() - (c - b).angle()));
	  }

	  float least_squares(const vector<point2f>& points){
		  float x_mean = 0;
		  float y_mean = 0;
		  float Dxx = 0, Dxy = 0, Dyy = 0;
		  float a = 0, b = 0;
		  for (int i = 0; i < points.size(); i++)
		  {
			  x_mean += points[i].x;
			  y_mean += points[i].y;
		  }
		  x_mean /= points.size();
		  y_mean /= points.size(); //至此，计算出了 x y 的均值

		  
		  for (int i = 0; i < points.size(); i++)
		  {
			  Dxx += (points[i].x - x_mean) * (points[i].x - x_mean);
			  Dxy += (points[i].x - x_mean) * (points[i].y - y_mean);
			  Dyy += (points[i].y - y_mean) * (points[i].y - y_mean);
		  }

		  double lambda = ((Dxx + Dyy) - sqrt((Dxx - Dyy) * (Dxx - Dyy) + 4 * Dxy * Dxy)) / 2.0;
		  double den = sqrt(Dxy * Dxy + (lambda - Dxx) * (lambda - Dxx));
		  
		  if (fabs(den) < 1e-5)
		  {
			  return 0.0;
		  }
		  else
		  {
			  a = Dxy / den;
			  b = (lambda - Dxx) / den;
			  return atan(-a/b);
		  }
	  }

	  point2f line_perp_across(const point2f& p1, float slope, const point2f& p2){
		  float a, b, b1;
		  if (isnan(slope)){
			  a = 1;
			  b = 0;
		  }
		  else if (fabs(slope) < 0.00001){
			  a = 0;
			  b = 1;
		  }
		  else
		  {
			  a = slope;
			  b = p1.y - a*p1.x;
		  }
		  b1 = -p2.x - a*p2.y;
		  point2f across_point;
		  across_point.x = (-b1 - a*b) / (a*a + 1);
		  across_point.y = a * across_point.x + b;
		  return across_point;
	  }

	  bool  in_range(const point2f& p1, const point2f& left_down, const point2f& right_up){
		  return (p1.x < right_up.x && p1.x > left_down.x) && (p1.y < right_up.y && p1.y > left_down.y);
	  }

	  point2f across_point(const point2f &p1, const point2f &p2, const point2f &q1, const point2f &q2){
		point2f project_point_a = point_on_segment(p1,p2,q1,true);
		point2f project_point_b = point_on_segment(p1,p2,q2,true);
		float a = (q1 - project_point_a).length();
		float b = (q2 - project_point_b).length();
		return (project_point_b*(a / (a + b)) + project_point_a*(b / (a + b)));
	}
			
	  bool is_inside_penatly(const point2f& ball){
		  if (fabs(ball.x) > FIELD_LENGTH_H - PENALTY_AREA_R  && ball.x < 0){
			  if (fabs(ball.y) < PENALTY_AREA_L / 2)
				  return true;
			  else if (ball.y < 0 && (ball - FieldPoint::Goal_Penalty_Area_L_Left).length() < PENALTY_AREA_R)
				  return true;
			  else if (ball.y > 0 && (ball - FieldPoint::Goal_Penalty_Area_L_Right).length() < PENALTY_AREA_R)
				  return true;
			  else
				  return false;
		  }
		  return false;
	  }
	  //动作方绕某点朝特定方向转动
	  void circle_move(const WorldModel* model, const int& robot_id, PlayerTask& task, const point2f& executer_p, const point2f& pos_p, const float& circleR, const float& angle){
		  bool add;
		  int orbit = -1;
		  const float DetAngle(0.6);
		  const float pos = angle;
		  const float executer = model->get_our_player_dir(robot_id);
		  float toPosDist = (executer_p - pos_p).length();

		  if (toPosDist > circleR + 10){
			  orbit = 0;//小车在圆外,则让小车运动到圆上
		  }
		  else{
			  orbit = 1;//运动到了圆上之后开始转圈
		  }
		  switch (orbit){
		  case 0:
			  task.target_pos = pos_p + Maths::vector2polar(circleR, (executer_p - pos_p).angle());
			  task.orientate = (pos_p - executer_p).angle();
			  break;
		  case 1:{
					 if (pos*executer > 0){
						 if (executer > 0){
							 if (executer > pos)
								 add = false;
							 else
								 add = true;
						 }
						 else{
							 if (executer > pos)
								 add = false;
							 else
								 add = true;
						 }
					 }
					 else{
						 if (executer > 0)
							 add = true;
						 else
							 add = false;
					 }
					 if (add)//当 add 为 true 时，顺时针移动
					 {
						 //+朝 x 正轴顺时针方向移动
						 task.target_pos = pos_p + Maths::vector2polar(circleR, (executer_p - pos_p).angle() + DetAngle);
						 task.orientate = (pos_p - executer_p).angle();
					 }
					 else
					 {
						 //-朝 x 正轴逆时针方向移动
						 task.target_pos = pos_p + Maths::vector2polar(circleR, (executer_p - pos_p).angle() - DetAngle);
						 task.orientate = (pos_p - executer_p).angle();
					 }
					 break;
		  }
		  }
	  }

	  bool block_shoot(const WorldModel* model, const point2f& executer, const point2f& receiver, int& block_id){
		  //初始化判断变量is_block
		  bool is_block = false;
		  const bool* opp_exist_id = model->get_opp_exist_id();
		  const bool* our_exist_id = model->get_our_exist_id();
		  for (int i = 0; i < MAX_ROBOTS; i++){
			  //判断敌方车号
			  if (opp_exist_id[i]){
				  const point2f& pos = model->get_opp_player_pos(i);
				  //point_on_segment(v0, v1, p, flages),求p点到v0v1线段上的最近点
				  point2f nearest_p = Vector::point_on_segment(executer, receiver, pos, true);
				  float dist = (nearest_p - pos).length();
				  if (dist < MAX_ROBOT_SIZE / 2){
					  block_id = i;
					  is_block = true;
					  break;
				  }
			  }
			  if (our_exist_id[i]){
				  const point2f& pos = model->get_our_player_pos(i);
				  point2f nearest_p = Vector::point_on_segment(executer, receiver, pos, true);
				  float dist = (nearest_p - pos).length();
				  if (dist < MAX_ROBOT_SIZE / 2){
					  block_id = i + 12;//作为参数传出去的时候可以用来区别己方阻挡还是地方阻挡
					  is_block = true;
					  break;
				  }
			  }
		  }
		   return is_block;
	  }

	  //void get_obstacle(int robot_id, const WorldModel* model, point2f* obstacle,const bool *our_id,const bool *opp_id,const point2f& ball){
		 // int i = 0;
		 // for (int j = 0; j<MAX_ROBOTS; j++){
			//  if (our_id[j] && (j != robot_id)){
			//	  obstacle[i++] = model->get_our_player_pos(j);
			//  }
			//  if (opp_id[j]){
			//	  obstacle[i++] = model->get_opp_player_pos(j);
			//  }
		 // }
		 // //小球作为最后一个障碍物
		 // obstacle[7] = ball;
	  //}

	  //point2f GetNextPos(const WorldModel* model, int robot_id, const point2f& start, const point2f& end){
		 // const bool* opp_exist_id = model->get_opp_exist_id();
		 // const bool* our_exist_id = model->get_our_exist_id();
		 // const point2f ball = model->get_ball_pos();
		 // //7个机器人，1个球
		 // point2f obstacle[8];
		 // Maths::get_obstacle(robot_id, model, obstacle, opp_exist_id, our_exist_id, ball);
		 // point2f next_pos;
		 // next_pos = get_track(start, end, obstacle);
		 // return next_pos;
	  //}

}