#ifndef __CAMERA_H
#define __CAMERA_H
#include "D3dUtility.h"
class Camera
{
public:
    Camera();
    ~Camera();

public:
    //相机位置
    XMVECTOR GetPositionXM()const;
    XMFLOAT3 GetPosition()const;
    void SetPosition(float x, float y, float z);
    void SetPosition(const XMFLOAT3& v);

    //上、右、朝向向量
    XMVECTOR GetRightXM()const;
    XMFLOAT3 GetRight()const;
    XMVECTOR GetUpXM()const;
    XMFLOAT3 GetUp()const;
    XMVECTOR GetLookXM()const;
    XMFLOAT3 GetLook()const;

    //裁剪面
    float GetNearZ()const;
    float GetFarZ()const;
    float GetAspect()const;
    float GetFovY()const;
    float GetFovX()const;
    float GetNearWindowWidth()const;
    float GetNearWindowHeight()const;
    float GetFarWindowWidth()const;
    float GetFarWindowHeight()const;
    
    //设置透视
    void SetLens(float fovY, float aspect, float zn, float zf);

    //看向
    void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
    void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

    //平移
    void Strafe(float d);
    //向前
    void Walk(float d);

    //旋转
    void Pitch(float angle);
    void RotateY(float angle);

    //更新矩阵
    void UpdateViewMatrix();

    //变换矩阵
    XMMATRIX View()const;
    XMMATRIX Proj()const;
    XMMATRIX ViewProj()const;


private:
    //相机位置
    XMFLOAT3 m_Position;
    //相机上向量
    XMFLOAT3 m_Up;
    //相机右向量
    XMFLOAT3 m_Right;
    //相机朝向向量
    XMFLOAT3 m_Look;

    //近裁剪面距离
    float m_NearZ;
    //远裁剪面距离
    float m_FarZ;
    //相机开口
    float m_FovY;
    //宽高比
    float m_Aspect;
    //近裁剪面高度
    float m_NearWindowHeight;
    //远裁剪面高度
    float m_FarWindowHeight;

    //相机矩阵
    XMFLOAT4X4 m_View;
    //透视矩阵
    XMFLOAT4X4 m_Proj;
};
#endif