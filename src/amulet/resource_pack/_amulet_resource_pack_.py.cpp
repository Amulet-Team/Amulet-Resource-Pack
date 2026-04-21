#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_mesh(py::module);
void init_image(py::module);

void init_amulet_resource_pack(py::module m)
{
    init_mesh(m);
    init_image(m);
}
