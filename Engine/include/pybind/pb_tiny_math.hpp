#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include "TinyMath.hpp"

namespace py = pybind11;

namespace CSPill::Math {

void PB_TinyMath(py::module &m) {
  py::class_<Vec2D>(m, "Vec2D")
      .def(py::init<float, float, float>(), py::arg("x") = 0, py::arg("y") = 0,
           py::arg("w") = 0)
      .def("__getitem__", [](const Vec2D &v, int index) { return v[index]; })
      .def("__setitem__",
           [](Vec2D &v, int index, float value) { v[index] = value; })
      .def_readwrite("x", &Vec2D::x)
      .def_readwrite("y", &Vec2D::y)
      .def_readwrite("w", &Vec2D::w)
      .def(py::self *= float())
      .def(py::self /= float())
      .def(py::self += py::self)
      .def(py::self -= py::self)
      .def(py::self * py::self)
      .def(py::self * float())
      .def(py::self / float())
      .def("__eq__", &Vec2D::operator==)
      .def("magnitude", &Vec2D::Magnitude)
      .def("dot", &Vec2D::Dot)
      .def("normalize", &Vec2D::Normalize)
      .def("project", &Vec2D::Project)
      .def("cross_product", &Vec2D::CrossProduct)
      .def("__add__", &Vec2D::operator+)
      .def("__sub__", &Vec2D::operator-)
      .def("__str__", [](const Vec2D &v) {
        return "Vec2D(x: " + std::to_string(v.x) +
               ", y: " + std::to_string(v.y) + ", w: " + std::to_string(v.w) +
               ")";
      });
}
}  // namespace CSPill::Math