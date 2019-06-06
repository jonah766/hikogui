// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "PipelineImage_Delegate.hpp"
#include "BoxModel.hpp"
#include "Window_forward.hpp"
#include "Device_forward.hpp"
#include "TTauri/all.hpp"
#include <limits>
#include <memory>
#include <vector>

namespace TTauri::GUI {

/*! View of a widget.
 * A view contains the dynamic data for a Widget. It is often accompanied with a Backing
 * which contains that static data of an Widget and the drawing code. Backings are shared
 * between Views.
 */
class Widget : public std::enable_shared_from_this<Widget>, public PipelineImage::Delegate {
public:
    //! Convenient reference to the Window.
    std::weak_ptr<Window> window;

    std::weak_ptr<Widget> parent;

    std::vector<std::shared_ptr<Widget>> children;

    //! Location of the frame compared to the window.
    BoxModel box;

    size_t depth = 0;

    /*! Constructor for creating subviews.
     */
    Widget();
    virtual ~Widget() {}

    Widget(const Widget &) = delete;
    Widget &operator=(const Widget &) = delete;
    Widget(Widget &&) = delete;
    Widget &operator=(Widget &&) = delete;

    virtual void setParent(const std::shared_ptr<Widget> &parent);

    virtual void add(std::shared_ptr<Widget> widget);

    std::shared_ptr<Device> device();

    void pipelineImagePlaceVertices(gsl::span<PipelineImage::Vertex> &vertices, size_t &offset) override;
};

}