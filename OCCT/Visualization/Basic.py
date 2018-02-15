# This file is part of pyOCCT which provides Python bindings to the OpenCASCADE
# geometry kernel.
#
# Copyright (C) 2016-2018  Laughlin Research, LLC (info@laughlinresearch.com)
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
import os
import warnings

from OCCT.AIS import AIS_InteractiveContext, AIS_Shaded, AIS_Shape
from OCCT.Aspect import Aspect_DisplayConnection, Aspect_TOTP_LEFT_LOWER
from OCCT.BRepBuilderAPI import (BRepBuilderAPI_MakeVertex,
                                 BRepBuilderAPI_MakeEdge,
                                 BRepBuilderAPI_MakeFace)
from OCCT.Geom import Geom_Curve, Geom_Surface
from OCCT.Graphic3d import (Graphic3d_MaterialAspect, Graphic3d_NOM_DEFAULT)
from OCCT.MeshVS import (MeshVS_DA_DisplayNodes, MeshVS_DA_EdgeColor,
                         MeshVS_Mesh, MeshVS_MeshPrsBuilder)
from OCCT.OpenGl import OpenGl_GraphicDriver
from OCCT.Quantity import (Quantity_TOC_RGB, Quantity_NOC_WHITE,
                           Quantity_Color,
                           Quantity_NOC_BLACK)
from OCCT.TopoDS import TopoDS_Shape
from OCCT.V3d import V3d_Viewer, V3d_TypeOfOrientation
from OCCT.WNT import WNT_Window
from OCCT.gp import gp_Pnt

try:
    from PySide import QtCore
    from PySide.QtGui import QApplication, QPalette, QIcon, QMainWindow
    from PySide.QtOpenGL import QGLWidget

    has_pyside = True
except ImportError:
    has_pyside = False
    msg = "PySide module was not found for visualization."
    warnings.warn(msg, RuntimeWarning)

try:
    from OCCT.SMESH import SMESH_MeshVSLink, SMESH_Mesh

    has_smesh = True
except ImportError:
    has_smesh = False
    msg = "SMESH module was not found for visualization."
    warnings.warn(msg, RuntimeWarning)

__all__ = ['Viewer']


