#include <stdio.h>
#include "Kalman.h"


void main()
{
	const float dt = 0.4;
	const Vector2f target_delta( (float)10, (float)10 );
	const float& x = 511;
	const float& y = 194;
	const float& vx = -3;
	const float& vy = 0;
	Matrix2f R;
	R.Init((float)100, (float)0, (float)0, (float)100);
	
	Kalman kalman(dt, target_delta, x, y, vx, vy, R);

	kalman.Predict();

	const float beta = 0.240925908;

	kalman.GainUpdate(beta);

	const std::vector< Vector2f > selected_detections = { {508.000000, 194.000000}, {278.000000, 262.000000}, {516.000000, 205.000000}, {644.000000, 281.000000} };
	const Vector5f beta2(0.497215271, 0, 0.261858821, 0, 0.240925908);
	const float& last_beta = 0.240925908;

	kalman.Update(selected_detections, beta2, last_beta);

}