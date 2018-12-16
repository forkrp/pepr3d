#include "Brush.h"
#include "commands/CmdPaintBrush.h"
#include "geometry/Geometry.h"
#include "ui/MainApplication.h"
#include "ui/ModelView.h"

namespace pepr3d {
void Brush::onModelViewMouseDown(ModelView& modelView, ci::app::MouseEvent event) {
    if(!event.isLeft()) {
        return;
    }
    mLastRay = modelView.getRayFromWindowCoordinates(event.getPos());
    paint();
}

void Brush::onModelViewMouseUp(ModelView& modelView, ci::app::MouseEvent event) {
    if(!event.isLeft()) {
        return;
    }
    stopPaint();
}

void Brush::onModelViewMouseDrag(ModelView& modelView, ci::app::MouseEvent event) {
    if(!event.isLeftDown()) {
        return;
    }
    mLastRay = modelView.getRayFromWindowCoordinates(event.getPos());
    paint();
    updateHighlight();
}

void Brush::onModelViewMouseMove(ModelView& modelView, ci::app::MouseEvent event) {
    mLastRay = modelView.getRayFromWindowCoordinates(event.getPos());
    updateHighlight();
}

void Brush::paint() {
    // Prevents blocking the rendering if painting takes too long
    if(mPaintsSinceDraw >= MAX_PAINTS_WITHOUT_DRAW) {
        return;
    }

    mPaintsSinceDraw++;

    mBrushSettings.color = mApplication.getCurrentGeometry()->getColorManager().getActiveColorIndex();
    auto* commandManager = mApplication.getCommandManager();
    if(commandManager) {
        commandManager->execute(std::make_unique<CmdPaintBrush>(mLastRay, mBrushSettings), mGroupCommands);
    }

    mGroupCommands = true;
}

void Brush::stopPaint() {
    mGroupCommands = false;
}

void Brush::updateHighlight() const {
    mApplication.getCurrentGeometry()->highlightArea(mLastRay, mBrushSettings);
}

void Brush::drawToSidePane(SidePane& sidePane) {
    sidePane.drawColorPalette(mApplication.getCurrentGeometry()->getColorManager());
    sidePane.drawSeparator();

    sidePane.drawCheckbox("Continuous", mBrushSettings.continuous);
    sidePane.drawCheckbox("Paint backfaces", mBrushSettings.paintBackfaces);
    sidePane.drawCheckbox("Respect original triangles", mBrushSettings.respectOriginalTriangles);

    if(mBrushSettings.respectOriginalTriangles) {
        sidePane.drawCheckbox("Paint outer ring", mBrushSettings.paintOuterRing);
    }

    mPaintsSinceDraw = 0;
}
}  // namespace pepr3d
