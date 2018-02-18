/*
This file is part of pyOCCT which provides Python bindings to the OpenCASCADE
geometry kernel.

Copyright (C) 2016-2018  Laughlin Research, LLC (info@laughlinresearch.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <pyOCCT_Common.hpp>

#include <Geom2dHatch_Intersector.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <TopAbs_Orientation.hxx>
#include <Standard_Handle.hxx>
#include <Geom2d_Curve.hxx>
#include <HatchGen_PointOnHatching.hxx>
#include <HatchGen_ErrorStatus.hxx>
#include <HatchGen_Domain.hxx>
#include <Geom2dHatch_Hatcher.hxx>
#include <Geom2dInt_GInter.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Dir2d.hxx>
#include <TopAbs_State.hxx>
#include <Geom2dHatch_FClass2dOfClassifier.hxx>
#include <Geom2dHatch_Elements.hxx>
#include <Geom2dHatch_Element.hxx>
#include <gp_Pnt2d.hxx>
#include <IntRes2d_Position.hxx>
#include <Geom2dHatch_Classifier.hxx>
#include <NCollection_BaseMap.hxx>
#include <NCollection_BaseAllocator.hxx>
#include <NCollection_DataMap.hxx>
#include <Geom2dHatch_MapOfElements.hxx>
#include <Geom2dHatch_Hatching.hxx>
#include <Geom2dHatch_Hatchings.hxx>
#include <NCollection_Templates.hpp>

PYBIND11_MODULE(Geom2dHatch, mod) {

	// IMPORT
	py::module::import("OCCT.Standard");
	py::module::import("OCCT.Geom2dAdaptor");
	py::module::import("OCCT.TopAbs");
	py::module::import("OCCT.Geom2d");
	py::module::import("OCCT.HatchGen");
	py::module::import("OCCT.Geom2dInt");
	py::module::import("OCCT.gp");
	py::module::import("OCCT.IntRes2d");
	py::module::import("OCCT.NCollection");

	py::module other_mod;

	// IMPORT GUARDS

	// ENUMS

	// FUNCTIONS

	// CLASSES
	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Hatcher.hxx
	py::class_<Geom2dHatch_Hatcher, std::unique_ptr<Geom2dHatch_Hatcher, Deleter<Geom2dHatch_Hatcher>>> cls_Geom2dHatch_Hatcher(mod, "Geom2dHatch_Hatcher", "None");
	cls_Geom2dHatch_Hatcher.def(py::init<const Geom2dHatch_Intersector &, const Standard_Real, const Standard_Real>(), py::arg("Intersector"), py::arg("Confusion2d"), py::arg("Confusion3d"));
	cls_Geom2dHatch_Hatcher.def(py::init<const Geom2dHatch_Intersector &, const Standard_Real, const Standard_Real, const Standard_Boolean>(), py::arg("Intersector"), py::arg("Confusion2d"), py::arg("Confusion3d"), py::arg("KeepPnt"));
	cls_Geom2dHatch_Hatcher.def(py::init<const Geom2dHatch_Intersector &, const Standard_Real, const Standard_Real, const Standard_Boolean, const Standard_Boolean>(), py::arg("Intersector"), py::arg("Confusion2d"), py::arg("Confusion3d"), py::arg("KeepPnt"), py::arg("KeepSeg"));
	cls_Geom2dHatch_Hatcher.def("Intersector", (void (Geom2dHatch_Hatcher::*)(const Geom2dHatch_Intersector &)) &Geom2dHatch_Hatcher::Intersector, "Sets the associated intersector.", py::arg("Intersector"));
	cls_Geom2dHatch_Hatcher.def("Intersector", (const Geom2dHatch_Intersector & (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::Intersector, "Returns the associated intersector.");
	cls_Geom2dHatch_Hatcher.def("ChangeIntersector", (Geom2dHatch_Intersector & (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::ChangeIntersector, "Returns the associated intersector.");
	cls_Geom2dHatch_Hatcher.def("Confusion2d", (void (Geom2dHatch_Hatcher::*)(const Standard_Real)) &Geom2dHatch_Hatcher::Confusion2d, "Sets the confusion tolerance.", py::arg("Confusion"));
	cls_Geom2dHatch_Hatcher.def("Confusion2d", (Standard_Real (Geom2dHatch_Hatcher::*)() const ) &Geom2dHatch_Hatcher::Confusion2d, "Returns the 2d confusion tolerance, i.e. the value under which two points are considered identical in the parametric space of the hatching.");
	cls_Geom2dHatch_Hatcher.def("Confusion3d", (void (Geom2dHatch_Hatcher::*)(const Standard_Real)) &Geom2dHatch_Hatcher::Confusion3d, "Sets the confusion tolerance.", py::arg("Confusion"));
	cls_Geom2dHatch_Hatcher.def("Confusion3d", (Standard_Real (Geom2dHatch_Hatcher::*)() const ) &Geom2dHatch_Hatcher::Confusion3d, "Returns the 3d confusion tolerance, i.e. the value under which two points are considered identical in the 3d space of the hatching.");
	cls_Geom2dHatch_Hatcher.def("KeepPoints", (void (Geom2dHatch_Hatcher::*)(const Standard_Boolean)) &Geom2dHatch_Hatcher::KeepPoints, "Sets the above flag.", py::arg("Keep"));
	cls_Geom2dHatch_Hatcher.def("KeepPoints", (Standard_Boolean (Geom2dHatch_Hatcher::*)() const ) &Geom2dHatch_Hatcher::KeepPoints, "Returns the flag about the points consideration.");
	cls_Geom2dHatch_Hatcher.def("KeepSegments", (void (Geom2dHatch_Hatcher::*)(const Standard_Boolean)) &Geom2dHatch_Hatcher::KeepSegments, "Sets the above flag.", py::arg("Keep"));
	cls_Geom2dHatch_Hatcher.def("KeepSegments", (Standard_Boolean (Geom2dHatch_Hatcher::*)() const ) &Geom2dHatch_Hatcher::KeepSegments, "Returns the flag about the segments consideration.");
	cls_Geom2dHatch_Hatcher.def("Clear", (void (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::Clear, "Removes all the hatchings and all the elements.");
	cls_Geom2dHatch_Hatcher.def("ElementCurve", (const Geom2dAdaptor_Curve & (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::ElementCurve, "Returns the curve associated to the IndE-th element.", py::arg("IndE"));
	cls_Geom2dHatch_Hatcher.def("AddElement", [](Geom2dHatch_Hatcher &self, const Geom2dAdaptor_Curve & a0) -> Standard_Integer { return self.AddElement(a0); }, py::arg("Curve"));
	cls_Geom2dHatch_Hatcher.def("AddElement", (Standard_Integer (Geom2dHatch_Hatcher::*)(const Geom2dAdaptor_Curve &, const TopAbs_Orientation)) &Geom2dHatch_Hatcher::AddElement, "Adds an element to the hatcher and returns its index.", py::arg("Curve"), py::arg("Orientation"));
	cls_Geom2dHatch_Hatcher.def("AddElement", [](Geom2dHatch_Hatcher &self, const opencascade::handle<Geom2d_Curve> & a0) -> Standard_Integer { return self.AddElement(a0); }, py::arg("Curve"));
	cls_Geom2dHatch_Hatcher.def("AddElement", (Standard_Integer (Geom2dHatch_Hatcher::*)(const opencascade::handle<Geom2d_Curve> &, const TopAbs_Orientation)) &Geom2dHatch_Hatcher::AddElement, "Adds an element to the hatcher and returns its index.", py::arg("Curve"), py::arg("Orientation"));
	cls_Geom2dHatch_Hatcher.def("RemElement", (void (Geom2dHatch_Hatcher::*)(const Standard_Integer)) &Geom2dHatch_Hatcher::RemElement, "Removes the IndE-th element from the hatcher.", py::arg("IndE"));
	cls_Geom2dHatch_Hatcher.def("ClrElements", (void (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::ClrElements, "Removes all the elements from the hatcher.");
	cls_Geom2dHatch_Hatcher.def("HatchingCurve", (const Geom2dAdaptor_Curve & (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::HatchingCurve, "Returns the curve associated to the IndH-th hatching.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("AddHatching", (Standard_Integer (Geom2dHatch_Hatcher::*)(const Geom2dAdaptor_Curve &)) &Geom2dHatch_Hatcher::AddHatching, "Adds a hatching to the hatcher and returns its index.", py::arg("Curve"));
	cls_Geom2dHatch_Hatcher.def("RemHatching", (void (Geom2dHatch_Hatcher::*)(const Standard_Integer)) &Geom2dHatch_Hatcher::RemHatching, "Removes the IndH-th hatching from the hatcher.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("ClrHatchings", (void (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::ClrHatchings, "Removes all the hatchings from the hatcher.");
	cls_Geom2dHatch_Hatcher.def("NbPoints", (Standard_Integer (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::NbPoints, "Returns the number of intersection points of the IndH-th hatching.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("Point", (const HatchGen_PointOnHatching & (Geom2dHatch_Hatcher::*)(const Standard_Integer, const Standard_Integer) const ) &Geom2dHatch_Hatcher::Point, "Returns the IndP-th intersection point of the IndH-th hatching.", py::arg("IndH"), py::arg("IndP"));
	cls_Geom2dHatch_Hatcher.def("Trim", (void (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::Trim, "Trims all the hatchings of the hatcher by all the elements of the hatcher.");
	cls_Geom2dHatch_Hatcher.def("Trim", (Standard_Integer (Geom2dHatch_Hatcher::*)(const Geom2dAdaptor_Curve &)) &Geom2dHatch_Hatcher::Trim, "Adds a hatching to the hatcher and trims it by the elements already given and returns its index.", py::arg("Curve"));
	cls_Geom2dHatch_Hatcher.def("Trim", (void (Geom2dHatch_Hatcher::*)(const Standard_Integer)) &Geom2dHatch_Hatcher::Trim, "Trims the IndH-th hatching by the elements already given.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("ComputeDomains", (void (Geom2dHatch_Hatcher::*)()) &Geom2dHatch_Hatcher::ComputeDomains, "Computes the domains of all the hatchings.");
	cls_Geom2dHatch_Hatcher.def("ComputeDomains", (void (Geom2dHatch_Hatcher::*)(const Standard_Integer)) &Geom2dHatch_Hatcher::ComputeDomains, "Computes the domains of the IndH-th hatching.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("TrimDone", (Standard_Boolean (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::TrimDone, "Returns the fact that the intersections were computed for the IndH-th hatching.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("TrimFailed", (Standard_Boolean (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::TrimFailed, "Returns the fact that the intersections failed for the IndH-th hatching.", py::arg("IndH"));
	// cls_Geom2dHatch_Hatcher.def("IsDone", (Standard_Boolean (Geom2dHatch_Hatcher::*)() const ) &Geom2dHatch_Hatcher::IsDone, "Returns the fact that the domains were computed for all the hatchings.");
	cls_Geom2dHatch_Hatcher.def("IsDone", (Standard_Boolean (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::IsDone, "Returns the fact that the domains were computed for the IndH-th hatching.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("Status", (HatchGen_ErrorStatus (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::Status, "Returns the status about the IndH-th hatching.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("NbDomains", (Standard_Integer (Geom2dHatch_Hatcher::*)(const Standard_Integer) const ) &Geom2dHatch_Hatcher::NbDomains, "Returns the number of domains of the IndH-th hatching. Only ONE 'INFINITE' domain means that the hatching is fully included in the contour defined by the elements.", py::arg("IndH"));
	cls_Geom2dHatch_Hatcher.def("Domain", (const HatchGen_Domain & (Geom2dHatch_Hatcher::*)(const Standard_Integer, const Standard_Integer) const ) &Geom2dHatch_Hatcher::Domain, "Returns the IDom-th domain of the IndH-th hatching.", py::arg("IndH"), py::arg("IDom"));
	// FIXME cls_Geom2dHatch_Hatcher.def("Dump", (void (Geom2dHatch_Hatcher::*)() const ) &Geom2dHatch_Hatcher::Dump, "Dump the hatcher.");

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Intersector.hxx
	py::class_<Geom2dHatch_Intersector, std::unique_ptr<Geom2dHatch_Intersector, Deleter<Geom2dHatch_Intersector>>, Geom2dInt_GInter> cls_Geom2dHatch_Intersector(mod, "Geom2dHatch_Intersector", "None");
	cls_Geom2dHatch_Intersector.def(py::init<const Standard_Real, const Standard_Real>(), py::arg("Confusion"), py::arg("Tangency"));
	cls_Geom2dHatch_Intersector.def(py::init<>());
	cls_Geom2dHatch_Intersector.def("ConfusionTolerance", (Standard_Real (Geom2dHatch_Intersector::*)() const ) &Geom2dHatch_Intersector::ConfusionTolerance, "Returns the confusion tolerance of the intersector.");
	cls_Geom2dHatch_Intersector.def("SetConfusionTolerance", (void (Geom2dHatch_Intersector::*)(const Standard_Real)) &Geom2dHatch_Intersector::SetConfusionTolerance, "Sets the confusion tolerance of the intersector.", py::arg("Confusion"));
	cls_Geom2dHatch_Intersector.def("TangencyTolerance", (Standard_Real (Geom2dHatch_Intersector::*)() const ) &Geom2dHatch_Intersector::TangencyTolerance, "Returns the tangency tolerance of the intersector.");
	cls_Geom2dHatch_Intersector.def("SetTangencyTolerance", (void (Geom2dHatch_Intersector::*)(const Standard_Real)) &Geom2dHatch_Intersector::SetTangencyTolerance, "Sets the tangency tolerance of the intersector.", py::arg("Tangency"));
	cls_Geom2dHatch_Intersector.def("Intersect", (void (Geom2dHatch_Intersector::*)(const Geom2dAdaptor_Curve &, const Geom2dAdaptor_Curve &)) &Geom2dHatch_Intersector::Intersect, "Intersects the curves C1 and C2. The results are retreived by the usual methods described in IntRes2d_Intersection. Creates an intersector.", py::arg("C1"), py::arg("C2"));
	cls_Geom2dHatch_Intersector.def("Perform", (void (Geom2dHatch_Intersector::*)(const gp_Lin2d &, const Standard_Real, const Standard_Real, const Geom2dAdaptor_Curve &)) &Geom2dHatch_Intersector::Perform, "Performs the intersection between the 2d line segment (<L>, <P>) and the Curve <E>. The line segment is the part of the 2d line <L> of parameter range [0, <P>] (P is positive and can be RealLast()). Tol is the Tolerance on the segment. The order is relevant, the first argument is the segment, the second the Edge.", py::arg("L"), py::arg("P"), py::arg("Tol"), py::arg("E"));
	cls_Geom2dHatch_Intersector.def("LocalGeometry", [](Geom2dHatch_Intersector &self, const Geom2dAdaptor_Curve & E, const Standard_Real U, gp_Dir2d & T, gp_Dir2d & N, Standard_Real & C){ self.LocalGeometry(E, U, T, N, C); return C; }, "Returns in <T>, <N> and <C> the tangent, normal and curvature of the edge <E> at parameter value <U>.", py::arg("E"), py::arg("U"), py::arg("T"), py::arg("N"), py::arg("C"));

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_FClass2dOfClassifier.hxx
	py::class_<Geom2dHatch_FClass2dOfClassifier, std::unique_ptr<Geom2dHatch_FClass2dOfClassifier, Deleter<Geom2dHatch_FClass2dOfClassifier>>> cls_Geom2dHatch_FClass2dOfClassifier(mod, "Geom2dHatch_FClass2dOfClassifier", "None");
	cls_Geom2dHatch_FClass2dOfClassifier.def(py::init<>());
	cls_Geom2dHatch_FClass2dOfClassifier.def("Reset", (void (Geom2dHatch_FClass2dOfClassifier::*)(const gp_Lin2d &, const Standard_Real, const Standard_Real)) &Geom2dHatch_FClass2dOfClassifier::Reset, "Starts a classification process. The point to classify is the origin of the line <L>. <P> is the original length of the segment on <L> used to compute intersections. <Tol> is the tolerance attached to the line segment in intersections.", py::arg("L"), py::arg("P"), py::arg("Tol"));
	cls_Geom2dHatch_FClass2dOfClassifier.def("Compare", (void (Geom2dHatch_FClass2dOfClassifier::*)(const Geom2dAdaptor_Curve &, const TopAbs_Orientation)) &Geom2dHatch_FClass2dOfClassifier::Compare, "Updates the classification process with the edge <E> from the boundary.", py::arg("E"), py::arg("Or"));
	cls_Geom2dHatch_FClass2dOfClassifier.def("Parameter", (Standard_Real (Geom2dHatch_FClass2dOfClassifier::*)() const ) &Geom2dHatch_FClass2dOfClassifier::Parameter, "Returns the current value of the parameter.");
	cls_Geom2dHatch_FClass2dOfClassifier.def("Intersector", (Geom2dHatch_Intersector & (Geom2dHatch_FClass2dOfClassifier::*)()) &Geom2dHatch_FClass2dOfClassifier::Intersector, "Returns the intersecting algorithm.");
	cls_Geom2dHatch_FClass2dOfClassifier.def("ClosestIntersection", (Standard_Integer (Geom2dHatch_FClass2dOfClassifier::*)() const ) &Geom2dHatch_FClass2dOfClassifier::ClosestIntersection, "Returns 0 if the last compared edge had no relevant intersection. Else returns the index of this intersection in the last intersection algorithm.");
	cls_Geom2dHatch_FClass2dOfClassifier.def("State", (TopAbs_State (Geom2dHatch_FClass2dOfClassifier::*)() const ) &Geom2dHatch_FClass2dOfClassifier::State, "Returns the current state of the point.");
	cls_Geom2dHatch_FClass2dOfClassifier.def("IsHeadOrEnd", (Standard_Boolean (Geom2dHatch_FClass2dOfClassifier::*)() const ) &Geom2dHatch_FClass2dOfClassifier::IsHeadOrEnd, "Returns the Standard_True if the closest intersection point represents head or end of the edge. Returns Standard_False otherwise.");

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Elements.hxx
	py::class_<Geom2dHatch_Elements, std::unique_ptr<Geom2dHatch_Elements, Deleter<Geom2dHatch_Elements>>> cls_Geom2dHatch_Elements(mod, "Geom2dHatch_Elements", "None");
	cls_Geom2dHatch_Elements.def(py::init<>());
	cls_Geom2dHatch_Elements.def(py::init([] (const Geom2dHatch_Elements &other) {return new Geom2dHatch_Elements(other);}), "Copy constructor", py::arg("other"));
	cls_Geom2dHatch_Elements.def("Clear", (void (Geom2dHatch_Elements::*)()) &Geom2dHatch_Elements::Clear, "None");
	cls_Geom2dHatch_Elements.def("Bind", (Standard_Boolean (Geom2dHatch_Elements::*)(const Standard_Integer, const Geom2dHatch_Element &)) &Geom2dHatch_Elements::Bind, "None", py::arg("K"), py::arg("I"));
	cls_Geom2dHatch_Elements.def("IsBound", (Standard_Boolean (Geom2dHatch_Elements::*)(const Standard_Integer) const ) &Geom2dHatch_Elements::IsBound, "None", py::arg("K"));
	cls_Geom2dHatch_Elements.def("UnBind", (Standard_Boolean (Geom2dHatch_Elements::*)(const Standard_Integer)) &Geom2dHatch_Elements::UnBind, "None", py::arg("K"));
	cls_Geom2dHatch_Elements.def("Find", (const Geom2dHatch_Element & (Geom2dHatch_Elements::*)(const Standard_Integer) const ) &Geom2dHatch_Elements::Find, "None", py::arg("K"));
	cls_Geom2dHatch_Elements.def("__call__", (const Geom2dHatch_Element & (Geom2dHatch_Elements::*)(const Standard_Integer) const ) &Geom2dHatch_Elements::operator(), py::is_operator(), "None", py::arg("K"));
	cls_Geom2dHatch_Elements.def("ChangeFind", (Geom2dHatch_Element & (Geom2dHatch_Elements::*)(const Standard_Integer)) &Geom2dHatch_Elements::ChangeFind, "None", py::arg("K"));
	cls_Geom2dHatch_Elements.def("__call__", (Geom2dHatch_Element & (Geom2dHatch_Elements::*)(const Standard_Integer)) &Geom2dHatch_Elements::operator(), py::is_operator(), "None", py::arg("K"));
	cls_Geom2dHatch_Elements.def("CheckPoint", (Standard_Boolean (Geom2dHatch_Elements::*)(gp_Pnt2d &)) &Geom2dHatch_Elements::CheckPoint, "None", py::arg("P"));
	cls_Geom2dHatch_Elements.def("Reject", (Standard_Boolean (Geom2dHatch_Elements::*)(const gp_Pnt2d &) const ) &Geom2dHatch_Elements::Reject, "None", py::arg("P"));
	cls_Geom2dHatch_Elements.def("Segment", [](Geom2dHatch_Elements &self, const gp_Pnt2d & P, gp_Lin2d & L, Standard_Real & Par){ Standard_Boolean rv = self.Segment(P, L, Par); return std::tuple<Standard_Boolean, Standard_Real &>(rv, Par); }, "None", py::arg("P"), py::arg("L"), py::arg("Par"));
	cls_Geom2dHatch_Elements.def("OtherSegment", [](Geom2dHatch_Elements &self, const gp_Pnt2d & P, gp_Lin2d & L, Standard_Real & Par){ Standard_Boolean rv = self.OtherSegment(P, L, Par); return std::tuple<Standard_Boolean, Standard_Real &>(rv, Par); }, "None", py::arg("P"), py::arg("L"), py::arg("Par"));
	cls_Geom2dHatch_Elements.def("InitWires", (void (Geom2dHatch_Elements::*)()) &Geom2dHatch_Elements::InitWires, "None");
	cls_Geom2dHatch_Elements.def("MoreWires", (Standard_Boolean (Geom2dHatch_Elements::*)() const ) &Geom2dHatch_Elements::MoreWires, "None");
	cls_Geom2dHatch_Elements.def("NextWire", (void (Geom2dHatch_Elements::*)()) &Geom2dHatch_Elements::NextWire, "None");
	cls_Geom2dHatch_Elements.def("RejectWire", (Standard_Boolean (Geom2dHatch_Elements::*)(const gp_Lin2d &, const Standard_Real) const ) &Geom2dHatch_Elements::RejectWire, "None", py::arg("L"), py::arg("Par"));
	cls_Geom2dHatch_Elements.def("InitEdges", (void (Geom2dHatch_Elements::*)()) &Geom2dHatch_Elements::InitEdges, "None");
	cls_Geom2dHatch_Elements.def("MoreEdges", (Standard_Boolean (Geom2dHatch_Elements::*)() const ) &Geom2dHatch_Elements::MoreEdges, "None");
	cls_Geom2dHatch_Elements.def("NextEdge", (void (Geom2dHatch_Elements::*)()) &Geom2dHatch_Elements::NextEdge, "None");
	cls_Geom2dHatch_Elements.def("RejectEdge", (Standard_Boolean (Geom2dHatch_Elements::*)(const gp_Lin2d &, const Standard_Real) const ) &Geom2dHatch_Elements::RejectEdge, "None", py::arg("L"), py::arg("Par"));
	cls_Geom2dHatch_Elements.def("CurrentEdge", (void (Geom2dHatch_Elements::*)(Geom2dAdaptor_Curve &, TopAbs_Orientation &) const ) &Geom2dHatch_Elements::CurrentEdge, "None", py::arg("E"), py::arg("Or"));

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Classifier.hxx
	py::class_<Geom2dHatch_Classifier, std::unique_ptr<Geom2dHatch_Classifier, Deleter<Geom2dHatch_Classifier>>> cls_Geom2dHatch_Classifier(mod, "Geom2dHatch_Classifier", "None");
	cls_Geom2dHatch_Classifier.def(py::init<>());
	cls_Geom2dHatch_Classifier.def(py::init<Geom2dHatch_Elements &, const gp_Pnt2d &, const Standard_Real>(), py::arg("F"), py::arg("P"), py::arg("Tol"));
	cls_Geom2dHatch_Classifier.def("Perform", (void (Geom2dHatch_Classifier::*)(Geom2dHatch_Elements &, const gp_Pnt2d &, const Standard_Real)) &Geom2dHatch_Classifier::Perform, "Classify the Point P with Tolerance <T> on the face described by <F>.", py::arg("F"), py::arg("P"), py::arg("Tol"));
	cls_Geom2dHatch_Classifier.def("State", (TopAbs_State (Geom2dHatch_Classifier::*)() const ) &Geom2dHatch_Classifier::State, "Returns the result of the classification.");
	cls_Geom2dHatch_Classifier.def("Rejected", (Standard_Boolean (Geom2dHatch_Classifier::*)() const ) &Geom2dHatch_Classifier::Rejected, "Returns True when the state was computed by a rejection. The state is OUT.");
	cls_Geom2dHatch_Classifier.def("NoWires", (Standard_Boolean (Geom2dHatch_Classifier::*)() const ) &Geom2dHatch_Classifier::NoWires, "Returns True if the face contains no wire. The state is IN.");
	cls_Geom2dHatch_Classifier.def("Edge", (const Geom2dAdaptor_Curve & (Geom2dHatch_Classifier::*)() const ) &Geom2dHatch_Classifier::Edge, "Returns the Edge used to determine the classification. When the State is ON this is the Edge containing the point.");
	cls_Geom2dHatch_Classifier.def("EdgeParameter", (Standard_Real (Geom2dHatch_Classifier::*)() const ) &Geom2dHatch_Classifier::EdgeParameter, "Returns the parameter on Edge() used to determine the classification.");
	cls_Geom2dHatch_Classifier.def("Position", (IntRes2d_Position (Geom2dHatch_Classifier::*)() const ) &Geom2dHatch_Classifier::Position, "Returns the position of the point on the edge returned by Edge.");

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Element.hxx
	py::class_<Geom2dHatch_Element, std::unique_ptr<Geom2dHatch_Element, Deleter<Geom2dHatch_Element>>> cls_Geom2dHatch_Element(mod, "Geom2dHatch_Element", "None");
	cls_Geom2dHatch_Element.def(py::init<>());
	cls_Geom2dHatch_Element.def(py::init([] (const Geom2dHatch_Element &other) {return new Geom2dHatch_Element(other);}), "Copy constructor", py::arg("other"));
	cls_Geom2dHatch_Element.def(py::init<const Geom2dAdaptor_Curve &>(), py::arg("Curve"));
	cls_Geom2dHatch_Element.def(py::init<const Geom2dAdaptor_Curve &, const TopAbs_Orientation>(), py::arg("Curve"), py::arg("Orientation"));
	cls_Geom2dHatch_Element.def("Curve", (const Geom2dAdaptor_Curve & (Geom2dHatch_Element::*)() const ) &Geom2dHatch_Element::Curve, "Returns the curve associated to the element.");
	cls_Geom2dHatch_Element.def("ChangeCurve", (Geom2dAdaptor_Curve & (Geom2dHatch_Element::*)()) &Geom2dHatch_Element::ChangeCurve, "Returns the curve associated to the element.");
	cls_Geom2dHatch_Element.def("Orientation", (void (Geom2dHatch_Element::*)(const TopAbs_Orientation)) &Geom2dHatch_Element::Orientation, "Sets the orientation of the element.", py::arg("Orientation"));
	cls_Geom2dHatch_Element.def("Orientation", (TopAbs_Orientation (Geom2dHatch_Element::*)() const ) &Geom2dHatch_Element::Orientation, "Returns the orientation of the element.");

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Hatching.hxx
	py::class_<Geom2dHatch_Hatching, std::unique_ptr<Geom2dHatch_Hatching, Deleter<Geom2dHatch_Hatching>>> cls_Geom2dHatch_Hatching(mod, "Geom2dHatch_Hatching", "None");
	cls_Geom2dHatch_Hatching.def(py::init<>());
	cls_Geom2dHatch_Hatching.def(py::init<const Geom2dAdaptor_Curve &>(), py::arg("Curve"));
	cls_Geom2dHatch_Hatching.def("Curve", (const Geom2dAdaptor_Curve & (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::Curve, "Returns the curve associated to the hatching.");
	cls_Geom2dHatch_Hatching.def("ChangeCurve", (Geom2dAdaptor_Curve & (Geom2dHatch_Hatching::*)()) &Geom2dHatch_Hatching::ChangeCurve, "Returns the curve associated to the hatching.");
	cls_Geom2dHatch_Hatching.def("TrimDone", (void (Geom2dHatch_Hatching::*)(const Standard_Boolean)) &Geom2dHatch_Hatching::TrimDone, "Sets the flag about the trimming computations to the given value.", py::arg("Flag"));
	cls_Geom2dHatch_Hatching.def("TrimDone", (Standard_Boolean (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::TrimDone, "Returns the flag about the trimming computations.");
	cls_Geom2dHatch_Hatching.def("TrimFailed", (void (Geom2dHatch_Hatching::*)(const Standard_Boolean)) &Geom2dHatch_Hatching::TrimFailed, "Sets the flag about the trimming failure to the given value.", py::arg("Flag"));
	cls_Geom2dHatch_Hatching.def("TrimFailed", (Standard_Boolean (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::TrimFailed, "Returns the flag about the trimming failure.");
	cls_Geom2dHatch_Hatching.def("IsDone", (void (Geom2dHatch_Hatching::*)(const Standard_Boolean)) &Geom2dHatch_Hatching::IsDone, "Sets the flag about the domains computation to the given value.", py::arg("Flag"));
	cls_Geom2dHatch_Hatching.def("IsDone", (Standard_Boolean (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::IsDone, "Returns the flag about the domains computation.");
	cls_Geom2dHatch_Hatching.def("Status", (void (Geom2dHatch_Hatching::*)(const HatchGen_ErrorStatus)) &Geom2dHatch_Hatching::Status, "Sets the error status.", py::arg("theStatus"));
	cls_Geom2dHatch_Hatching.def("Status", (HatchGen_ErrorStatus (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::Status, "Returns the error status.");
	cls_Geom2dHatch_Hatching.def("AddPoint", (void (Geom2dHatch_Hatching::*)(const HatchGen_PointOnHatching &, const Standard_Real)) &Geom2dHatch_Hatching::AddPoint, "Adds an intersection point to the hatching.", py::arg("Point"), py::arg("Confusion"));
	cls_Geom2dHatch_Hatching.def("NbPoints", (Standard_Integer (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::NbPoints, "Returns the number of intersection points of the hatching.");
	cls_Geom2dHatch_Hatching.def("Point", (const HatchGen_PointOnHatching & (Geom2dHatch_Hatching::*)(const Standard_Integer) const ) &Geom2dHatch_Hatching::Point, "Returns the Index-th intersection point of the hatching. The exception OutOfRange is raised if Index < 1 or Index > NbPoints.", py::arg("Index"));
	cls_Geom2dHatch_Hatching.def("ChangePoint", (HatchGen_PointOnHatching & (Geom2dHatch_Hatching::*)(const Standard_Integer)) &Geom2dHatch_Hatching::ChangePoint, "Returns the Index-th intersection point of the hatching. The exception OutOfRange is raised if Index < 1 or Index > NbPoints.", py::arg("Index"));
	cls_Geom2dHatch_Hatching.def("RemPoint", (void (Geom2dHatch_Hatching::*)(const Standard_Integer)) &Geom2dHatch_Hatching::RemPoint, "Removes the Index-th intersection point of the hatching. The exception OutOfRange is raised if Index < 1 or Index > NbPoints.", py::arg("Index"));
	cls_Geom2dHatch_Hatching.def("ClrPoints", (void (Geom2dHatch_Hatching::*)()) &Geom2dHatch_Hatching::ClrPoints, "Removes all the intersection points of the hatching.");
	cls_Geom2dHatch_Hatching.def("AddDomain", (void (Geom2dHatch_Hatching::*)(const HatchGen_Domain &)) &Geom2dHatch_Hatching::AddDomain, "Adds a domain to the hatching.", py::arg("Domain"));
	cls_Geom2dHatch_Hatching.def("NbDomains", (Standard_Integer (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::NbDomains, "Returns the number of domains of the hatching.");
	cls_Geom2dHatch_Hatching.def("Domain", (const HatchGen_Domain & (Geom2dHatch_Hatching::*)(const Standard_Integer) const ) &Geom2dHatch_Hatching::Domain, "Returns the Index-th domain of the hatching. The exception OutOfRange is raised if Index < 1 or Index > NbDomains.", py::arg("Index"));
	cls_Geom2dHatch_Hatching.def("RemDomain", (void (Geom2dHatch_Hatching::*)(const Standard_Integer)) &Geom2dHatch_Hatching::RemDomain, "Removes the Index-th domain of the hatching. The exception OutOfRange is raised if Index < 1 or Index > NbDomains.", py::arg("Index"));
	cls_Geom2dHatch_Hatching.def("ClrDomains", (void (Geom2dHatch_Hatching::*)()) &Geom2dHatch_Hatching::ClrDomains, "Removes all the domains of the hatching.");
	cls_Geom2dHatch_Hatching.def("ClassificationPoint", (gp_Pnt2d (Geom2dHatch_Hatching::*)() const ) &Geom2dHatch_Hatching::ClassificationPoint, "Returns a point on the curve. This point will be used for the classification.");

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_MapOfElements.hxx
	bind_NCollection_DataMap<int, Geom2dHatch_Element, NCollection_DefaultHasher<int> >(mod, "Geom2dHatch_MapOfElements");

	/* FIXME

	*/

	// C:\Miniconda\envs\occt\Library\include\opencascade\Geom2dHatch_Hatchings.hxx
	bind_NCollection_DataMap<int, Geom2dHatch_Hatching, NCollection_DefaultHasher<int> >(mod, "Geom2dHatch_Hatchings");

	/* FIXME

	*/


}
