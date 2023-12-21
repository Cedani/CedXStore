#pragma once

#include <QLabel>

namespace lau {
    class ClickableLabel : public QLabel {

        Q_OBJECT
        public:
            explicit ClickableLabel(QWidget *w = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

        signals:
            void clicked();

        protected:
            void mouseClickEvent(QMouseEvent *);
    };
}