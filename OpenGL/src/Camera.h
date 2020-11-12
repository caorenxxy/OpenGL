#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Def.h"
#include "math/Lingal.h"
#include "math/Transform.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

namespace OpenGL
{
	using namespace Math;
	class Camera 
	{
		//using Vec3 = glm::vec3;
		//using Matrix4 = glm::mat4;
	private:
		float mFovy;
		float mDistanceToFilm;
		float mNear, mFar;
		int mWidth, mHeight;
		float mR, mTheta, mPhi;
		Vec3 mPosition, mLookAt, mUp;
		Vec3 mCx, mCy, mCz;
		Matrix4 mViewMatrix;
		Matrix4 mProjMatrix;

		float mMoveSpeed, mScaleFactor;
		float mMaxR, mMinR;
	public:
		void Init(
			const Vec3& position,
			const Vec3& lookat,
			const Vec3& up,
			int width,
			int height,
			float fovy = 45.f,
			float disToFilm = 1.0f,
			float nClip = 0.01f,
			float fClip = 1000.0f
		)
		{
			mPosition = position;
			mLookAt = lookat;
			mWidth = width;
			mHeight = height;
			mFovy = fovy;
			mDistanceToFilm = disToFilm;
			mNear = nClip;
			mFar = fClip;
			mUp = up;

			mMoveSpeed = 2.5f;
			mScaleFactor = 2.0f;
			mMinR = 0.001f;
			mMaxR = 100000.f;

			ComputeSphereCoord();
			ComputePosition();
			ComputeViewMatrix();
			ComputeProjMatrix();
		}

		void Place(const Vec3& position)
		{
			mPosition = position;
			ComputeViewMatrix();
		}

		void Resize(int width, int height)
		{
			mWidth = width;
			mHeight = height;
			ComputeProjMatrix();
		}

		void Scale(float d)
		{
			mR = std::min(mMaxR, std::max(mMinR, mR + d * mScaleFactor));
			ComputePosition();
			ComputeViewMatrix();
		}

		void Move(float dx, float dy)
		{
			Vec3 moveDir = dx * mMoveSpeed * mCx + dy * mMoveSpeed * mCy;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void MoveX(float dx)
		{
			Vec3 moveDir = dx * mMoveSpeed * mCx;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void MoveY(float dy)
		{
			Vec3 moveDir = dy * mMoveSpeed * mCy;
			mPosition += moveDir;
			mLookAt += moveDir;
			ComputeViewMatrix();
		}

		void Rotate(float dPhi, float dTheta) 
		{
			mPhi += dPhi;
			/*mTheta = glm::clamp(mTheta + dTheta, 0.f, PI - EPS_F);*/
			mTheta = Clamp(mTheta + dTheta, 0.f, PI - EPS_F);
			ComputePosition();
			ComputeViewMatrix();
		}

		inline Matrix4 GetViewMatrix() const { return mViewMatrix; }

		inline Matrix4 GetProjMatrix() const { return mProjMatrix; }

	private:
		
		void ComputePosition()
		{
			float sinTheta = std::sin(mTheta);
			if (sinTheta == 0)
			{
				mTheta += EPS_F;
				sinTheta = std::sin(mTheta);
			}
			Vec3 dirToCam(
				mR * sinTheta * std::cos(mPhi),
				mR * std::cos(mTheta),
				mR * sinTheta * std::sin(mPhi));
			mPosition = mLookAt + dirToCam;
			//mCz = glm::normalize(mPosition - mLookAt); // -1 * dir in raytracer
			//mCx = glm::normalize(glm::cross(mUp, mCz));
			//mCy = glm::cross(mCz, mCx);

			mCz = -(mPosition - mLookAt).Norm(); // -1 * dir in raytracer
			mCx = Cross(mUp, mCz).Norm();
			mCy = Cross(mCz, mCx);
		}

		void ComputeSphereCoord()
		{
			/*mR = float((mPosition - mLookAt).length());*/
			mR = float((mPosition - mLookAt).Length());
			mPhi = std::atan2(mPosition.z, mPosition.x);
			mPhi = mPhi > 0 ? mPhi : 2.f * PI - mPhi;
			mTheta = std::acos(mPosition.y / mR);
		}

		void ComputeViewMatrix()
		{
			/*mViewMatrix = glm::lookAt(mPosition, mLookAt, mUp);*/
			mViewMatrix = Transform::LookAt(mPosition, mLookAt, mUp).GetMatrix();
		}

		void ComputeProjMatrix()
		{
			/*mProjMatrix = glm::perspective(mFovy, (float)mWidth / (float)mHeight, mNear, mFar);*/
			mProjMatrix = Transform::Perspective(mFovy, (float)mWidth / (float)mHeight, mDistanceToFilm, mNear, mFar).GetMatrix();
		}
	};
}