#include"libOne.h"
/*
円と直線の交点座標ip(intersection point)を求める計算

重要→円の中心を原点として考えていく
「円の中心(原点)」から「円と線分の交差点」までの長さは必ず半径rとなる。
「円の中心(原点)」から「円と線分の交差点」までのベクトルをipとすると、
r^2 = dot(ip,ip)・・・式１
線分の始点をp、終点に向かう正規化ベクトルをv、pからipまでの長さをlとすると、
ip = p + lv・・・式２
となる。
式２を式１に代入すると
r^2 = dot(p+lv, p+lv);
この式のlを求めていく。まずは内積の分配をしまくる
r^2 = dot(p,p)+dot(p,lv)+dot(lv,p)+dot(lv,lv)
lを外に出し、解の公式に当てはめられるように変形
dot(v,v)*l^2 + 2dot(p,v)*l + (dot(p,p)-r^2) = 0
解の公式によりlを求められる
l = (2dot(p,v) ± √( (2dot(p,v))^2 - 4dot(v,v)(dot(p,p)-r^2) ) ) / 2dot(v,v)
２は消せるので
l = (dot(p,v) ± √(dot(p,v)^2 - dot(v,v)(dot(p,p)-r^2))) / dot(v,v)
vは正規化ベクトルなので
l = dot(p,v) ± √(dot(p,v)^2 - dot(p,p)-r^2)
*/
/*
すみません。動画で紹介している関数名とは別の名前にしました
calcSegmentCircleInsectPosを
calcLineCircleIntersectPosに変更しました
*/
bool calcLineCircleIntersectPos(
    VECTOR cp,//円の中心点
    float r,//円の半径
    VECTOR p,//線分の始点
    VECTOR v,//終点へのベクトル
    VECTOR* ip1,//交点１
    VECTOR* ip2//交点２
) {
    //円の中心点が原点になるようにする
    p -= cp;
    //線分の方向ベクトルを正規化
    v.normalize();
    //ipまでの長さlを求める
    float dPV = dot(p, v);
    float dPP = dot(p, p);
    float ss = dPV * dPV - dPP + r * r;//解の公式ルートの中
    if (Abs(ss) < 0.000001f)
        ss = 0.0f; //誤差修正
    if (ss < 0.0f)
        return false; //ルートの中がマイナスのとき解は無い＝衝突していない
    float s = Sqrt(ss);
    float l1 = -dPV - s;
    float l2 = -dPV + s;
    //交点座標
    if (ip1 != 0) {
        *ip1 = p + l1 * v + cp;
    }
    if (ip2 != 0) {
        *ip2 = p + l2 * v + cp;
    }
    return true;
}

program()
{
	window(1920, 1080, full);
    //円
    VECTOR cp(0, 0);
    float r = 0.5f;
    //線分
	VECTOR sp(0, -1);
	VECTOR ep(0.4f, 0);
	//いろ
	angleMode(DEGREES);
	colorMode(HSV, 100);
	COLOR segCol(0, 50, 100);
	COLOR circleCol(210, 50, 100);
    COLOR pointCol(60, 50, 100);
	COLOR tCol(0, 0, 100);
	COLOR backCol(180, 40, 30);
    axisMode(NODRAW);
	//マウスで点をつかむためのデータ
	VECTOR mouse;
	VECTOR* points[] = { &ep, &sp, &cp };
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
        //円と線分
        mathCircle(cp, r + r, COLOR(0, 0, 0, 0), circleCol, 8);
        mathArrow(sp, ep, segCol, 8);
        //交点 intersection point
        {
            VECTOR ip1, ip2;
            VECTOR v = ep - sp;
            bool flag = calcLineCircleIntersectPos(
                cp, r, sp, v, &ip1, &ip2
            );
            if (flag) {
                //交点が線分内にあったら表示
                float t;
                t = dot(v, ip1 - sp) / v.magSq();
                if (0.0f <= t && t <= 1.0f)
                    mathPoint(ip1, pointCol, 24);
                t = dot(v, ip2 - sp) / v.magSq();
                if (0.0f <= t && t <= 1.0f)
                    mathPoint(ip2, pointCol, 24);
            }
        }
	}
}