// Sphere.hpp
// KlayGE 边框球体 头文件
// Ver 2.1.1
// 版权所有(C) 龚敏敏, 2004
// Homepage: http://klayge.sourceforge.net
//
// 2.1.1
// 初次建立 (2004.4.30)
//
// 修改记录
///////////////////////////////////////////////////////////////////////////////

#ifndef _SPHERE_HPP
#define _SPHERE_HPP

#include <boost/operators.hpp>

#include <KlayGE/Bound.hpp>

namespace KlayGE
{
	class Sphere : boost::addable2<Sphere, Vector3, 
						boost::subtractable2<Sphere, Vector3,
						boost::andable<Sphere,
						boost::orable<Sphere > > > >,
				public Bound
	{
	public:
		Sphere()
		{
		}
		Sphere(const Vector3& center, float radius)
			: center_(center),
				radius_(radius)
		{
		}

		// 赋值操作符
		Sphere& operator+=(const Vector3& rhs)
		{
			this->Center() += rhs;
			return *this;
		}
		Sphere& operator-=(const Vector3& rhs)
		{
			this->Center() -= rhs;
			return *this;
		}
		Sphere& operator*=(float rhs)
		{
			this->Radius() *= rhs;
			return *this;
		}
		Sphere& operator/=(float rhs)
		{
			return operator*=(1.0f / rhs);
		}

		Sphere& operator=(const Sphere& rhs)
		{
			if (this != &rhs)
			{
				this->Center() = rhs.Center();
				this->Radius() = rhs.Radius();
			}
			return *this;
		}

		// 一元操作符
		const Sphere& operator+() const
		{
			return *this;
		}
		const Sphere& operator-() const
		{
			return *this;
		}

		// 属性
		Vector3& Center()
		{
			return center_;
		}
		const Vector3& Center() const
		{
			return center_;
		}
		float& Radius()
		{
			return radius_;
		}
		const float& Radius() const
		{
			return radius_;
		}

		bool IsEmpty() const
		{
			return MathLib::Eq(radius_, 0.0f);
		}

		bool VecInBound(const Vector3& v) const
		{
			return MathLib::VecInSphere(*this, v);
		}
		float MaxRadiusSq() const
		{
			return this->Radius() * this->Radius();
		}

	private:
		Vector3 center_;
		float radius_;
	};

	inline bool
	operator==(const Sphere& lhs, const Sphere& rhs)
	{
		return (lhs.Center() == rhs.Center()) && (lhs.Radius() == rhs.Radius());
	}
	inline bool
	operator!=(const Sphere& lhs, const Sphere& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif			// _SPHERE_HPP
