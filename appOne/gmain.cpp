#include"libOne.h"

//�����N���X
class SEGMENT {
public:
	VECTOR sp; //�n�_
	VECTOR ep; //�I�_
	VECTOR v; //�n�_����I�_�ւ̃x�N�g��
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
	//����p
	void inverseVector() {
		ep = sp-v;
	}
};

/*
�u�n�_�����_�܂ł̒����v���u�n�_����I�_�܂ł̒����v= t1 or t2 �̋��ߕ�

	�O�ς𗘗p���邪�A�QD�Ȃ̂�Z�����̌v�Z�������B
	����crossZ(a,b)�� a.x*b.y-a.y*b.y ���Ӗ�����B
	���߂���̂̓x�N�g���ł͂Ȃ������B

	�܂��At1�����߂�B
	�u�����Q�̎n�_�v����u�����P�̎n�_�v�܂ł̃x�N�g�� �� v0
	�uv�P(�����P�̃x�N�g��)�̒����v���P�Ƃ����Ƃ���
	�u����1�̎n�_�����_�܂ł̒����v�̔� �� t1
	�uv0+t1*v1�v�Ɓuv2�v�����s�ɂȂ�Ƃ��A���̎������藧��
	crossZ(v0+t1*v1, v2) = 0
	�O�ς����z�@����������
	crossZ(v0,v2) + crossZ(t1*v1,v2) = 0
	�萔�{���g����
	crossZ(v0,v2) + t1*crossZ(v1,v2) = 0
	���P�ɂ��ĉ���
	t1 = -crossZ(v0,v2) / crossZ(v1,v2)
	�O�ς͋t�Ɋ|����ƕ������ς�遨 crossZ(v1,v2) = -crossZ(v2,v1)
	t1 = crossZ(v0,v2) / crossZ(v2,v1)

	t2�����l�ɋ��߂邱�Ƃ��ł���
	-v0�́u�����P�̎n�_�v����u�����Q�̎n�_�v�܂ł̃x�N�g��
	crossZ(-v0+t2*v2, v1) = 0
	crossZ(-v0,v1) + crossZ(t2*v2,v1) = 0
	-crossZ(v0,v1) + t2*crossZ(v2,v1) = 0
	t2 = crossZ(v0,v1) / crossZ(v2,v1)

	t1,t2�����߂�ɂ�
	crossZ(v2,v1)
	crossZ(v0,v2)
	crossZ(v0,v1)
	���v�Z����΂悢�B
*/
//�Q�̐����̌�_�����߂�
bool calcSegmentsIntersectPos(
	const SEGMENT& seg1, //����1
	const SEGMENT& seg2, //����2
	VECTOR* ip //��_
)
{
	float cV2V1 = crossZ(seg2.v, seg1.v);
	print(cV2V1);
	if (cV2V1 == 0.0f) {
		print("���s����`��");
		//���s�Ȃ̂ŁA��_�͂Ȃ�
		return false;
	}
	
	//�n�_1�����_�܂ł̒��� �� �n�_1����I�_1�܂ł̒��� �� t1 �����߂�
	VECTOR v0 = seg1.sp - seg2.sp;
	float cV0V2 = crossZ(v0, seg2.v);
	float t1 = cV0V2 / cV2V1;
	if (t1 < 0 || t1 > 1) {
		//��_�������P���ɂȂ�
		return false;
	}

	//�n�_2�����_�܂ł̒��� �� �n�_2����I�_2�܂ł̒��� �� t2 �����߂�
	float cV0V1 = crossZ(v0, seg1.v);
	float t2 = cV0V1 / cV2V1;
	if (t2 < 0 || t2 > 1) {
		//��_�������Q���ɂȂ�
		return false;
	}
	
	//��_ip
	*ip = seg1.sp + t1 * seg1.v;
	return true;
}

program()
{
	window(1920, 1080, full);
	//�����P�C�Q
	SEGMENT seg1(VECTOR(-0.5f, -0.5f), VECTOR(-0.5f, 0.5f));
	SEGMENT seg2(VECTOR(0.5f, -0.5f), VECTOR(0.5f, 0.5f));
	//�F
	angleMode(DEGREES);
	colorMode(HSV, 100);
	COLOR seg1Col(0, 80, 100);
	COLOR seg2Col(210, 80, 100);
    COLOR pointCol(60, 50, 100);
	COLOR backCol(180, 40, 30);
	COLOR textCol(0, 0, 100);
    axisMode(NODRAW);
	//�}�E�X�œ_�����ނ��߂̃f�[�^
	VECTOR mouse;
	VECTOR* points[] = { &seg1.sp,&seg1.ep, &seg2.sp, &seg2.ep };
	int numPoints = sizeof(points) / sizeof(points[0]);
	VECTOR* grabPoint = nullptr;
	float grabRadiusSq = Pow(0.04f, 2);
	//����p
	COLOR vCol(0, 0, 100);
	COLOR tCol(60, 100, 100);
	int sw = 0;
	int sw2 = 0;
	//���C�����[�v
	loop()
	{
		clear(backCol);
		mathAxis(3.0f);
		//�}�E�X�œ_������ňړ�����
		{
			mouse.set(mathMouseX, mathMouseY);
			if (isPress(MOUSE_LBUTTON)) {
				if (grabPoint == nullptr) {
					//����
					for (int i = 0; i < numPoints; i++) {
						if ((*points[i] - mouse).magSq() <= grabRadiusSq) {
							grabPoint = points[i];
						}
					}
				}
				else {
					//�ړ�
					*grabPoint = mouse;
				}
			}
			else {
				grabPoint = nullptr;
			}
		}
		//�����x�N�g���X�V
		seg1.updateVector();
		seg2.updateVector();
		//��_ip�����߂�
		VECTOR ip;
		bool flag = calcSegmentsIntersectPos(seg1, seg2, &ip);
		//�����`��
		seg1.draw(seg1Col);
		seg2.draw(seg2Col);
		//��_�`��
		if (flag) {
			mathPoint(ip, pointCol, 24);
		}
		//����p���
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