class View(QGLWidget):
    """
    View for displaying shapes.

    :param PySide.QtGui.QWidget parent: The parent widget.
    """

    def __init__(self, parent=None):
        if not has_pyside:
            raise ModuleNotFoundError("PySide was not found and is required.")
        super(View, self).__init__(parent)

        # Qt settings
        self.setBackgroundRole(QPalette.NoRole)
        self.setMouseTracking(True)

        # Create viewer and view
        self.my_viewer = None
        self.my_view = None

        # AIS interactive context
        self.my_context = None

        # Some default settings
        self._white = Quantity_Color(Quantity_NOC_WHITE)
        self._black = Quantity_Color(Quantity_NOC_BLACK)
        self.my_drawer = None
        self.graphic3d_cview = None

        # Values for mouse movement
        self._x0, self._y0 = 0., 0.

        # Display connection
        self.display_connect = Aspect_DisplayConnection()

        # Graphics driver
        self._graphics_driver = OpenGl_GraphicDriver(self.display_connect)

        # Window handle
        self.window_handle = self.winId()

        # Windows window
        self.wind = WNT_Window(self.window_handle)

        # Create viewer and view
        self.my_viewer = V3d_Viewer(self._graphics_driver)
        self.my_view = self.my_viewer.CreateView()
        self.my_view.SetWindow(self.wind)

        # Map window
        if not self.wind.IsMapped():
            self.wind.Map()

        # AIS interactive context
        self.my_context = AIS_InteractiveContext(self.my_viewer)

        # Some default settings
        self._white = Quantity_Color(Quantity_NOC_WHITE)
        self._black = Quantity_Color(Quantity_NOC_BLACK)

        self.my_context.SetAutomaticHilight(True)

        self.my_viewer.SetDefaultLights()
        self.my_viewer.SetLightOn()

        self.my_view.SetBackgroundColor(Quantity_TOC_RGB, 0.5, 0.5, 0.5)
        self.my_view.MustBeResized()

        self.my_context.SetDisplayMode(AIS_Shaded, True)

        self.my_drawer = self.my_context.DefaultDrawer()
        self.my_drawer.SetFaceBoundaryDraw(True)

        self.my_view.TriedronDisplay(Aspect_TOTP_LEFT_LOWER, self._black, 0.08)
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_XposYposZpos)

        self.graphic3d_cview = self.my_view.View()

        # Mesh display mode
        self._mesh_mode = 1

    def paintEvent(self, *args, **kwargs):
        self.my_view.Redraw()

    def resizeEvent(self, *args, **kwargs):
        self.my_view.MustBeResized()

    def wheelEvent(self, e):
        if e.delta() > 0:
            zoom = 1.5
        else:
            zoom = 0.75
        self.my_view.SetZoom(zoom)

    def mousePressEvent(self, e):
        pos = e.pos()
        x, y = pos.x(), pos.y()
        self._x0, self._y0 = x, y
        self.my_view.StartRotation(x, y)

    def mouseMoveEvent(self, e):
        pos = e.pos()
        x, y = pos.x(), pos.y()
        button = e.buttons()

        # Rotate
        if button == QtCore.Qt.LeftButton:
            self.my_view.Rotation(x, y)
        # Pan
        elif button in [QtCore.Qt.MidButton, QtCore.Qt.RightButton]:
            dx, dy = x - self._x0, y - self._y0
            self._x0, self._y0 = x, y
            self.my_view.Pan(dx, -dy)

    def fit(self):
        """
        Fit the contents.

        :return: None.
        """
        self.my_view.FitAll()
        self.my_view.ZFitAll()
        self.my_view.Redraw()

    def display(self, ais_shape, update=True):
        """
        Display an AIS_Shape.

        :param OCCT.AIS.AIS_Shape ais_shape: The AIS shape.
        :param bool update: Option to update the viewer.

        :return: None.
        """
        self.my_context.Display(ais_shape, update)

    def display_shape(self, shape, rgb=None, transparency=None,
                      material=Graphic3d_NOM_DEFAULT):
        """
        Display a shape.

        :param OCCT.TopoDS.TopoDS_Shape shape: The shape.
        :param rgb: The RGB color (r, g, b).
        :type rgb: collections.Sequence[float] or OCCT.Quantity.Quantity_Color
        :param float transparency: The transparency (0 to 1).
        :param OCCT.Graphic3d.Graphic3d_NameOfMaterial material: The material.

        :return: The AIS_Shape created for the part.
        :rtype: OCCT.AIS.AIS_Shape
        """
        ais_shape = AIS_Shape(shape)

        if isinstance(rgb, (tuple, list)):
            r, g, b = rgb
            if r > 1.:
                r /= 255.
            if g > 1.:
                g /= 255.
            if b > 1.:
                b /= 255.
            color = Quantity_Color(r, g, b, Quantity_TOC_RGB)
            ais_shape.SetColor(color)
        elif isinstance(rgb, Quantity_Color):
            ais_shape.SetColor(rgb)

        if transparency is not None:
            ais_shape.SetTransparency(transparency)

        ma = Graphic3d_MaterialAspect(material)
        ais_shape.SetMaterial(ma)

        self.my_context.Display(ais_shape, True)
        return ais_shape

    def display_geom(self, geom, rgb=None, transparency=None,
                     material=Graphic3d_NOM_DEFAULT):
        """
        Display a geometric entity.

        :param geom: The geometry.
        :type geom: OCCT.gp.gp_Pnt or OCCT.Geom.Geom_Curve or
            OCCT.Geom.Geom_Surface
        :param rgb: The RGB color (r, g, b).
        :type rgb: collections.Sequence[float] or OCCT.Quantity.Quantity_Color
        :param float transparency: The transparency (0 to 1).
        :param OCCT.Graphic3d.Graphic3d_NameOfMaterial material: The material.

        :return: The AIS_Shape created for the geometry. Returns *None* if the
            entity cannot be converted to a shape.
        :rtype: OCCT.AIS.AIS_Shape or None
        """
        if isinstance(geom, gp_Pnt):
            shape = BRepBuilderAPI_MakeVertex(geom).Vertex()
        elif isinstance(geom, Geom_Curve):
            shape = BRepBuilderAPI_MakeEdge(geom).Edge()
        elif isinstance(geom, Geom_Surface):
            shape = BRepBuilderAPI_MakeFace(geom, 1.0e-7).Face()
        else:
            return None

        return self.display_shape(shape, rgb, transparency, material)

    def display_mesh(self, mesh, mode=None):
        """
        Display a mesh.

        :param OCCT.SMESH.SMESH_Mesh mesh: The mesh.
        :param int mode: Display mode for mesh elements (1=wireframe, 2=solid).

        :return: The MeshVS_Mesh created for the mesh.
        :rtype: OCCT.MeshVS.MeshVS_Mesh
        """
        if mode is None:
            mode = self._mesh_mode

        vs_link = SMESH_MeshVSLink(mesh)
        mesh_vs = MeshVS_Mesh()
        mesh_vs.SetDataSource(vs_link)
        prs_builder = MeshVS_MeshPrsBuilder(mesh_vs)
        mesh_vs.AddBuilder(prs_builder)
        mesh_vs_drawer = mesh_vs.GetDrawer()
        mesh_vs_drawer.SetBoolean(MeshVS_DA_DisplayNodes, False)
        mesh_vs_drawer.SetColor(MeshVS_DA_EdgeColor, self._black)
        mesh_vs.SetDisplayMode(mode)
        self.my_context.Display(mesh_vs, True)
        return mesh_vs

    def set_bg_color(self, r, g, b):
        """
        Set the background color.

        :param float r: The r-value.
        :param float g: The g-value.
        :param float b: The b-value.

        :return: None.
        """
        self.my_view.SetBackgroundColor(Quantity_TOC_RGB, r, g, b)

    def set_white_background(self):
        """
        Set the background color to white.

        :return: None.
        """
        self.my_view.SetBackgroundColor(Quantity_TOC_RGB, 1., 1., 1.)

    def set_mesh_wireframe(self):
        """
        Set the default mesh view to wireframe.

        :return: None.
        """
        self._mesh_mode = 1

    def set_mesh_shaded(self):
        """
        Set the default mesh view to shaded.

        :return: None.
        """
        self._mesh_mode = 2

    def view_iso(self):
        """
        Isometric view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_XposYposZpos)

    def view_top(self):
        """
        Top view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_Zpos)

    def view_bottom(self):
        """
        Bottom view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_Zneg)

    def view_front(self):
        """
        Front view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_Xneg)

    def view_rear(self):
        """
        Rear view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_Xpos)

    def view_left(self):
        """
        Left view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_Yneg)

    def view_right(self):
        """
        Right view.

        :return: None.
        """
        self.my_view.SetProj(V3d_TypeOfOrientation.V3d_Ypos)

    def capture(self, fn):
        """
        Capture the screen contents and save to a file. The type of file will be
        determined by the extension.

        :param str fn: The filename.

        :return: None.
        """
        self.my_view.Dump(fn)

    def remove_all(self):
        """
        Remove all items from the context.

        :return: None.
        """
        self.my_context.RemoveAll(True)

    def export_pdf(self, fn):
        """
        Export the screen contents to PDF.
        :param str fn: The filename.

        :return: None.
        """
        raise NotImplemented('Need gl2ps library.')


