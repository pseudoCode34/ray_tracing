#include "intersection.hpp"
#include "solid_reorientable.hpp"
#include "vector.hpp"

#include <vector>

namespace raytracing::Imager {

class Torus : public SolidObject_Reorientable {
public:
	Torus(double r, double s);

protected:
	IntersectionList object_space_append_all_intersections(
		const Vector &vantage, const Vector &direction) const override;

	bool object_space_contains(const Vector &point) const override;

	std::vector<double> solve_intersections(const Vector &vantage,
											const Vector &direction) const;

	Vector surface_normal(const Vector &point) const;

private:
	const double R; // distance from center of hole to center of tube
	const double S; // distance from center of tube to outside of tube
};
} // namespace raytracing::Imager

