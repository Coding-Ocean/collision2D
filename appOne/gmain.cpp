#include"libOne.h"
program()
{
	window(1920, 1080, full);
	//�I�u�W�F�N�g�f�[�^
	VECTOR o(0, 0);
	VECTOR a(1, 0);
	VECTOR b(0.86f, 0.5f);
	//����
	angleMode(DEGREES);
	colorMode(HSV, 100);
	COLOR aCol(0, 50, 100);
	COLOR bCol(210, 50, 100);
	COLOR arcCol(60, 50, 100);
	COLOR cCol(0, 0, 100);
	COLOR tCol(0, 0, 100);
	COLOR backCol(180, 40, 40);
	//�}�E�X�œ_�����ނ��߂̃f�[�^
	VECTOR mouse;
	VECTOR* points[] = { &a,&b };
	int numPoints = sizeof(points) / sizeof(points[0]);
	VECTOR* grabPoint = nullptr;
	float grabRadiusSq = Pow(0.04f, 2);
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
		//���ρE�O�ρE�Ȃ��p
		float dp = a.x * b.x + a.y * b.y;//|a||b|cos��
		float cp = a.x * b.y - a.y * b.x;//|a||b|sin��
		float angleAB = Atan2(cp, dp);
		//�I�u�W�F�N�g�\��
		//��ӁE����
		VECTOR an = normalize(a);
		VECTOR d = an * dp;
		mathLine(o, d, cCol, 2);
		//�����E�O��
		VECTOR oa(-an.y, an.x);
		VECTOR c = oa * cp;
		mathLine(d, d+c, cCol, 2);
		//�Ε�
		mathLine(o, d + c, cCol, 2);
		//�ʁE�Ȃ��p
		mathArc(Atan2(a.y, a.x), angleAB, 0.1f, arcCol, 5);

		mathArrow(o, a, aCol, 6);
		mathText("a", a, BCENTER, tCol, 60);
		mathArrow(0,b, bCol, 6);
		mathText("b", b, BCENTER, tCol, 60);

		print((let)"�Ȃ��p" + angleAB, tCol, 30);
	}
}