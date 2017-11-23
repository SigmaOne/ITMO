#include "window.h"

#include <QtWidgets>

Window::Window() {
    game = new Game;
    renderArea = new RenderArea;

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    setLayout(mainLayout);

    setWindowTitle(tr("Computer Graphics Lab 3 Qt"));
}
