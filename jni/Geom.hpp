/*
	This file is owned by Murtaza Alexandru and may not be distributed, edited or used without written permission of the owner
	When using this work you accept to keep this header
	E-mails from murtaza_alexandru73@yahoo.com with permissions can be seen as valid.
*/



#ifndef GEOM_HPP
#define GEOM_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"

namespace Math
{
	template<class T> class Vector2;
	template<class T> class Vector3;

	template<class T> class Geom
	{
	public:
		static void CalculateTangentBitangent(const Vector3<T> &P1, const Vector3<T> &P2, const Vector3<T> &P3, const Vector2<T> &UV1, const Vector2<T> &UV2, const Vector2<T> &UV3, Vector3<T> &tangent, Vector3<T> &bitangent );

		static bool SphereIntersectSphere(const Vector3<T> &p1, const T &radius1, const Vector3<T> &p2, const T &radius2);

		static T TriangleDet2D(const Vector2<T> &A, const Vector2<T> &B, const Vector2<T> &C);

		static void LineCoeff2D(const Vector2<T> A, const Vector2<T> B, T &a, T &b, T &c);

		static bool LineIntersect(const Vector2<T> A1, const Vector2<T> B1, const Vector2<T> A2, const Vector2<T> B2, Vector2<T> &I);
	};

	template<class T> inline void Geom<T>::CalculateTangentBitangent(const Vector3<T> &P1, const Vector3<T> &P2, const Vector3<T> &P3, const Vector2<T> &UV1, const Vector2<T> &UV2, const Vector2<T> &UV3, Vector3<T> &tangent, Vector3<T> &bitangent )
	{
		Vector3<T> Edge1 = P2 - P1;
		Vector3<T> Edge2 = P3 - P1;
		Vector2<T> Edge1uv = UV2 - UV1;
		Vector2<T> Edge2uv = UV3 - UV1;
	
		T cp = Edge1uv.y * Edge2uv.x - Edge1uv.x * Edge2uv.y;

		if ( cp != static_cast<T>(0) )
		{
			T mul = 1.0f / cp;
			tangent   = (Edge1 * -Edge2uv.y + Edge2 * Edge1uv.y) * mul;
			bitangent = (Edge1 * -Edge2uv.x + Edge2 * Edge1uv.x) * mul;
	
			tangent.Normalize();
			bitangent.Normalize();
		}
	}
	
	template<class T> inline bool Geom<T>::SphereIntersectSphere(const Vector3<T> &p1, const T &radius1, const Vector3<T> &p2, const T &radius2)
	{
		return p1.Distance(p2)<=radius1+radius2;
	}

	template<class T> inline T Geom<T>::TriangleDet2D(const Vector2<T> &A, const Vector2<T> &B, const Vector2<T> &C)
	{
		return (B.x-A.x)*(C.y-A.y)-(B.y-A.y)*(C.x-A.x);
	}

	template<class T> inline void Geom<T>::LineCoeff2D(const Vector2<T> A, const Vector2<T> B, T &a, T &b, T &c)
	{
		a=B.y-A.y;
		b=A.x-B.x;
		c=A.y*B.x-A.x*B.y;
	}

	template<class T> inline bool Geom<T>::LineIntersect(const Vector2<T> A1, const Vector2<T> B1, const Vector2<T> A2, const Vector2<T> B2, Vector2<T> &I)
	{
		if(Misc<T>::Sign(TriangleDet2D(A1,B1,A2))==Misc<T>::Sign(TriangleDet2D(A1,B1,B2)))
		{
			return false;
		}
		if(Misc<T>::Sign(TriangleDet2D(A2,B2,A1))==Misc<T>::Sign(TriangleDet2D(A2,B2,B1)))
		{
			return false;
		}
		if(!AABB2D(A1,B1).Intersect(AABB2D(A2,B2)))
		{
			return false;
		}
		T a1,b1,c1;
		LineCoeff2D(A1,B1,a1,b1,c1);
		T a2,b2,c2;
		LineCoeff2D(A2,B2,a2,b2,c2);
		I.x=(b1*c2-b2*c1)/(a1*b2-a2*b1);
		I.y=(c1*a2-c2*a1)/(a1*b2-a2*b1);
		return true;
	}
}

#endif
