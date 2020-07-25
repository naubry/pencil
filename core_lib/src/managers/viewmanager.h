/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2012-2018 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QTransform>
#include "basemanager.h"

class Layer;
class LayerCamera;
class Camera;


class ViewManager : public BaseManager
{
    Q_OBJECT

public:
    explicit ViewManager(Editor* editor);
    ~ViewManager() override;

    bool init() override;
    Status load(Object*) override;
    Status save(Object*) override;
    void workingLayerChanged(Layer* layer) override;

    QTransform getView();
    QTransform getViewInverse();
    void resetView();

    QPointF mapCanvasToScreen(QPointF p);
    QPointF mapScreenToCanvas(QPointF p);

    QRectF mapCanvasToScreen(const QRectF& rect);
    QRectF mapScreenToCanvas(const QRectF& rect);

    QPolygonF mapPolygonToScreen(const QPolygonF& polygon);
    QPolygonF mapPolygonToCanvas(const QPolygonF& polygon);

    QPainterPath mapCanvasToScreen(const QPainterPath& path);
    QPainterPath mapScreenToCanvas(const QPainterPath& path);

    QPointF translation();
    void translate(float dx, float dy);
    void translate(QPointF offset);

    float rotation();
    void rotate(float degree);

    qreal scaling();
    void scale(qreal scaleValue, QPointF offset = {0, 0});
    void scaleUp();
    void scaleDown();
    void scale400();
    void scale300();
    void scale200();
    void scale100();
    void scale50();
    void scale33();
    void scale25();

    void flipHorizontal(bool b);
    void flipVertical(bool b);
    void setOverlayCenter(bool b);
    void setOverlayThirds(bool b);
    void setOverlayGoldenRatio(bool b);
    void setOverlaySafeAreas(bool b);

    bool isFlipHorizontal() { return mIsFlipHorizontal; }
    bool isFlipVertical() { return mIsFlipVertical; }
    bool getOverlayCenter() { return mOverlayCenter; }
    bool getOverlayThirds() { return mOverlayThirds; }
    bool getOverlayGoldenRatio() { return mOverlayGoldenRatio; }
    bool getOverlaySafeAreas() { return mOverlaySafeAreas; }


    void setCanvasSize(QSize size);
    void setCameraLayer(Layer* layer);

    QTransform getImportView() { return mImportView; }
    void setImportView(const QTransform& newView) { mImportView = newView; }

    void setImportFollowsCamera(bool b) { mImportFollowsCamera = b; }
    bool getImportFollowsCamera() { return mImportFollowsCamera; }

    void updateViewTransforms();

    Q_SIGNAL void viewChanged();
    Q_SIGNAL void viewFlipped();

private:

    void onCurrentFrameChanged();

    QTransform mView;
    QTransform mViewInverse;
    QTransform mViewCanvas;
    QTransform mViewCanvasInverse;
    QTransform mCentre;
    QTransform mImportView;

    Camera* mDefaultEditorCamera = nullptr;
    Camera* mCurrentCamera = nullptr;

    QSize mCanvasSize = { 1, 1 };

    bool mIsFlipHorizontal = false;
    bool mIsFlipVertical = false;
    bool mOverlayCenter = false;
    bool mOverlayThirds = false;
    bool mOverlayGoldenRatio = false;
    bool mOverlaySafeAreas = false;

    bool mImportFollowsCamera = false;

    LayerCamera* mCameraLayer = nullptr;
};

#endif // VIEWMANAGER_H
