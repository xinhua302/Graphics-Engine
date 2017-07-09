#ifndef __CAMERA_H
#define __CAMERA_H
#include "D3dUtility.h"
class Camera
{
public:
    Camera();
    ~Camera();

public:
    //���λ��
    XMVECTOR GetPositionXM()const;
    XMFLOAT3 GetPosition()const;
    void SetPosition(float x, float y, float z);
    void SetPosition(const XMFLOAT3& v);

    //�ϡ��ҡ���������
    XMVECTOR GetRightXM()const;
    XMFLOAT3 GetRight()const;
    XMVECTOR GetUpXM()const;
    XMFLOAT3 GetUp()const;
    XMVECTOR GetLookXM()const;
    XMFLOAT3 GetLook()const;

    //�ü���
    float GetNearZ()const;
    float GetFarZ()const;
    float GetAspect()const;
    float GetFovY()const;
    float GetFovX()const;
    float GetNearWindowWidth()const;
    float GetNearWindowHeight()const;
    float GetFarWindowWidth()const;
    float GetFarWindowHeight()const;
    
    //����͸��
    void SetLens(float fovY, float aspect, float zn, float zf);

    //����
    void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
    void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

    //ƽ��
    void Strafe(float d);
    //��ǰ
    void Walk(float d);

    //��ת
    void Pitch(float angle);
    void RotateY(float angle);

    //���¾���
    void UpdateViewMatrix();

    //�任����
    XMMATRIX View()const;
    XMMATRIX Proj()const;
    XMMATRIX ViewProj()const;


private:
    //���λ��
    XMFLOAT3 m_Position;
    //���������
    XMFLOAT3 m_Up;
    //���������
    XMFLOAT3 m_Right;
    //�����������
    XMFLOAT3 m_Look;

    //���ü������
    float m_NearZ;
    //Զ�ü������
    float m_FarZ;
    //�������
    float m_FovY;
    //��߱�
    float m_Aspect;
    //���ü���߶�
    float m_NearWindowHeight;
    //Զ�ü���߶�
    float m_FarWindowHeight;

    //�������
    XMFLOAT4X4 m_View;
    //͸�Ӿ���
    XMFLOAT4X4 m_Proj;
};
#endif