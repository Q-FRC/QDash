#ifndef COMPILEDEFINITIONS_H
#define COMPILEDEFINITIONS_H

#include <QObject>
#include <QQmlProperty>

class CompileDefinitions : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool useWebView READ useWebView FINAL CONSTANT)
    Q_PROPERTY(bool useCameraView READ useCameraView FINAL CONSTANT)
    Q_PROPERTY(bool useNetwork READ useNetwork FINAL CONSTANT)
    Q_PROPERTY(QVariantMap extraWidgets READ extraWidgets FINAL CONSTANT)
public:
    using QObject::QObject;

    QVariantMap extraWidgets() {
        // title, type
        QVariantMap widgets;

#ifdef QDASH_WEBVIEW
        widgets.insert("Web View", "web");
#endif

        return widgets;
    }

    constexpr bool useWebView() {
#ifdef QDASH_WEBVIEW
        return true;
#else
        return false;
#endif
    }

    constexpr bool useCameraView() {
#ifdef QDASH_CAMVIEW
        return true;
#else
        return false;
#endif
    }

    constexpr bool useNetwork() {
#ifdef QDASH_NETWORK
        return true;
#else
        return false;
#endif
    }
};

#endif // COMPILEDEFINITIONS_H
