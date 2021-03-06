/*

Pencil2D - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2012-2020 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/
#ifndef LAYER_H
#define LAYER_H

#include <map>
#include <functional>
#include <QObject>
#include <QString>
#include <QDomElement>
#include "pencilerror.h"
#include "pencildef.h"

class QMouseEvent;
class QPainter;

class KeyFrame;
class Object;
class TimeLineCells;
class Status;

#define ProgressCallback std::function<void()>


class Layer : public QObject
{
    Q_OBJECT

public:
    enum LAYER_TYPE
    {
        UNDEFINED = 0,
        BITMAP = 1,
        VECTOR = 2,
        MOVIE = 3, // not supported yet
        SOUND = 4,
        CAMERA = 5,
    };

    explicit Layer(Object*, LAYER_TYPE);
    virtual ~Layer();

    int id() const { return mId; }
    LAYER_TYPE type() const { return meType; }

    Object* object() const { return mObject; }
    void setObject(Object* obj);

    void setName(QString name) { mName = name; }
    QString name() const { return mName; }

    void switchVisibility() { mVisible = !mVisible; }

    bool visible() const { return mVisible; }
    void setVisible(bool b) { mVisible = b; }

    virtual Status saveKeyFrameFile(KeyFrame*, QString dataPath) = 0;
    virtual void loadDomElement(const QDomElement& element, QString dataDirPath, ProgressCallback progressForward) = 0;
    virtual QDomElement createDomElement(QDomDocument& doc) const = 0;
    QDomElement createBaseDomElement(QDomDocument& doc) const;
    void loadBaseDomElement(const QDomElement& elem);

    // KeyFrame interface
    int getMaxKeyFramePosition() const;
    int firstKeyFramePosition() const;

    bool keyExists(int position) const;
    int  getPreviousKeyFramePosition(int position) const;
    int  getNextKeyFramePosition(int position) const;
    int  getPreviousFrameNumber(int position, bool isAbsolute) const;
    int  getNextFrameNumber(int position, bool isAbsolute) const;

    int keyFrameCount() const { return static_cast<int>(mKeyFrames.size()); }

    bool addNewKeyFrameAt(int position);
    bool addKeyFrame(int position, KeyFrame*);
    bool removeKeyFrame(int position);
    bool swapKeyFrames(int position1, int position2);
    bool moveKeyFrameForward(int position);
    bool moveKeyFrameBackward(int position);
    bool loadKey(KeyFrame*);
    KeyFrame* getKeyFrameAt(int position) const;
    KeyFrame* getLastKeyFrameAtPosition(int position) const;
    bool keyExistsWhichCovers(int frameNumber);
    KeyFrame *getKeyFrameWhichCovers(int frameNumber) const;
    bool getVisibility() { return mVisible; }

    void foreachKeyFrame(std::function<void(KeyFrame*)>) const;

    void setModified(int position, bool isModified);

    // Handle selection
    bool isFrameSelected(int position) const;
    void setFrameSelected(int position, bool isSelected);
    void toggleFrameSelected(int position, bool allowMultiple = false);
    void extendSelectionTo(int position);
    void selectAllFramesAfter(int position);
    void deselectAll();

    bool moveSelectedFrames(int offset);

    Status save(const QString& sDataFolder, QStringList& attachedFiles, ProgressCallback progressStep);
    virtual Status presave(const QString& sDataFolder) { Q_UNUSED(sDataFolder); return Status::SAFE; }

    // graphic representation -- could be put in another class
    void paintTrack(QPainter& painter, TimeLineCells* cells, int x, int y, int width, int height, bool selected, int frameSize) const;
    void paintFrames(QPainter& painter, QColor trackCol, TimeLineCells* cells, int y, int height, bool selected, int frameSize) const;
    void paintLabel(QPainter& painter, TimeLineCells* cells, int x, int y, int height, int width, bool selected, LayerVisibility layerVisibility) const;
    void paintSelection(QPainter& painter, int x, int y, int height, int width) const;
    void mouseDoubleClick(QMouseEvent*, int frameNumber);

    virtual void editProperties(); // TODO: it's used by camera layers only, should move somewhere else

    bool isPaintable() const;

protected:
    void setId(int LayerId) { mId = LayerId; }
    virtual KeyFrame* createKeyFrame(int position, Object*) = 0;

private:
    LAYER_TYPE meType = UNDEFINED;
    Object*    mObject = nullptr;
    int        mId = 0;
    bool       mVisible = true;
    QString    mName;

    std::map<int, KeyFrame*, std::greater<int>> mKeyFrames;

    // We need to keep track of selected frames ordered by last selected
    // and by position.
    // Both should be pre-sorted on each selection for optimization purpose when moving frames.
    //
    QList<int> mSelectedFrames_byLast; // Used to handle selection range (based on last selected
    QList<int> mSelectedFrames_byPosition; // Used to handle frames movements on the timeline
};

#endif
