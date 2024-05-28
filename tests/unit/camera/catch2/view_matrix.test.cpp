#include "camera.hpp"
#include "vector3f.hpp"

#include <catch2/catch_test_macros.hpp>

namespace raytracing {
SCENARIO("Camera member function update_view_matrix() test",
		 "[Camera][glm::lookAt]") {
	GIVEN("3 world orthonormal basis vectors") {
		Camera under_test;
		under_test.set_position(Vector3f{1, 2, 3});
		under_test.set_view_center(Vector3f{-3, 4, 1});
		under_test.set_world_up(Vector3f::UnitY());

		// clang-format on
		WHEN("update_view_matrix() is called()") {
			under_test.update_view_matrix();
			Eigen::Affine3f actual = under_test.get_view_matrix();
			THEN("My invert viewMatrix is an orthogonal matrix") {
				REQUIRE(actual.linear().isUnitary());

				// FIXME: what is the post-condition of translation part
			}
		}
	}
}
} // namespace raytracing
