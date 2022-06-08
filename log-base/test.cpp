#include "logging.h"
#include <iostream>

class Point {
public:
    Point(int x, int y) : px(x), py(y) {}

    void setP(int x, int y) {
        px = x;
        py = y;
    }

    int getX() const { return px; }
    int getY() const { return py; }

    friend std::ostream& operator<<(std::ostream& os, const Point pt);
private:
    int px;
    int py;
};

std::ostream& operator<<(std::ostream& os, const Point pt) {
    os << '(' << pt.getX() << ", " << pt.getY() << ')';
    return os;
}

int main() {
    logSettingInitialize(0);
    std::string str = " It is a message.";
    // We cannot use std::endl, so the project need to be updated
    LOG(INFO) << "String: " << str << '\n';
    Point point(-5, 10);
    LOG(WARN) << point << " won't be showed here.\n";
    DLOG(FATAL) << "Should Not Abort because no debug!";
    LOG(ERROR) << "Not true " << 1009;
    LOG(INFO)  << "Test appendLine.";
    CHECK_EQ(2, 2.0);
    const int line = __LINE__ + 1;
    LOG(FATAL) << "Abort at LINE " << line << std::endl;
}
