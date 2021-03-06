//
// Created by admin on 2020/8/4.
//

#include "BindCGAL.h"

// ----------------- Data exchange with glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLM_Matrix_Helper.h"
#include "GLM_Vec_Helper.h"
#include "CartesianLog.h"


#define CGAL_NO_GMP 1

#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

// define base mesh
typedef CGAL::Simple_cartesian<double> CGAL_K;
typedef CGAL::Surface_mesh<CGAL_K::Point_3> CGAL_Mesh;

// define base vertex & face
typedef CGAL_Mesh::Vertex_index CGAL_vertex_descriptor;
typedef CGAL_Mesh::Face_index CGAL_face_descriptor;
typedef CGAL_Mesh::edge_index CGAL_edge_descriptor;
typedef CGAL_Mesh::halfedge_index CGAL_halfedge_descriptor;

// iterator
typedef CGAL_Mesh::face_iterator CGAL_face_iterator;
typedef CGAL_Mesh::vertex_iterator CGAL_vertex_iterator;


// ------------ add triangle by houdini------------------------
/*
int prim = addprim(geoself(),"poly");
addpoint(geoself(), set(0,1,0) );
addpoint(geoself(), set(1,1,0) );
addpoint(geoself(), set(0,0,0) );
addvertex(geoself(), prim, 0 );
addvertex(geoself(), prim, 1 );
addvertex(geoself(), prim, 2 );
 */





namespace Cartesian{

void BindCGAL::bind(sol::state *lua) {
    // bind vertex index
    sol::usertype<CGAL_vertex_descriptor> vertex= (*lua).new_usertype<CGAL_vertex_descriptor>(
            "vertex",
            sol::constructors<CGAL_vertex_descriptor(),CGAL_vertex_descriptor(int)>()
            );
    vertex["idx"] = &CGAL_vertex_descriptor::idx;


    // bind face index
    sol::usertype<CGAL_face_descriptor> face= (*lua).new_usertype<CGAL_face_descriptor>(
            "face",
            sol::constructors<CGAL_face_descriptor(),CGAL_face_descriptor(int)>()
    );
    face["idx"] = &CGAL_face_descriptor::idx;


    // bind half-edge index
    sol::usertype<CGAL_halfedge_descriptor> half_edge= (*lua).new_usertype<CGAL_halfedge_descriptor>(
            "half_edge",
            sol::constructors<CGAL_halfedge_descriptor(),CGAL_halfedge_descriptor(int)>()
    );
    half_edge["idx"] = &CGAL_halfedge_descriptor::idx;


    // bind edge index
    sol::usertype<CGAL_edge_descriptor> edge= (*lua).new_usertype<CGAL_edge_descriptor>(
            "edge",
            sol::constructors<CGAL_edge_descriptor(),CGAL_edge_descriptor(int),CGAL_edge_descriptor(CGAL_halfedge_descriptor)>()
    );
    edge["idx"] = &CGAL_edge_descriptor::idx;


    // bind mesh pointer
    sol::usertype<CGAL_Mesh> mesh= (*lua).new_usertype<CGAL_Mesh>(
            "mesh"
            );


    // todo addprim() need imp in DCC software,and return mesh handle.

    // mesh functions
    // addpoint(mesh, pt) : return vertex descriptor
    auto add_point = [](CGAL_Mesh & mesh, const glm::vec3 &ptpos){
        auto vd = mesh.add_vertex(CGAL_K::Point_3(ptpos.x, ptpos.y, ptpos.z));
        return vd;
    };
    auto add_point_table = [](CGAL_Mesh & mesh, const sol::lua_table & table){
        if(table.size() != 3){
            CARTESIAN_CORE_ERROR("addpoint(table) table length = 3");
            return CGAL_vertex_descriptor(-1);
        }
        auto ptpos = GLM_Vec_Helper::table_to_vec3(table);
        auto vd = mesh.add_vertex(CGAL_K::Point_3(ptpos.x, ptpos.y, ptpos.z));
        return vd;
    };
    lua->set_function("addpoint",sol::overload(add_point,add_point_table));

}

}
