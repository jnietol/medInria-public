// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef V3DVIEW2D_H
#define V3DVIEW2D_H

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractView.h>

#include "v3dViewPluginExport.h"

class v3dView2DPrivate;

class V3DVIEWPLUGIN_EXPORT v3dView2D : public dtkAbstractView
{
    Q_OBJECT

public:
             v3dView2D(void);
    virtual ~v3dView2D(void);

    virtual QString description(void) const;

    static bool registered(void);

public:
    void reset(void);
    void clear(void);
    void update(void);

    void   link(dtkAbstractView *other);
    void unlink(dtkAbstractView *other);

    void *view(void);

    void setData(dtkAbstractData *data);

    QWidget *widget(void);

private:
    v3dView2DPrivate *d;
};

dtkAbstractView *createV3dView2D(void);

#endif
