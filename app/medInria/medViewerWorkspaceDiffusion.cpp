#include "medViewerWorkspaceDiffusion.h"

#include <dtkCore/dtkSmartPointer.h>
#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewInteractor.h>

#include <medDataManager.h>

#include "medViewerToolBoxViewProperties.h"
#include "medToolBoxDiffusionTensorView.h"
#include "medToolBoxDiffusion.h"
#include "medToolBoxDiffusionFiberView.h"
#include "medToolBoxDiffusionFiberBundling.h"
#include <medViewContainer.h>
#include <medViewContainerSingle.h>
#include <medTabbedViewContainers.h>

class medViewerWorkspaceDiffusionPrivate
{
public:

    medViewerToolBoxViewProperties      *viewPropertiesToolBox;
    medToolBoxDiffusionFiberView        *fiberViewToolBox;
    medToolBoxDiffusionFiberBundling    *fiberBundlingToolBox;
    medToolBoxDiffusion                 *diffusionToolBox;
    medToolBoxDiffusionTensorView       *tensorViewToolBox;

    QList<dtkSmartPointer<dtkAbstractView> > views;

    QString uuid;
};

medViewerWorkspaceDiffusion::medViewerWorkspaceDiffusion(QWidget *parent) : medViewerWorkspace(parent), d(new medViewerWorkspaceDiffusionPrivate)
{
    d->viewPropertiesToolBox = new medViewerToolBoxViewProperties(parent);

    // -- Bundling  toolbox --
    d->fiberBundlingToolBox = new medToolBoxDiffusionFiberBundling(parent);

    // -- Diffusion toolbox --
    d->diffusionToolBox = new medToolBoxDiffusion(parent);

    connect(d->diffusionToolBox, SIGNAL(addToolBox(medToolBox *)),
            this, SLOT(addToolBox(medToolBox *)));
    connect(d->diffusionToolBox, SIGNAL(removeToolBox(medToolBox *)),
            this, SLOT(removeToolBox(medToolBox *)));

    // -- Tensor tb --
    d->tensorViewToolBox = new medToolBoxDiffusionTensorView(parent);

    connect(d->tensorViewToolBox, SIGNAL(glyphShapeChanged(const QString&)), this, SLOT(onGlyphShapeChanged(const QString&)));
    connect(d->tensorViewToolBox, SIGNAL(flipX(bool)),                       this, SLOT(onFlipXChanged(bool)));
    connect(d->tensorViewToolBox, SIGNAL(flipY(bool)),                       this, SLOT(onFlipYChanged(bool)));
    connect(d->tensorViewToolBox, SIGNAL(flipZ(bool)),                       this, SLOT(onFlipZChanged(bool)));

    // -- Fiber view tb --
    d->fiberViewToolBox = new medToolBoxDiffusionFiberView(parent);

    connect(d->fiberViewToolBox, SIGNAL(fiberColorModeChanged(int)), this, SLOT(onFiberColorModeChanged(int)));
    connect(d->fiberViewToolBox, SIGNAL(GPUActivated(bool)),         this, SLOT(onGPUActivated(bool)));
    connect(d->fiberViewToolBox, SIGNAL(lineModeSelected(bool)),     this, SLOT(onLineModeSelected(bool)));
    connect(d->fiberViewToolBox, SIGNAL(ribbonModeSelected(bool)),   this, SLOT(onRibbonModeSelected(bool)));
    connect(d->fiberViewToolBox, SIGNAL(tubeModeSelected(bool)),     this, SLOT(onTubeModeSelected(bool)));

    connect(d->fiberBundlingToolBox, SIGNAL(fiberSelectionValidated(const QString&, const QColor&)), this, SLOT(refreshInteractors()));

    connect(d->diffusionToolBox, SIGNAL(success()),                  this, SLOT(onTBDiffusionSuccess()));


    this->addToolBox( d->diffusionToolBox );
    this->addToolBox( d->viewPropertiesToolBox );
    this->addToolBox( d->tensorViewToolBox );
    this->addToolBox( d->fiberViewToolBox );
    this->addToolBox( d->fiberBundlingToolBox );
}

