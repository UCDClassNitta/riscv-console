#include "GUIWindowGTK3.h"
#include "GraphicSurfaceCairo.h"

CGUIWindowGTK3::CGUIWindowGTK3(GtkWidget *widget, bool reference) : CGUIContainerGTK3(widget, reference){

}

CGUIWindowGTK3::~CGUIWindowGTK3(){
    
}

void CGUIWindowGTK3::GetPosition(int &x, int &y){
    gint RootX, RootY;
    gtk_window_get_position(GTK_WINDOW(DWidget),&RootX,&RootY);
    x = RootX;
    y = RootY;
}

void CGUIWindowGTK3::Move(int x, int y){
    gtk_window_move(GTK_WINDOW(DWidget),x,y);
}

void CGUIWindowGTK3::SetMinSize(int width, int height){
    GdkGeometry Geometry;
    Geometry.min_width = width;
    Geometry.min_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), NULL, &Geometry, GDK_HINT_MIN_SIZE);
}

void CGUIWindowGTK3::SetMaxSize(int width, int height){
    GdkGeometry Geometry;
    Geometry.max_width = width;
    Geometry.max_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), NULL, &Geometry, GDK_HINT_MAX_SIZE);    
}

void CGUIWindowGTK3::SetMinSize(std::shared_ptr<CGUIWidget> widget, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToSize = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    GdkGeometry Geometry;
    Geometry.min_width = width;
    Geometry.min_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), WidgetToSize->Widget(), &Geometry, GDK_HINT_MIN_SIZE);
}

void CGUIWindowGTK3::SetMaxSize(std::shared_ptr<CGUIWidget> widget, int width, int height){
    std::shared_ptr<CGUIWidgetGTK3> WidgetToSize = std::dynamic_pointer_cast<CGUIWidgetGTK3>(widget);
    GdkGeometry Geometry;
    Geometry.max_width = width;
    Geometry.max_height = height;
    gtk_window_set_geometry_hints(GTK_WINDOW(DWidget), WidgetToSize->Widget(), &Geometry, GDK_HINT_MAX_SIZE);    
}

void CGUIWindowGTK3::Resize(int width, int height){
    gtk_window_resize(GTK_WINDOW(DWidget), width, height);
}

void CGUIWindowGTK3::SetTitle(const std::string &title){
    gtk_window_set_title(GTK_WINDOW(DWidget), title.c_str());
}

void CGUIWindowGTK3::Close(){
    gtk_window_close(GTK_WINDOW(DWidget));
}

std::shared_ptr<CGraphicSurface> CGUIWindowGTK3::RenderToSurface(){
    auto ReturnSurface = gdk_window_create_similar_image_surface(gtk_widget_get_window(DWidget),
                                                       CAIRO_FORMAT_ARGB32,
                                                       AllocatedWidth(),
                                                       AllocatedHeight(), 0);
    auto ContextResource = cairo_create(ReturnSurface);
    gdk_cairo_set_source_window(ContextResource,gtk_widget_get_window(DWidget),0,0);
    cairo_paint(ContextResource);
    cairo_destroy(ContextResource);
    return std::make_shared<CGraphicSurfaceCairo>(ReturnSurface);
}