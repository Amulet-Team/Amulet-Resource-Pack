#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/typing.h>

#include <memory>

#include <amulet/pybind11_extensions/collections.hpp>

#include "block_mesh.hpp"

namespace py = pybind11;

void init_block_mesh(py::module m)
{
    // FloatVec2
    py::class_<Amulet::FloatVec2> FloatVec2(m, "FloatVec2",
        "A 2D floating point vector");
    FloatVec2.def(
        py::init<
            float,
            float>(),
        py::arg("x"),
        py::arg("y"));
    FloatVec2.def_readonly("x", &Amulet::FloatVec2::x);
    FloatVec2.def_readonly("y", &Amulet::FloatVec2::y);

    // FloatVec3
    py::class_<Amulet::FloatVec3> FloatVec3(m, "FloatVec3",
        "A 3D floating point vector");
    FloatVec3.def(
        py::init<
            float,
            float,
            float>(),
        py::arg("x"),
        py::arg("y"),
        py::arg("z"));
    FloatVec3.def_readonly("x", &Amulet::FloatVec3::x);
    FloatVec3.def_readonly("y", &Amulet::FloatVec3::y);
    FloatVec3.def_readonly("z", &Amulet::FloatVec3::z);

    // Vertex
    py::class_<Amulet::Vertex> Vertex(m, "Vertex",
        "Attributes for a single vertex.");
    Vertex.def(
        py::init<
            const Amulet::FloatVec3&,
            const Amulet::FloatVec2&,
            const Amulet::FloatVec3&>(),
        py::arg("coord"),
        py::arg("texture_coord"),
        py::arg("tint"));
    Vertex.def_readonly("coord", &Amulet::Vertex::coord, py::doc("The spatial coordinate of the vertex."));
    Vertex.def_readonly("texture_coord", &Amulet::Vertex::texture_coord, py::doc("The texture coordinate of the vertex."));
    Vertex.def_readonly("tint", &Amulet::Vertex::tint, py::doc("The tint colour for the vertex."));

    // Triangle
    py::class_<Amulet::Triangle> Triangle(m, "Triangle",
        "The vertex and texture indexes that make up a triangle.");
    Triangle.def(
        py::init<
            size_t,
            size_t,
            size_t,
            size_t>(),
        py::arg("vert_index_a"),
        py::arg("vert_index_b"),
        py::arg("vert_index_c"),
        py::arg("texture_index"));
    Triangle.def_readonly("vert_index_a", &Amulet::Triangle::vert_index_a);
    Triangle.def_readonly("vert_index_b", &Amulet::Triangle::vert_index_b);
    Triangle.def_readonly("vert_index_c", &Amulet::Triangle::vert_index_c);
    Triangle.def_readonly("texture_index", &Amulet::Triangle::texture_index);

    // BlockMeshPart
    py::class_<Amulet::BlockMeshPart> BlockMeshPart(m, "BlockMeshPart",
        "A part of a block mesh for one of the culling directions.");
    BlockMeshPart.def(
        py::init<
            const std::vector<Amulet::Vertex>&,
            const std::vector<Amulet::Triangle>&>(),
        py::arg("verts"),
        py::arg("triangles"));
    BlockMeshPart.def_readonly("verts", &Amulet::BlockMeshPart::verts, py::doc("The vertices in this block mesh part."));
    BlockMeshPart.def_readonly("triangles", &Amulet::BlockMeshPart::triangles, py::doc("The triangles in this block mesh part."));

    // BlockMeshTransparency
    py::enum_<Amulet::BlockMeshTransparency>(m, "BlockMeshTransparency",
        "The transparency of a block mesh.", py::arithmetic())
        .value("FullOpaque", Amulet::BlockMeshTransparency::FullOpaque, "A block that occupies the whole block and is opaque.")
        .value("FullTranslucent", Amulet::BlockMeshTransparency::FullTranslucent, "A block that occupies the whole block and has at least one translucent face.")
        .value("Partial", Amulet::BlockMeshTransparency::Partial, "A block that does not occupy the whole block.");

    // BlockMeshCullDirection
    py::enum_<Amulet::BlockMeshCullDirection>(m, "BlockMeshCullDirection",
        "The direction a mesh part is culled by. The value corresponds to the index in the mesh parts array.",
        py::arithmetic())
        .value("CullNone", Amulet::BlockMeshCullDirection::BlockMeshCullNone, "Is not culled by any neighbouring blocks.")
        .value("CullUp", Amulet::BlockMeshCullDirection::BlockMeshCullUp, "Is culled by an opaque block above.")
        .value("CullDown", Amulet::BlockMeshCullDirection::BlockMeshCullDown, "Is culled by an opaque block below.")
        .value("CullNorth", Amulet::BlockMeshCullDirection::BlockMeshCullNorth, "Is culled by an opaque block to the north.")
        .value("CullEast", Amulet::BlockMeshCullDirection::BlockMeshCullEast, "Is culled by an opaque block to the east.")
        .value("CullSouth", Amulet::BlockMeshCullDirection::BlockMeshCullSouth, "Is culled by an opaque block to the south.")
        .value("CullWest", Amulet::BlockMeshCullDirection::BlockMeshCullWest, "Is culled by an opaque block to the west.");

    // BlockMesh
    py::class_<Amulet::BlockMesh> BlockMesh(m, "BlockMesh", "All the data that makes up a block mesh.");
    BlockMesh.def(
        py::init(
            [](
                Amulet::BlockMeshTransparency transparency,
                std::vector<std::string> textures,
                py::typing::Tuple<
                    std::optional<Amulet::BlockMeshPart>,
                    std::optional<Amulet::BlockMeshPart>,
                    std::optional<Amulet::BlockMeshPart>,
                    std::optional<Amulet::BlockMeshPart>,
                    std::optional<Amulet::BlockMeshPart>,
                    std::optional<Amulet::BlockMeshPart>,
                    std::optional<Amulet::BlockMeshPart>>
                    py_parts) {
                auto parts = py_parts.cast<std::array<std::optional<Amulet::BlockMeshPart>, 7>>();
                return Amulet::BlockMesh(
                    transparency,
                    textures,
                    parts);
            }),
        py::arg("transparency"),
        py::arg("textures"),
        py::arg("parts"));
    BlockMesh.def_readonly("transparency", &Amulet::BlockMesh::transparency, py::doc("The transparency state of this block mesh."));
    BlockMesh.def_readonly("textures", &Amulet::BlockMesh::textures, py::doc("The texture paths used in this block mesh. The Triangle's texture_index attribute is an index into this list."));
    BlockMesh.def_property_readonly(
        "parts",
        [](const Amulet::BlockMesh& self) -> py::typing::Tuple<
                                              std::optional<Amulet::BlockMeshPart>,
                                              std::optional<Amulet::BlockMeshPart>,
                                              std::optional<Amulet::BlockMeshPart>,
                                              std::optional<Amulet::BlockMeshPart>,
                                              std::optional<Amulet::BlockMeshPart>,
                                              std::optional<Amulet::BlockMeshPart>,
                                              std::optional<Amulet::BlockMeshPart>> {
            return py::make_tuple(
                py::cast(self.parts[0]),
                py::cast(self.parts[1]),
                py::cast(self.parts[2]),
                py::cast(self.parts[3]),
                py::cast(self.parts[4]),
                py::cast(self.parts[5]),
                py::cast(self.parts[6]));
        },
        py::doc("The mesh parts that make up this mesh. The index corresponds to the value of BlockMeshCullDirection."));
    BlockMesh.def("rotate", &Amulet::BlockMesh::rotate, py::arg("rotx"), py::arg("roty"), py::doc("Rotate the mesh in the x and y axis. Accepted values are -3 to 3 which correspond to 90 degree rotations."));

    m.def(
        "merge_block_meshes", [](Amulet::pybind11_extensions::collections::Sequence<Amulet::BlockMesh> py_meshes) {
            std::vector<std::reference_wrapper<const Amulet::BlockMesh>> meshes;
            for (auto mesh : py_meshes) {
                meshes.push_back(mesh);
            }
            return Amulet::merge_block_meshes(meshes);
        },
        py::arg("meshes"), py::doc("Merge multiple block mesh objects into one block mesh."));

    m.attr("CUBE_FACE_LUT") = py::module::import("amulet.resource_pack.mesh.block._cube").attr("CUBE_FACE_LUT");
    m.attr("TRI_FACE") = py::module::import("amulet.resource_pack.mesh.block._cube").attr("TRI_FACE");
    m.attr("UV_ROTATION_LUT") = py::module::import("amulet.resource_pack.mesh.block._cube").attr("UV_ROTATION_LUT");
    m.attr("get_cube") = py::module::import("amulet.resource_pack.mesh.block._cube").attr("get_cube");
    m.attr("get_unit_cube") = py::module::import("amulet.resource_pack.mesh.block._cube").attr("get_unit_cube");

    py::dict face_keys;
    face_keys[py::none()] = Amulet::BlockMeshCullDirection::BlockMeshCullNone;
    face_keys[py::str("up")] = Amulet::BlockMeshCullDirection::BlockMeshCullUp;
    face_keys[py::str("down")] = Amulet::BlockMeshCullDirection::BlockMeshCullDown;
    face_keys[py::str("north")] = Amulet::BlockMeshCullDirection::BlockMeshCullNorth;
    face_keys[py::str("east")] = Amulet::BlockMeshCullDirection::BlockMeshCullEast;
    face_keys[py::str("south")] = Amulet::BlockMeshCullDirection::BlockMeshCullSouth;
    face_keys[py::str("west")] = Amulet::BlockMeshCullDirection::BlockMeshCullWest;
    m.attr("FACE_KEYS") = face_keys;
}
