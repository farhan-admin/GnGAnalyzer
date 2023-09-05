#pragma once
#include <QWidget>
#include <QPainter>

/// Example taken from
/// https://stackoverflow.com/questions/50611762/how-to-use-qt-qcolormap-in-code/50612579
/// 


class cColorBar : public QWidget {
    Q_OBJECT
public:
    cColorBar(std::shared_ptr<QLinearGradient> gradient): m_ptrGradient(gradient){}
    using QWidget::QWidget;

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setOpacity(0.9);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);

        painter.fillRect(rect(), *m_ptrGradient);
    }

private:
    std::shared_ptr<QLinearGradient> m_ptrGradient;
};