class Viewer(QMainWindow):
    """
    Simple class for viewing items.

    :param int width: Window width.
    :param int height: Window height.
    :param PySide.QtGui.QWidget parent: The parent widget.
    """

    def __init__(self, width=800, height=600, parent=None):
        if not has_pyside:
            raise ModuleNotFoundError("PySide was not found and is required.")
        # Start app
        self._app = QApplication.instance()
        if self._app is None:
            self._app = QApplication([])
        super(Viewer, self).__init__(parent)

        # Create the OCCT view
        self._the_view = View(self)
        self.setCentralWidget(self._the_view)

        # Window settings
        self.setWindowTitle('pyOCCT')
        _icon = os.path.dirname(__file__) + '/_resources/icon.png'
        _qicon = QIcon(_icon)
        self.setWindowIcon(_qicon)
        self.resize(width, height)

    @property
    def view(self):
        return self._the_view

    def keyPressEvent(self, e):
        if e.key() == ord('F'):
            self.view.fit()
        elif e.key() == ord('I'):
            self.view.view_iso()
        elif e.key() == ord('T'):
            self.view.view_top()
        else:
            print('Key is not mapped to anything.')

    def display(self, shape, rgb=None, transparency=None,
                material=Graphic3d_NOM_DEFAULT):
        """
        Display a shape.

        :param OCCT.TopoDS.TopoDS_Shape shape: The shape.
        :param rgb: The RGB color (r, g, b).
        :type rgb: collections.Sequence[float] or OCCT.Quantity.Quantity_Color
        :param float transparency: The transparency (0 to 1).
        :param OCCT.Graphic3d.Graphic3d_NameOfMaterial material: The material.

        :return: The AIS_Shape created for the part.
        :rtype: OCCT.AIS.AIS_Shape
        """
        return self.view.display_shape(shape, rgb, transparency, material)

    def add(self, entity, rgb=None, transparency=None,
            material=Graphic3d_NOM_DEFAULT, mode=None):
        """
        Add an entity to the view.

        :param entity: The entity.
        :param rgb: The RGB color (r, g, b).
        :type rgb: collections.Sequence[float] or OCCT.Quantity.Quantity_Color
        :param float transparency: The transparency (0 to 1).
        :param OCCT.Graphic3d.Graphic3d_NameOfMaterial material: The material.
        :param int mode: Display mode for mesh elements (1=wireframe, 2=solid).

        :return: The AIS_Shape created for the entity. Returns *None* if the
            entity cannot be converted to a shape.
        :rtype: OCCT.AIS.AIS_Shape or None
        """
        if isinstance(entity, TopoDS_Shape):
            return self.view.display_shape(entity, rgb, transparency, material)
        elif isinstance(entity, (gp_Pnt, Geom_Curve, Geom_Surface)):
            return self.view.display_geom(entity, rgb, transparency, material)
        elif isinstance(entity, SMESH_Mesh):
            return self.view.display_mesh(entity, mode)
        else:
            return None

    def clear(self):
        """
        Clear contents of the view.

        :return: None.
        """
        self.view.remove_all()

    def start(self, fit=True):
        """
        Start the viewer.

        :param bool fit: Option to fit contents.

        :return: None.
        """
        if fit:
            self.view.fit()
        self.show()
        self._app.exec_()
