#include "view.h"
#include "coordinate_system_panel.h"
#include <QPainter>
#include <QResizeEvent>
#include <algorithm>
#include <cmath>
#include <utility>
#include <QTextStream>

view::view(QWidget *parent) : QWidget(parent), /*step(50),*/ step_x(50), step_y(50)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);

    setAutoFillBackground(true);
}

void view::paintEvent(QPaintEvent *) {
    if (c_s == nullptr) {
        return;
    }
    draw_cord_system(this);
    draw_function(this);
}

void view::resizeEvent(QResizeEvent *event) {
    if (event->oldSize().width() == -1 || event->oldSize().height() == -1)
        return;
    double coef_x = width() / static_cast<double>(event->oldSize().width());
    double coef_y = height() / static_cast<double>(event->oldSize().height());
//    QTextStream out(stdout);
//    out << "coef_x: " << coef_x << ", coef_y: " << coef_y;
//    out << ", width: " << width() << ", old width: " << event->oldSize().width();
//    out << ", height: " << height() << ", old height: " << event->oldSize().height();
//    out << ", step_x: " << step_x << ", step_y: " << step_y << "\n";
    step_x *= coef_x;
    step_y *= coef_y;
    update();
}

coordinate_system_panel *view::cord_system() const {
    return c_s;
}

void view::set_cord_system_panel(coordinate_system_panel *c) {
    c_s = c;
    update();
}

void view::set_step(const int &x, const int &y) {
    step_x = x;
    step_y = y;
}

std::pair<int, int> view::get_step() {
    return std::make_pair(step_x, step_y);
}

void view::set_division(const int &x, const int &y) {
    c_s->set_div_x(x);
    c_s->set_div_y(y);
}

void draw_cord_system(view *v) {
    const int div_x = v->c_s->division_x();
    const int div_y = v->c_s->division_y();
    const int center_x = v->width() / 2;
    const int center_y = v->height() / 2;
    const double pi = atan(1) * 4;

    QPainter ptr{v};
    ptr.setPen(QPen(Qt::black, 3));

    QPoint p1{0, center_y};
    QPoint p2{v->width(), center_y};
    const int branch_len = 15;
    // drawing axis x
    ptr.drawLine(p1, p2);

    QPointF p_branch1{static_cast<double>(v->width()), static_cast<double>(center_y)};
    QPointF p_branch2{branch_len * cos(-5 * pi / 6) + v->width(),
                branch_len * sin(-5 * pi / 6) + center_y};
    ptr.drawLine(p_branch1, p_branch2);
    p_branch2 = {branch_len * cos(5 * pi / 6) + v->width(),
                 branch_len * sin(5 * pi / 6) + center_y};
    ptr.drawLine(p_branch1, p_branch2);

    // drawing axis y
    p1.setX(center_x);
    p1.setY(0);
    p2.setX(center_x);
    p2.setY(v->height());
    ptr.drawLine(p1, p2);

    p_branch1 = {static_cast<double>(center_x), 0};
    p_branch2 = {branch_len * cos(pi / 3) + center_x,
                branch_len * sin(pi / 3)};
    ptr.drawLine(p_branch1, p_branch2);
    p_branch2 = {branch_len * cos(2 * pi / 3) + center_x,
                 branch_len * sin(2 * pi / 3)};
    ptr.drawLine(p_branch1, p_branch2);


    ptr.setPen(QPen(Qt::black, 3));
    ptr.drawText(QPointF(center_x - (v->step_x / 4), center_y + (v->step_y / 4)),
                 QString::number(0));

    // drawing grid
    p_branch1.setY(center_y + v->step_y / 4);
    p_branch2.setY(center_y - v->step_y / 4);
    p1.setY(0);
    p2.setY(v->height());
    for (int x = static_cast<int>(v->step_x), num = 0; x + center_x < v->width(); x += v->step_x, num += div_x) {
        ptr.setPen(Qt::gray);
        p1.setX(x + center_x);
        p2.setX(x + center_x);
        ptr.drawLine(p1, p2);

        ptr.setPen(QPen(Qt::black, 3));
        p_branch1.setX(x + center_x);
        p_branch2.setX(x + center_x);
        ptr.drawLine(p_branch1, p_branch2);

        ptr.drawText(QPointF(x + center_x - v->step_x / 6, static_cast<int>(p_branch1.y()) - v->step_y / 2),
                     QString::number(num + div_x));

        ptr.setPen(Qt::gray);
        p1.setX(center_x - x);
        p2.setX(center_x - x);
        ptr.drawLine(p1, p2);

        ptr.setPen(QPen(Qt::black, 3));
        p_branch1.setX(center_x - x);
        p_branch2.setX(center_x - x);
        ptr.drawLine(p_branch1, p_branch2);

        ptr.drawText(QPointF(center_x - x - v->step_x / 6, static_cast<int>(p_branch1.y()) - v->step_y / 2),
                     QString::number(-1 * (num + div_x)));
    }

    p_branch1.setX(center_x + v->step_x / 4);
    p_branch2.setX(center_x - v->step_x / 4);
    p1.setX(0);
    p2.setX(v->width());
    for (int y = static_cast<int>(v->step_y), num = 0; y + center_y < v->height(); y += v->step_y, num += div_y) {
        ptr.setPen(Qt::gray);
        p1.setY(y + center_y);
        p2.setY(y + center_y);
        ptr.drawLine(p1, p2);

        ptr.setPen(QPen(Qt::black, 3));
        p_branch1.setY(y + center_y);
        p_branch2.setY(y + center_y);
        ptr.drawLine(p_branch1, p_branch2);

        ptr.drawText(QPointF(static_cast<int>(p_branch1.x()) + v->step_x / 6, center_y - y + v->step_y / 6),
                     QString::number(num + div_y));

        ptr.setPen(Qt::gray);
        p1.setY(center_y - y);
        p2.setY(center_y - y);
        ptr.drawLine(p1, p2);

        ptr.setPen(QPen(Qt::black, 3));
        p_branch1.setY(center_y - y);
        p_branch2.setY(center_y - y);
        ptr.drawLine(p_branch1, p_branch2);

        ptr.drawText(QPointF(static_cast<int>(p_branch1.x()) + v->step_x / 6, center_y + y + v->step_y / 6),
                     QString::number(-1 * (num + div_y)));
    }
}

void draw_function(view *v) {
    QPainter ptr{v};
    ptr.setPen(QPen(Qt::blue, 3));
    const double param = pow(v->c_s->parametr(), 2. / 3.);
    const double step = 0.01;
    const double center_x = v->width() / 2;
    const double center_y = v->height() / 2;
    const double div_x = v->c_s->division_x();
    const double div_y = v->c_s->division_y();
    QPointF p1{center_x, center_y - pow(param, 3. / 2.) * v->step_y / div_y};
    QPointF p2{};
    QPointF p3{p1};
    QPointF p4{};
    for (double x = step; param - pow(x, 2. / 3.) >= 0; x += step) {
        p2 = {x * v->step_x / div_x + center_x, center_y - pow(param - pow(x, 2. / 3.), 3. / 2.) * v->step_y / div_y};
        ptr.drawLine(p1, p2);
        p1 = p2;
        p4 = {-x * v->step_x / div_x + center_x, center_y - pow(param - pow(x, 2. / 3.), 3. / 2.) * v->step_y / div_y};
        ptr.drawLine(p3, p4);
        p3 = p4;
    }

}
