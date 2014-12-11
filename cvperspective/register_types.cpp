#include "register_types.h"
#include "object_type_db.h"
#include "OpenCVPerspective.h"

void register_cvperspective_types()
{
		ObjectTypeDB::register_type<OpenCVPerspective>();
}

void unregister_cvperspective_types() {
   //nothing to do here
}
