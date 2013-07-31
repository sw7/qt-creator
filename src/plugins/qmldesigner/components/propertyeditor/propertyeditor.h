/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <abstractview.h>
#include <declarativewidgetview.h>
#include <QHash>
#include <QStackedWidget>
#include <QTimer>

#include "qmlanchorbindingproxy.h"
#include "designerpropertymap.h"
#include "propertyeditorvalue.h"
#include "propertyeditorcontextobject.h"

QT_BEGIN_NAMESPACE
class QShortcut;
class QStackedWidget;
class QTimer;
QT_END_NAMESPACE

class PropertyEditorValue;

namespace QmlDesigner {

class PropertyEditorTransaction;
class CollapseButton;
class StackedWidget;

class PropertyEditor: public AbstractView
{
    Q_OBJECT

    class NodeType {
    public:
        NodeType(PropertyEditor *propertyEditor);
        ~NodeType();

        void setup(const QmlObjectNode &fxObjectNode, const QString &stateName, const QUrl &qmlSpecificsFile, PropertyEditor *propertyEditor);
        void initialSetup(const TypeName &typeName, const QUrl &qmlSpecificsFile, PropertyEditor *propertyEditor);
        void setValue(const QmlObjectNode &fxObjectNode, const PropertyName &name, const QVariant &value);

        DeclarativeWidgetView *m_view;
        Internal::QmlAnchorBindingProxy m_backendAnchorBinding;
        DesignerPropertyMap<PropertyEditorValue> m_backendValuesPropertyMap;
        QScopedPointer<PropertyEditorTransaction> m_propertyEditorTransaction;
        QScopedPointer<PropertyEditorValue> m_dummyPropertyEditorValue;
        QScopedPointer<PropertyEditorContextObject> m_contextObject;
    };

public:
    PropertyEditor(QWidget *parent = 0);
    ~PropertyEditor();

    void setQmlDir(const QString &qmlDirPath);

    bool hasWidget() const;
    WidgetInfo widgetInfo();

    void selectedNodesChanged(const QList<ModelNode> &selectedNodeList,
                              const QList<ModelNode> &lastSelectedNodeList) QTC_OVERRIDE;
    void nodeAboutToBeRemoved(const ModelNode &removedNode) QTC_OVERRIDE;

    void propertiesRemoved(const QList<AbstractProperty>& propertyList) QTC_OVERRIDE;

    void modelAttached(Model *model) QTC_OVERRIDE;

    void modelAboutToBeDetached(Model *model) QTC_OVERRIDE;

    ModelState modelState() const;

    void variantPropertiesChanged(const QList<VariantProperty>& propertyList, PropertyChangeFlags propertyChange) QTC_OVERRIDE;
    void bindingPropertiesChanged(const QList<BindingProperty>& propertyList, PropertyChangeFlags propertyChange) QTC_OVERRIDE;
    void signalHandlerPropertiesChanged(const QVector<SignalHandlerProperty>& propertyList,PropertyChangeFlags propertyChange) QTC_OVERRIDE;

    void instanceInformationsChange(const QMultiHash<ModelNode, InformationName> &informationChangeHash) QTC_OVERRIDE;

    void nodeIdChanged(const ModelNode& node, const QString& newId, const QString& oldId) QTC_OVERRIDE;
    void scriptFunctionsChanged(const ModelNode &node, const QStringList &scriptFunctionList) QTC_OVERRIDE;

    void resetView();
    void currentStateChanged(const ModelNode &node) QTC_OVERRIDE;
    void instancePropertyChange(const QList<QPair<ModelNode, PropertyName> > &propertyList) QTC_OVERRIDE;

    void nodeCreated(const ModelNode &createdNode) QTC_OVERRIDE;
    void nodeRemoved(const ModelNode &removedNode, const NodeAbstractProperty &parentProperty, PropertyChangeFlags propertyChange) QTC_OVERRIDE;
    void nodeAboutToBeReparented(const ModelNode &node, const NodeAbstractProperty &newPropertyParent, const NodeAbstractProperty &oldPropertyParent, PropertyChangeFlags propertyChange) QTC_OVERRIDE;
    void nodeReparented(const ModelNode &node, const NodeAbstractProperty &newPropertyParent, const NodeAbstractProperty &oldPropertyParent, PropertyChangeFlags propertyChange) QTC_OVERRIDE;
    void propertiesAboutToBeRemoved(const QList<AbstractProperty> &propertyList) QTC_OVERRIDE;
    void rootNodeTypeChanged(const QString &type, int majorVersion, int minorVersion) QTC_OVERRIDE;
    void instancesCompleted(const QVector<ModelNode> &completedNodeList) QTC_OVERRIDE;
    void instancesRenderImageChanged(const QVector<ModelNode> &nodeList) QTC_OVERRIDE;
    void instancesPreviewImageChanged(const QVector<ModelNode> &nodeList) QTC_OVERRIDE;
    void instancesChildrenChanged(const QVector<ModelNode> &nodeList) QTC_OVERRIDE;
    void instancesToken(const QString &tokenName, int tokenNumber, const QVector<ModelNode> &nodeVector) QTC_OVERRIDE;
    void nodeSourceChanged(const ModelNode &modelNode, const QString &newNodeSource) QTC_OVERRIDE;
    void rewriterBeginTransaction() QTC_OVERRIDE;
    void rewriterEndTransaction() QTC_OVERRIDE;
    void nodeOrderChanged(const NodeListProperty &listProperty, const ModelNode &movedNode, int oldIndex) QTC_OVERRIDE;
    void importsChanged(const QList<Import> &addedImports, const QList<Import> &removedImports) QTC_OVERRIDE;

protected:
    void timerEvent(QTimerEvent *event);
    void setupPane(const TypeName &typeName);
    void setValue(const QmlObjectNode &fxObjectNode, const PropertyName &name, const QVariant &value);

private slots:
    void reloadQml();
    void changeValue(const QString &name);
    void changeExpression(const QString &name);
    void updateSize();
    void setupPanes();

private: //functions
    QString qmlFileName(const NodeMetaInfo &nodeInfo) const;
    QUrl fileToUrl(const QString &filePath) const;
    QString fileFromUrl(const QUrl &url) const;
    QUrl qmlForNode(const ModelNode &modelNode, TypeName &className) const;
    QString locateQmlFile(const NodeMetaInfo &info, const QString &relativePath) const;
    void select(const ModelNode& node);

    void delayedResetView();


private: //variables
    ModelNode m_selectedNode;
    QWidget *m_parent;
    QShortcut *m_updateShortcut;
    int m_timerId;
    StackedWidget* m_stackedWidget;
    QString m_qmlDir;
    QHash<QString, NodeType *> m_typeHash;
    NodeType *m_currentType;
    bool m_locked;
    bool m_setupCompleted;
    QTimer *m_singleShotTimer;
};


class StackedWidget : public QStackedWidget
{
Q_OBJECT

public:
    StackedWidget(QWidget *parent = 0) : QStackedWidget(parent) {}

signals:
    void resized();
protected:
    void resizeEvent(QResizeEvent * event)
    {
        QStackedWidget::resizeEvent(event);
        emit resized();
    }
};
}

#endif // PROPERTYEDITOR_H
