#include"libOne.h"

//線分クラス
class SEGMENT {
public:
	VECTOR sp; //始点
	VECTOR ep; //終点
	VECTOR v; //始点から終点へのベクトル
	SEGMENT(const VECTOR& sp, const VECTOR& ep)
		:sp(sp)
		,ep(ep) 
	{
		updateVector();
	}
	void updateVector() 
	{
		v = ep - sp;
	}
	void draw(const COLOR& c) 
	{
		mathArrow(sp, ep, c, 10);
	}
	//解説用
	void inverseVector() {
		ep = sp-v;
	}
};

/*
「始点から交点までの長さ」÷「始点から終点までの長さ」= t1 or t2 の求め方

	外積を利用するが、２DなのでZ成分の計算式だけ。
	下のcrossZ(a,b)は a.x*b.y-a.y*b.y を意味する。
	求められるのはベクトルではなく実数。

	まず、t1を求める。
	「線分２の始点」から「線分１の始点」までのベクトル ⇒ v0
	「v１(線分１のベクトル)の長さ」を１としたときの
	「線分1の始点から交点までの長さ」の比 ⇒ t1
	「v0+t1*v1」と「v2」が平行になるとき、次の式が成り立つ
	crossZ(v0+t1*v1, v2) = 0
	外積も分配法則をつかえる
	crossZ(v0,v2) + crossZ(t1*v1,v2) = 0
	定数倍も使える
	crossZ(v0,v2) + t1*crossZ(v1,v2) = 0
	ｔ１について解く
	t1 = -crossZ(v0,v2) / crossZ(v1,v2)
	外積は逆に掛けると符号が変わる→ crossZ(v1,v2) = -crossZ(v2,v1)
	t1 = crossZ(v0,v2) / crossZ(v2,v1)

	t2も同様に求めることができる
	-v0は「線分１の始点」から「線分２の始点」までのベクトル
	crossZ(-v0+t2*v2, v1) = 0
	crossZ(-v0,v1) + crossZ(t2*v2,v1) = 0
	-crossZ(v0,v1) + t2*crossZ(v2,v1) = 0
	t2 = crossZ(v0,v1) / crossZ(v2,v1)

	t1,t2を求めるには
	crossZ(v2,v1)
	crossZ(v0,v2)
	crossZ(v0,v1)
	を計算すればよい。
*/
//２つの線分の交点を求める
bool calcSegmentsIntersectPos(
	const SEGMENT& seg1, //線分1
	const SEGMENT& seg2, //線分2
	VECTOR* ip //交点
)
{
	float cV2V1 = crossZ(seg2.v, seg1.v);
	print(cV2V1);
	if (cV2V1 == 0.0f) {
		print("平行だよ～ん");
		//平行なので、交点はない
		return false;
	}
	
	//始点1から交点までの長さ ÷ 始点1から終点1までの長さ ⇒ t1 を求める
	VECTOR v0 = seg1.sp - seg2.sp;
	float cV0V2 = crossZ(v0, seg2.v);
	float t1 = cV0V2 / cV2V1;
	if (t1 < 0 || t1 > 1) {
		//交点が線分１内にない
		return false;
	}

	//始点2から交点までの長さ ÷ 始点2から終点2までの長さ ⇒ t2 を求める
	float cV0V1 = crossZ(v0, seg1.v);
	float t2 = cV0V1 / cV2V1;
	if (t2 < 0 || t2 > 1) {
		//交点が線分２内にない
		return false;
	}
	
	//交点ip
	*ip = seg1.sp + t1 * seg1.v;
	return true;
}

program()
{
	window(1920, 1080, full);
	//線分１，２
	SEGMENT seg1(VECTOR(-0.5f, -0.5f), VECTOR(-0.5f, 0.5f));
	SEGMENT seg2(VECTOR(0.5f, -0.5f), VECTOR(0.5f, 0.5f));
	//色
	angleMode(DEGREES);
	colorMode(HSV, 100);
	COLOR seg1Col(0, 80, 100);
	COLOR seg2Col(210, 80, 100);
    COLOR pointCol(60, 50, 100);
	COLOR backCol(180, 40, 30);
	COLOR textCol(0, 0, 100);
    axisMode(NODRAW);
	//マウスで点をつかむためのデータ
	VECTOR mouse;
	VECTOR* points[] = { &seg1.sp,&seg1.ep, &seg2.sp, &seg2.ep };
	int numPoints = sizeof(points) / sizeof(points[0]);
	VECTOR* grabPoint = nullptr;
	float grabRadiusSq = Pow(0.04f, 2);
	//解説用
	COLOR vCol(0, 0, 100);
	COLOR tCol(60, 100, 100);
	int sw = 0;
	int sw2 = 0;
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
		//線分ベクトル更新
		seg1.updateVector();
		seg2.updateVector();
		//交点ipを求める
		VECTOR ip;
		bool flag = calcSegmentsIntersectPos(seg1, seg2, &ip);
		//線分描画
		seg1.draw(seg1Col);
		seg2.draw(seg2Col);
		//交点描画
		if (flag) {
			mathPoint(ip, pointCol, 24);
		}
		//解説用情報
		{
			if (isTrigger(KEY_X)) { seg2.inverseVector();}
			if (flag) {
				if (isTrigger(KEY_SPACE)) { ++sw %= 3; }
				if (isTrigger(KEY_Z)) { ++sw2 %= 2; }
				if (sw == 1) {
					mathArrow(seg2.sp, seg1.sp, vCol, 2);
					mathText("V0", seg1.sp + (seg1.sp - seg2.sp).normalize() * 0.2f, BCENTER, textCol, 50);
					mathArrow(seg1.sp, ip, vCol, 2);
					mathText("t1*V1", ip + VECTOR(-0.5f, 0), TOP, textCol, 50);
					mathArrow(seg2.sp, ip, tCol, 6);
					mathText("crossZ(V0+t1*V1, V2)=0", VECTOR(-1, 1.5f), TOP, textCol, 50);
					if (sw2 == 1) {
						mathText("t1=crossZ(V0,V2)/crossZ(V2,V1)", VECTOR(-1, 1.3f), TOP, textCol, 50);
					}
				}
				else if (sw == 2) {
					mathArrow(seg1.sp, seg2.sp, vCol, 2);
					mathText("-V0", seg2.sp + (seg2.sp - seg1.sp).normalize() * 0.12f, BCENTER, textCol, 50);
					mathArrow(seg2.sp, ip, vCol, 2);
					mathText(" t2*V2", ip, BOTTOM, textCol, 50);
					mathArrow(seg1.sp, ip, tCol, 6);
					mathText("crossZ(-V0+t2*V2, V1)=0", VECTOR(-1, 1.5f), TOP, textCol, 50);
					mathText("t2=crossZ(V0,V1)/crossZ(V2,V1)", VECTOR(-1, 1.3f), TOP, textCol, 50);
				}
			}
			mathText("V1", seg1.ep + (seg1.ep - seg1.sp).normalize() * 0.12f, BCENTER, textCol, 50);
			mathText("V2", seg2.ep + (seg2.ep - seg2.sp).normalize() * 0.12f, BCENTER, textCol, 50);
		}
	}
}