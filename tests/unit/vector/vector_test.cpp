#include "vector_test.hpp"

#include <gtest/gtest-printers.h>
#include <limits>

namespace raytracing::Imager {

TEST_F(VectorTest, IsEmptyInitially) {
	constexpr Vector ZERO{};
	EXPECT_NE(vector1, ZERO);
	EXPECT_NE(vector2, ZERO);
	EXPECT_NE(vector3, ZERO);
	EXPECT_EQ(vector4, ZERO);
}

TEST_F(VectorTest, AddOperation) {
	EXPECT_EQ(vector1 + vector2, (Vector{-0.6, 2.57735027, 166.7944501}));
	EXPECT_EQ(vector2 + vector1, (Vector{-0.6, 2.57735027, 166.7944501}));
	EXPECT_EQ(vector2 + vector3,
			  (Vector{-0.50138771, -0.86980776, 163.7945009}));
	EXPECT_EQ(vector3 + vector4, vector3);
	EXPECT_EQ(vector4 + vector1, vector1);
}

TEST_F(VectorTest, SubtractionOperation) {
	EXPECT_EQ(vector1 - vector2, (Vector{2.6, 1.42264973, -160.7944501}));
	EXPECT_EQ(vector2 - vector3,
			  (Vector{-2.69861229, 2.0245083, 163.79439929}));
	EXPECT_EQ(vector3 - vector4, vector3);
	EXPECT_EQ(vector4 - vector1, -vector1);
}

TEST_F(VectorTest, Negate) {
	EXPECT_EQ(-vector1, (Vector{-1, -2, -3}));
	EXPECT_EQ(-vector2, (Vector{1.6, -0.57735027, -163.7944501}));
	EXPECT_EQ(-vector3,
			  (Vector{-1.09861229e+00, 1.44715803e+00, -5.08052634e-05}));
	EXPECT_EQ(-vector4, vector4);
}

TEST_F(VectorTest, ZeroMultiply) {
	constexpr Vector ZERO{};
	EXPECT_EQ(vector1 * 0, ZERO);
	EXPECT_EQ(0 * vector2, ZERO);
	EXPECT_EQ(vector3 * 0, ZERO);
	EXPECT_EQ(0 * vector4, ZERO);
}

TEST_F(VectorTest, IntScalarMultiply) {
	EXPECT_EQ(vector1 * 3, (Vector{3, 6, 9}));
	EXPECT_EQ(vector2 * -4, (Vector{6.4, -2.30940108, -655.1778004}));
	EXPECT_EQ(vector3 * -9,
			  (Vector{-9.88751060e+00, 1.30244223e+01, -4.57247371e-04}));
	EXPECT_EQ(vector4 * 10, (Vector{}));
}

TEST_F(VectorTest, FloatingPointScalarMultiply) {
	EXPECT_EQ(vector1 * log2(3), (Vector{1.5849625, 3.169925, 4.7548875}));
	EXPECT_EQ(vector2 * (-1 / sqrt(8)),
			  (Vector{0.56568542, -0.20412415, -57.91008319}));
	EXPECT_EQ(vector3 * -exp(4),
			  (Vector{-5.99821986e+01, 7.90121513e+01, -2.77387339e-03}));
	EXPECT_EQ(vector4 * (1. / 3), (Vector{}));
}

TEST_F(VectorTest, ScalarDivision) {
	double scalar = cbrt(-3);

	EXPECT_EQ(vector1 / scalar,
			  (Vector{-0.69336127, -1.38672255, -2.08008382}));
	EXPECT_EQ(vector2 / scalar,
			  (Vector{1.10937804, -0.40031232, -113.56872865}));
	EXPECT_EQ(vector3 / scalar,
			  (Vector{-7.61735216e-01, 1.00340334e+00, -3.52264022e-05}));
	EXPECT_EQ(vector4 / scalar, (Vector{}));
}

TEST_F(VectorTest, Magnitude) {
	EXPECT_EQ(vector1.magnitude(), 3.7416573867739413);
	EXPECT_EQ(vector2.magnitude(), 163.80328206940447);
	EXPECT_EQ(vector3.magnitude(), 1.8169246905339924);
	EXPECT_EQ(vector4.magnitude(), 0);
}

TEST_F(VectorTest, Normalize) {
	auto nan = std::numeric_limits<double>::quiet_NaN();
	EXPECT_EQ(vector1.unit_vector(),
			  (Vector{0.26726124, 0.53452248, 0.80178373}));
	EXPECT_EQ(vector2.unit_vector(),
			  (Vector{-0.00976781, 0.00352466, 0.99994608}));
	EXPECT_EQ(vector3.unit_vector(),
			  (Vector{6.04654829e-01, -7.96487625e-01, 2.79622285e-05}));
	EXPECT_EQ(vector4.unit_vector(), (Vector{nan, nan, nan}));
}

TEST_F(VectorTest, DotProduct) {
	EXPECT_DOUBLE_EQ(vector1.dot(vector2), 490.9380508366774);
	EXPECT_DOUBLE_EQ(vector2.dot(vector1), 490.9380508366774);

	EXPECT_DOUBLE_EQ(vector2.dot(vector3), -2.584975120639433);
	EXPECT_DOUBLE_EQ(vector3.dot(vector2), -2.584975120639433);

	EXPECT_DOUBLE_EQ(vector3.dot(vector4), 0);

	EXPECT_DOUBLE_EQ(vector4.dot(vector1), 0);
}

TEST_F(VectorTest, CrossProduct) {
	EXPECT_EQ(vector1.cross(vector2),
			  (Vector{325.85684939, -168.5944501, 3.77735027}));
	EXPECT_EQ(vector2.cross(vector1),
			  (Vector{-325.85684939, 168.5944501, -3.77735027}));

	EXPECT_EQ(vector2.cross(vector3),
			  (Vector{237.03648328, 179.94667698, 1.68116875}));
	EXPECT_EQ(vector3.cross(vector2),
			  (Vector{-237.03648328, -179.94667698, -1.68116875}));

	EXPECT_EQ(vector3.cross(vector4), (Vector{}));
	EXPECT_EQ(vector4.cross(vector1), (Vector{}));
}
} // namespace raytracing::Imager
