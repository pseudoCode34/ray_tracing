#ifndef TRANSLATABLE_HPP
#define TRANSLATABLE_HPP

#include "spatial/primitive.hpp"

namespace raytracing {

class Translatable {
public:
	virtual ~Translatable() = default;
	/**
	 * Translate the object vector by \param displacement
	 */
	virtual void translate_by(Vector3fConstRef displacement) = 0;

	/**
	 * \return The object being translated \param displacement
	 */
	/*[[nodiscard]] virtual Translatable
	translated_by(Vector3fConstRef displacement) const
		= 0;*/
};
} // namespace raytracing
#endif /*  */
