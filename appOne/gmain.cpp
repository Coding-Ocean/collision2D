#include"libOne.h"
/*
�~�ƒ����̌�_���Wip(intersection point)�����߂�v�Z

�d�v���~�̒��S�����_�Ƃ��čl���Ă���
�u�~�̒��S(���_)�v����u�~�Ɛ����̌����_�v�܂ł̒����͕K�����ar�ƂȂ�B
�u�~�̒��S(���_)�v����u�~�Ɛ����̌����_�v�܂ł̃x�N�g����ip�Ƃ���ƁA
r^2 = dot(ip,ip)�E�E�E���P
�����̎n�_��p�A�I�_�Ɍ��������K���x�N�g����v�Ap����ip�܂ł̒�����l�Ƃ���ƁA
ip = p + lv�E�E�E���Q
�ƂȂ�B
���Q�����P�ɑ�������
r^2 = dot(p+lv, p+lv);
���̎���l�����߂Ă����B�܂��͓��ς̕��z�����܂���
r^2 = dot(p,p)+dot(p,lv)+dot(lv,p)+dot(lv,lv)
l���O�ɏo���A���̌����ɓ��Ă͂߂���悤�ɕό`
dot(v,v)*l^2 + 2dot(p,v)*l + (dot(p,p)-r^2) = 0
���̌����ɂ��l�����߂���
l = (2dot(p,v) �} ��( (2dot(p,v))^2 - 4dot(v,v)(dot(p,p)-r^2) ) ) / 2dot(v,v)
�Q�͏�����̂�
l = (dot(p,v) �} ��(dot(p,v)^2 - dot(v,v)(dot(p,p)-r^2))) / dot(v,v)
v�͐��K���x�N�g���Ȃ̂�
l = dot(p,v) �} ��(dot(p,v)^2 - dot(p,p)-r^2)
*/
/*
���݂܂���B����ŏЉ�Ă���֐����Ƃ͕ʂ̖��O�ɂ��܂���
calcSegmentCircleInsectPos��
calcLineCircleIntersectPos�ɕύX���܂���
*/
bool calcLineCircleIntersectPos(
    VECTOR cp,//�~�̒��S�_
    float r,//�~�̔��a
    VECTOR p,//�����̎n�_
    VECTOR v,//�I�_�ւ̃x�N�g��
    VECTOR* ip1,//��_�P
    VECTOR* ip2//��_�Q
) {
    //�~�̒��S�_�����_�ɂȂ�悤�ɂ���
    p -= cp;
    //�����̕����x�N�g���𐳋K��
    v.normalize();
    //ip�܂ł̒���l�����߂�
    float dPV = dot(p, v);
    float dPP = dot(p, p);
    float ss = dPV * dPV - dPP + r * r;//���̌������[�g�̒�
    if (Abs(ss) < 0.000001f)
        ss = 0.0f; //�덷�C��
    if (ss < 0.0f)
        return false; //���[�g�̒����}�C�i�X�̂Ƃ����͖������Փ˂��Ă��Ȃ�
    float s = Sqrt(ss);
    float l1 = -dPV - s;
    float l2 = -dPV + s;
    //��_���W
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
    //�~
    VECTOR cp(0, 0);
    float r = 0.5f;
    //����
	VECTOR sp(0, -1);
	VECTOR ep(0.4f, 0);
	//����
	angleMode(DEGREES);
	colorMode(HSV, 100);
	COLOR segCol(0, 50, 100);
	COLOR circleCol(210, 50, 100);
    COLOR pointCol(60, 50, 100);
	COLOR tCol(0, 0, 100);
	COLOR backCol(180, 40, 30);
    axisMode(NODRAW);
	//�}�E�X�œ_�����ނ��߂̃f�[�^
	VECTOR mouse;
	VECTOR* points[] = { &ep, &sp, &cp };
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
        //�~�Ɛ���
        mathCircle(cp, r + r, COLOR(0, 0, 0, 0), circleCol, 8);
        mathArrow(sp, ep, segCol, 8);
        //��_ intersection point
        {
            VECTOR ip1, ip2;
            VECTOR v = ep - sp;
            bool flag = calcLineCircleIntersectPos(
                cp, r, sp, v, &ip1, &ip2
            );
            if (flag) {
                //��_���������ɂ�������\��
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