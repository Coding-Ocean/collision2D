#include"libOne.h"
program()
{
	window(1920, 1080, full);
	//オブジェクトデータ
	VECTOR o(0, 0);
	VECTOR a(1, 0);
	VECTOR b(0.86f, 0.5f);
	//いろ
	angleMode(DEGREES);
	colorMode(HSV, 100);
	COLOR aCol(0, 50, 100);
	COLOR bCol(210, 50, 100);
	COLOR arcCol(60, 50, 100);
	COLOR cCol(0, 0, 100);
	COLOR tCol(0, 0, 100);
	COLOR backCol(180, 40, 40);
	//マウスで点をつかむためのデータ
	VECTOR mouse;
	VECTOR* points[] = { &a,&b };
	int numPoints = sizeof(points) / sizeof(points[0]);
	VECTOR* grabPoint = nullptr;
	float grabRadiusSq = Pow(0.04f, 2);
	//メインループ
	loop()
	{
		clear(backCol);
		mathAxis(3.0f);
		//マウスで点をつかんで移動する
		{
			mouse.set(mathMouseX, mathMouseY);
			if (isPress(MOUSE_LBUTTON)) {
				if (grabPoint == nullptr) {
					//つかむ
					for (int i = 0; i < numPoints; i++) {
						if ((*points[i] - mouse).magSq() <= grabRadiusSq) {
							grabPoint = points[i];
						}
					}
				}
				else {
					//移動
					*grabPoint = mouse;
				}
			}
			else {
				grabPoint = nullptr;
			}
		}
		//内積・外積・なす角
		float dp = a.x * b.x + a.y * b.y;//|a||b|cosθ
		float cp = a.x * b.y - a.y * b.x;//|a||b|sinθ
		float angleAB = Atan2(cp, dp);
		//オブジェクト表示
		//底辺・内積
		VECTOR an = normalize(a);
		VECTOR d = an * dp;
		mathLine(o, d, cCol, 2);
		//高さ・外積
		VECTOR oa(-an.y, an.x);
		VECTOR c = oa * cp;
		mathLine(d, d+c, cCol, 2);
		//斜辺
		mathLine(o, d + c, cCol, 2);
		//弧・なす角
		mathArc(Atan2(a.y, a.x), angleAB, 0.1f, arcCol, 5);

		mathArrow(o, a, aCol, 6);
		mathText("a", a, BCENTER, tCol, 60);
		mathArrow(0,b, bCol, 6);
		mathText("b", b, BCENTER, tCol, 60);

		print((let)"なす角" + angleAB, tCol, 30);
	}
}