medViewerWorkspaceDiffusion::~medViewerWorkspaceDiffusion(void)
{
    delete d;
    d = NULL;
}

QString medViewerWorkspaceDiffusion::identifier() const {
    return "Diffusion";
}

QString medViewerWorkspaceDiffusion::description() const {
    return "Diffusion";
}

void medViewerWorkspaceDiffusion::setupViewContainerStack()
{
    d->views.clear();
    medViewContainer * diffusionContainer = NULL;

    //the stack has been instantiated in constructor
    if (!this->stackedViewContainers()->count())
    {
        medViewContainerSingle *single = new medViewContainerSingle ();
        connect (single, SIGNAL (viewAdded (dtkAbstractView*)),   this, SLOT (onViewAdded (dtkAbstractView*)));
        connect (single, SIGNAL (viewRemoved (dtkAbstractView*)), this, SLOT (onViewRemoved (dtkAbstractView*)));

        //ownership of single is transferred to the stackedWidget.
        this->stackedViewContainers()->addContainer (identifier(), single);

        diffusionContainer = single;

        this->stackedViewContainers()->lockTabs();
        this->stackedViewContainers()->hideTabBar();
    }
    else
    {
        diffusionContainer = this->stackedViewContainers()->container(identifier());
        //TODO: maybe clear views here too?
    }

    if (!diffusionContainer)
        return;

    foreach(dtkAbstractView *view, diffusionContainer->views())
        d->views << view;

    //this->stackedViewContainers()->setContainer (identifier());
}


