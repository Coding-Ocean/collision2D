#include"libOne.h"
program()
{
	window(1920, 1080, full);
	//�I�u�W�F�N�g�f�[�^
	VECTOR o(0, 0);
	VECTOR a(0.3f, 0.2f);
	VECTOR b(-0.3f, 0.2f);
	//�}�E�X�œ_�����ނ��߂̃f�[�^
	VECTOR mouse;
	VECTOR* points[] = { &a,&b };
	int numPoints = sizeof(points) / sizeof(points[0]);
	VECTOR* grabPoint = nullptr;
	float grabRadiusSq = Pow(0.04f, 2);
	//���C�����[�v
	loop()
	{
		clear(0, 60, 0);
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
		//�I�u�W�F�N�g�\��
		mathArrow(o, a, COLOR(255, 200, 200), 10);
		mathText("a", a, BCENTER, COLOR(255, 255, 255), 60);
		mathPoint(b, COLOR(200, 200, 255), 20);
	}
}