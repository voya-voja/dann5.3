#pragma once

#include <pybind11/pybind11.h>

#include <utility>
#include <Eigen/Dense>
#include <iostream>
#include <Eigen/Core>

using namespace Eigen;

namespace dann5 {


/*		template<typename _Scalar, int _Rows, int _Cols, int _Options = 0, int _MaxRows = Dynamic, int _MaxCols = Dynamic>
	Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> rotate(const Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& right)
	{
		Index rcI;
		Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> rotatedMatrix(right.cols(), right.rows());
		for (Index rIndex = 0; rIndex < right.rows(); rIndex++)
		{
			for (Index cIndex = 0; cIndex < right.cols(); cIndex++)
			{
				rcI = right.cols() - cIndex - 1;
				rotatedMatrix(rIndex, cIndex) = right(rIndex, rcI);
			}
		}
		return(rotatedMatrix);
	};
*/
	template <bool COND, int A, int B>
	struct IF
	{
		enum { val = A };
	};

	template <int A, int B>
	struct IF<false, A, B>
	{
		enum { val = B };
	};

	template <int A, int B>
	struct MIN : IF < A < B, A, B>
	{
	};

	template <typename T, int dim, int newDim>
	Eigen::Matrix<T, newDim, 1> to(Eigen::Matrix<T, dim, 1> p)
	{
		Eigen::Matrix<int, newDim, 1> newp =
			Eigen::Matrix<T, newDim, 1>::Zero();

		newp.template head< MIN<dim, newDim>::val >() =
			p.template head< MIN<dim, newDim>::val >();

		return newp;
	}

	inline bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
		if (from.empty())
			return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	struct group_thousands : std::numpunct<char>
	{
		std::string do_grouping() const override { return "\3"; }
	};

};