void medViewerWorkspaceDiffusion::onViewAdded (dtkAbstractView *view)
{
    if (!view)
        return;

    if (d->views.contains (view))
        return;

    d->views.append (view);

    view->enableInteractor ("v3dViewFiberInteractor");
    view->enableInteractor ("v3dViewTensorInteractor");

    if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor"))
    {
        connect(d->fiberViewToolBox, SIGNAL(fiberRadiusSet(int)), interactor, SLOT(onRadiusSet(int)));

        updateFiberInteractorWithToolboxValues(interactor, d->fiberViewToolBox);
    }

    if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewTensorInteractor"))
    {
        connect(d->tensorViewToolBox, SIGNAL(sampleRateChanged(int)), interactor, SLOT(onSampleRatePropertySet(int)));
        connect(d->tensorViewToolBox, SIGNAL(eigenVectorChanged(int)), interactor, SLOT(onEigenVectorPropertySet(int)));
        connect(d->tensorViewToolBox, SIGNAL(glyphResolutionChanged(int)), interactor, SLOT(onGlyphResolutionPropertySet(int)));
        connect(d->tensorViewToolBox, SIGNAL(reverseBackgroundColor(bool)), interactor, SLOT(onReverseBackgroundColorPropertySet(bool)));
        connect(d->tensorViewToolBox, SIGNAL(scalingChanged(double)), interactor, SLOT(onScalingPropertySet(double)));
        connect(d->tensorViewToolBox, SIGNAL(hideShowAxial(bool)), interactor, SLOT(onHideShowAxialPropertySet(bool)));
        connect(d->tensorViewToolBox, SIGNAL(hideShowCoronal(bool)), interactor, SLOT(onHideShowCoronalPropertySet(bool)));
        connect(d->tensorViewToolBox, SIGNAL(hideShowSagittal(bool)), interactor, SLOT(onHideShowSagittalPropertySet(bool)));

        // each change triggers and update of the view
        // TODO we need to unify the view->update() as now some updates are done here and some
        // are performed in the 'translate' function of the signal emitted by the toolbox
        // (e.g. onFlipXChanged) ... maybe after dtk is extended to support QVariant properties
        connect(d->tensorViewToolBox, SIGNAL(sampleRateChanged(int)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(eigenVectorChanged(int)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(glyphResolutionChanged(int)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(reverseBackgroundColor(bool)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(scalingChanged(double)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(hideShowAxial(bool)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(hideShowCoronal(bool)), view, SLOT(update(void)));
        connect(d->tensorViewToolBox, SIGNAL(hideShowSagittal(bool)), view, SLOT(update(void)));

        updateTensorInteractorWithToolboxValues(interactor, d->tensorViewToolBox);
    }
}

void medViewerWorkspaceDiffusion::updateTensorInteractorWithToolboxValues(dtkAbstractViewInteractor* interactor, medToolBoxDiffusionTensorView* tensorViewToolBox)
{
    // we are temporary using Qt's reflection in this function to call the slots in the interactor
    // without casting to a specific type (which is in a plugin)
    // this code will be modified once a refactor in dtk property system is done
    // (we might switch to QVariant instead of strings)
    // TODO refactor this...

    interactor->setProperty("GlyphShape", tensorViewToolBox->glyphShape());

    int sampleRate = tensorViewToolBox->sampleRate();
    QMetaObject::invokeMethod( interactor, "onSampleRatePropertySet", Qt::QueuedConnection, Q_ARG( int, sampleRate ) );

    bool isFlipX = tensorViewToolBox->isFlipX();
    interactor->setProperty("FlipX", isFlipX ? "true" : "false");

    bool isFlipY = tensorViewToolBox->isFlipY();
    interactor->setProperty("FlipY", isFlipY ? "true" : "false");

    bool isFlipZ = tensorViewToolBox->isFlipZ();
    interactor->setProperty("FlipZ", isFlipZ ? "true" : "false");

    int eigenVector = tensorViewToolBox->eigenVector();
    QMetaObject::invokeMethod( interactor, "onEigenVectorPropertySet", Qt::QueuedConnection, Q_ARG( int, eigenVector ) );

    int glyphResolution = tensorViewToolBox->glyphResolution();
    QMetaObject::invokeMethod( interactor, "onGlyphResolutionPropertySet", Qt::QueuedConnection, Q_ARG( int, glyphResolution ) );

    double scale = tensorViewToolBox->scale();
    QMetaObject::invokeMethod( interactor, "onScalingPropertySet", Qt::QueuedConnection, Q_ARG( double, scale ) );

    bool isShowAxial = tensorViewToolBox->isShowAxial();
    QMetaObject::invokeMethod( interactor, "onHideShowAxialPropertySet", Qt::QueuedConnection, Q_ARG( bool, isShowAxial ) );

    bool isShowCoronal = tensorViewToolBox->isShowCoronal();
    QMetaObject::invokeMethod( interactor, "onHideShowCoronalPropertySet", Qt::QueuedConnection, Q_ARG( bool, isShowCoronal ) );

    bool isShowSagittal = tensorViewToolBox->isShowSagittal();
    QMetaObject::invokeMethod( interactor, "onHideShowSagittalPropertySet", Qt::QueuedConnection, Q_ARG( bool, isShowSagittal ) );
}

void medViewerWorkspaceDiffusion::updateFiberInteractorWithToolboxValues(dtkAbstractViewInteractor* interactor, medToolBoxDiffusionFiberView* fiberViewToolBox)
{
    // we are temporary using Qt's reflection in this function to call the slots in the interactor
    // without casting to a specific type (which is in a plugin)
    // this code will be modified once a refactor in dtk property system is done
    // (we might switch to QVariant instead of strings)
    // TODO refactor this...

    int fiberRadius = d->fiberViewToolBox->fiberRadius();
    QMetaObject::invokeMethod( interactor, "onRadiusSet", Qt::QueuedConnection, Q_ARG( int, fiberRadius ) );

    int fiberColorMode = d->fiberViewToolBox->fiberColorMode();

    if (fiberColorMode==0)
        interactor->setProperty("ColorMode","local");
    if (fiberColorMode==1)
        interactor->setProperty("ColorMode","global");
    if (fiberColorMode==2)
        interactor->setProperty("ColorMode","fa");


    bool isGPUActivated = d->fiberViewToolBox->isGPUActivated();
    interactor->setProperty ("GPUMode", isGPUActivated ? "true" : "false");

    bool isLineModeSelected = d->fiberViewToolBox->isLineModeSelected();
    if (isLineModeSelected)
        interactor->setProperty ("RenderingMode", "lines");

    bool isRibbonModeSelected = d->fiberViewToolBox->isRibbonModeSelected();
    if (isRibbonModeSelected)
        interactor->setProperty ("RenderingMode", "ribbons");

    bool isTubesModeSelected = d->fiberViewToolBox->isTubesModeSelected();
    if (isTubesModeSelected)
        interactor->setProperty ("RenderingMode", "tubes");
}

void medViewerWorkspaceDiffusion::onViewRemoved (dtkAbstractView *view)
{
    if (!view)
        return;

    if (!d->views.contains (view))
        return;

    // view->disableInteractor ("v3dViewFiberInteractor");
    if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor"))
        disconnect(d->fiberViewToolBox, SIGNAL(fiberRadiusSet(int)), interactor, SLOT(onRadiusSet(int)));

    d->views.removeOne (view);
}

void medViewerWorkspaceDiffusion::onFiberColorModeChanged(int index)
{
    foreach (dtkAbstractView *view, d->views) {
        if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
            if (index==0)
                interactor->setProperty("ColorMode","local");
            if (index==1)
                interactor->setProperty("ColorMode","global");
            if (index==2)
                interactor->setProperty("ColorMode","fa");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onGPUActivated (bool value)
{
    foreach (dtkAbstractView *view, d->views) {
        if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
            if (value)
                interactor->setProperty ("GPUMode", "true");
            else
                interactor->setProperty ("GPUMode", "false");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onLineModeSelected (bool value)
{
    foreach (dtkAbstractView *view, d->views) {
        if (value)
            if(dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
            interactor->setProperty ("RenderingMode", "lines");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onRibbonModeSelected (bool value)
{
    foreach (dtkAbstractView *view, d->views) {
        if (value)
            if(dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
            interactor->setProperty ("RenderingMode", "ribbons");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onTubeModeSelected (bool value)
{
    foreach (dtkAbstractView *view, d->views) {
        if (value)
            if(dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
            interactor->setProperty ("RenderingMode", "tubes");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onTBDiffusionSuccess(void)
{
    foreach (dtkAbstractView *view, d->views) {
        view->setData( d->diffusionToolBox->output(), 0 );
        view->reset();
        view->update();
    }

    if (d->diffusionToolBox->output()->identifier()=="v3dDataFibers")
        d->fiberBundlingToolBox->setData( d->diffusionToolBox->output() );

    medDataManager::instance()->importNonPersistent ( d->diffusionToolBox->output(), d->uuid);
}

// tensor interaction related methods

void medViewerWorkspaceDiffusion::onGlyphShapeChanged(const QString& glyphShape)
{
    foreach (dtkAbstractView *view, d->views) {
        if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewTensorInteractor")) {
            interactor->setProperty("GlyphShape", glyphShape);

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onFlipXChanged(bool flipX)
{
    foreach (dtkAbstractView *view, d->views) {
        if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewTensorInteractor")) {

            if (flipX)
                interactor->setProperty("FlipX", "true");
            else
                interactor->setProperty("FlipX", "false");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onFlipYChanged(bool flipY)
{
    foreach (dtkAbstractView *view, d->views) {
        if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewTensorInteractor")) {

            if (flipY)
                interactor->setProperty("FlipY", "true");
            else
                interactor->setProperty("FlipY", "false");

            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onFlipZChanged(bool flipZ)
{
    foreach (dtkAbstractView *view, d->views) {
        if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewTensorInteractor")) {

            if (flipZ)
                interactor->setProperty("FlipZ", "true");
            else
                interactor->setProperty("FlipZ", "false");

            view->update();
        }
    }
}

// end of tensor interaction related methods

void medViewerWorkspaceDiffusion::refreshInteractors (void)
{
    foreach (dtkAbstractView *view, d->views) {
        if(dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
            // interactor->update(); // TO BE IMPLEMENTED
            view->update();
        }
    }
}

void medViewerWorkspaceDiffusion::onAddTabClicked()
{
    QString name = this->identifier();
    QString realName = name;

    unsigned int suppTag = 0;
    while (this->stackedViewContainers()->container(realName))
    {
        suppTag++;
        realName = name + " ";
        realName += QString::number(suppTag);
    }

    this->addSingleContainer(realName);
    this->stackedViewContainers()->setContainer(realName);
}
