#include "planet.hpp"

#include "color.hpp"
#include "imager.hpp"
#include "thinring.hpp"
#include "vector.hpp"

#include <memory>
#include <utility>

namespace raytracing {
namespace Imager {

Planet::Planet(const Color &color, double equatorial_radius_in_km,
			   double polar_radius_in_km)
	: Spheroid(equatorial_radius_in_km / MEAN_EARTH_RADIUS_KM,
			   equatorial_radius_in_km / MEAN_EARTH_RADIUS_KM,
			   polar_radius_in_km / MEAN_EARTH_RADIUS_KM) {
	set_full_matte(color);
}

Saturn::Saturn()
	: SetUnion(Vector{},
			   new Planet(Color(255, 255, 153, 0.335), 60268.0, 54364.0),
			   create_ring_system()) {}

/*static*/ SolidObject *Saturn::create_ring_system() {
	struct RingData {
		double inner_radius_km;
		double outer_radius_km;
		double red;
		double green;
		double blue;
	};

	static const RingData RING_DATA[]
		= {{92154.0, 117733.0, 196.0, 194.0, 180.0},
		   {122405.0, 133501.0, 157.0, 160.0, 158.0},
		   {134085.0, 136888.0, 136.0, 140.0, 142.0}};

	SolidObject *ring_system;

	for (const auto [inner_radius_km, outer_radius_km, red, green, blue] :
		 RING_DATA) {
		Color color = Color(red, green, blue) / PIXEL_8BIT_MAX_COLOR;

		auto *ring_solid = new ThinRing(inner_radius_km / MEAN_EARTH_RADIUS_KM,
										outer_radius_km / MEAN_EARTH_RADIUS_KM);
		ring_solid->set_full_matte(color);

		if (ring_system)
			ring_system = new SetUnion(Vector{}, ring_solid, ring_system);
		else ring_system = ring_solid;
	}

	return ring_system;
}
} // namespace Imager
} // namespace raytracing
