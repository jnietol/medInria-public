#ifndef medSegmentationWorkspace_H
#define medSegmentationWorkspace_H


// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include <medViewerWorkspace.h>

class QGraphicsSceneMouseEvent;

class medProgressionStack;

class medSegmentationWorkspacePrivate;
class medSegmentationSelectorToolBox;

/**
 * class medSegmentationWorkspace
 * Defines the segmentation workspace.
 */
class medSegmentationWorkspace : public medViewerWorkspace
{
    Q_OBJECT

public:
    medSegmentationWorkspace(QWidget * parent = NULL);

    virtual ~medSegmentationWorkspace(void);

    //! Overrides base class.
    QString description(void) const;
    //! Implement base class
    QString identifier(void) const;

    static bool isUsable();

    //! Implement abstract method in base class.
    void setupViewContainerStack();

    //! Register with factory, return true if successful.
    static bool registerWithViewerWorkspaceFactory();


    //!
    medProgressionStack * progressionStack();

    medSegmentationSelectorToolBox * segmentationToobox();

    void buildWorkspace(  );

    

signals:
    void viewAdded(dtkAbstractView* view);
    void viewRemoved(dtkAbstractView* view);

public slots:
    void onViewAdded(dtkAbstractView* view);
    void onViewRemoved(dtkAbstractView* view);

    /**@brief Connects toolboxes to the current container
     * @param name the container name */
    virtual void connectToolboxesToCurrentContainer(const QString &name);


private:
    static QString s_identifier();
    medSegmentationWorkspacePrivate *d;
};

#endif // medSegmentationWorkspace_H
