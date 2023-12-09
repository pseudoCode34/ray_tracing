#ifndef __DDC_IMAGER_PLANET_H
#define __DDC_IMAGER_PLANET_H

#include "set.hpp"
#include "solid_object.hpp"
#include "spheroid.hpp"

namespace raytracing::Imager {
class SolidObject;
struct Color;

constexpr double MEAN_EARTH_RADIUS_KM = 6371.0;

class Planet : public Spheroid {
public:
	Planet(const Color &color, double equatorial_radius_in_km,
		   double polar_radius_in_km);
};

class Saturn : public SetUnion {
public:
	Saturn();

private:
	static SolidObject *create_ring_system();
};
} // namespace raytracing::Imager

#endif // __DDC_IMAGER_PLANET_H
