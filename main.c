#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 24
#define MAX_OBJECTS 100

#define EMPTY '_'
#define PIXEL '*'

typedef enum {
    LINE,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Shape;

char picture[HEIGHT][WIDTH];
Shape objects[MAX_OBJECTS];
int object_count = 0;

void clearPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            picture[y][x] = EMPTY;
        }
    }
}

void displayPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(picture[y][x]);
        }
        putchar('\n');
    }
}

void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        picture[y][x] = PIXEL;
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;

    drawLine(left, top, right, top);
    drawLine(left, bottom, right, bottom);
    drawLine(left, top, left, bottom);
    drawLine(right, top, right, bottom);
}

void drawCircle(int cx, int cy, int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        setPixel(cx + x, cy + y);
        setPixel(cx + y, cy + x);
        setPixel(cx - y, cy + x);
        setPixel(cx - x, cy + y);
        setPixel(cx - x, cy - y);
        setPixel(cx - y, cy - x);
        setPixel(cx + y, cy - x);
        setPixel(cx + x, cy - y);

        y++;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void drawObject(const Shape *shape) {
    switch (shape->type) {
        case LINE:
            drawLine(shape->x1, shape->y1, shape->x2, shape->y2);
            break;
        case RECTANGLE:
            drawRectangle(shape->x1, shape->y1, shape->x2, shape->y2);
            break;
        case CIRCLE:
            drawCircle(shape->x1, shape->y1, shape->radius);
            break;
        case TRIANGLE:
            drawTriangle(shape->x1, shape->y1, shape->x2, shape->y2, shape->x3, shape->y3);
            break;
    }
}

void renderObjects() {
    clearPicture();
    for (int i = 0; i < object_count; i++) {
        drawObject(&objects[i]);
    }
}

void printMenu() {
    printf("2D Graphics Editor\n");
    printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
    printf("1. Add object\n");
    printf("2. Delete object\n");
    printf("3. Modify object\n");
    printf("4. Display picture\n");
    printf("5. List objects\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

ShapeType readShapeType() {
    int type;
    printf("Choose shape type:\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("Enter shape type: ");
    scanf("%d", &type);
    if (type == 1) return LINE;
    if (type == 2) return RECTANGLE;
    if (type == 3) return CIRCLE;
    if (type == 4) return TRIANGLE;
    return LINE;
}

void addObject() {
    if (object_count >= MAX_OBJECTS) {
        return;
    }

    Shape shape;
    shape.type = readShapeType();

    if (shape.type == LINE) {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2);
    } else if (shape.type == RECTANGLE) {
        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2);
    } else if (shape.type == CIRCLE) {
        printf("Enter center x y and radius: ");
        scanf("%d %d %d", &shape.x1, &shape.y1, &shape.radius);
    } else if (shape.type == TRIANGLE) {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2, &shape.x3, &shape.y3);
    }

    objects[object_count] = shape;
    printf("Object added with index %d.\n", object_count);
    object_count++;
}

void deleteObject() {
    int index;
    printf("Enter object index: ");
    scanf("%d", &index);
    if (index < 0 || index >= object_count) {
        printf("Invalid index.\n");
        return;
    }
    for (int i = index; i < object_count - 1; i++) {
        objects[i] = objects[i + 1];
    }
    object_count--;
}

void modifyObject() {
    int index;
    printf("Enter object index: ");
    scanf("%d", &index);
    if (index < 0 || index >= object_count) {
        printf("Invalid index.\n");
        return;
    }

    Shape shape;
    shape.type = readShapeType();

    if (shape.type == LINE) {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2);
    } else if (shape.type == RECTANGLE) {
        printf("Enter top-left x y and bottom-right x y: ");
        scanf("%d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2);
    } else if (shape.type == CIRCLE) {
        printf("Enter center x y and radius: ");
        scanf("%d %d %d", &shape.x1, &shape.y1, &shape.radius);
    } else if (shape.type == TRIANGLE) {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d %d %d %d %d %d", &shape.x1, &shape.y1, &shape.x2, &shape.y2, &shape.x3, &shape.y3);
    }

    objects[index] = shape;
}

const char *getShapeName(ShapeType type) {
    switch (type) {
        case LINE: return "Line";
        case RECTANGLE: return "Rectangle";
        case CIRCLE: return "Circle";
        case TRIANGLE: return "Triangle";
        default: return "Unknown";
    }
}

void listObjects() {
    if (object_count == 0) {
        printf("No objects.\n");
        return;
    }

    for (int i = 0; i < object_count; i++) {
        Shape *shape = &objects[i];
        if (shape->type == LINE) {
            printf("%d. %s %d %d %d %d\n", i, getShapeName(shape->type), shape->x1, shape->y1, shape->x2, shape->y2);
        } else if (shape->type == RECTANGLE) {
            printf("%d. %s %d %d %d %d\n", i, getShapeName(shape->type), shape->x1, shape->y1, shape->x2, shape->y2);
        } else if (shape->type == CIRCLE) {
            printf("%d. %s %d %d %d\n", i, getShapeName(shape->type), shape->x1, shape->y1, shape->radius);
        } else if (shape->type == TRIANGLE) {
            printf("%d. %s %d %d %d %d %d %d\n", i, getShapeName(shape->type), shape->x1, shape->y1, shape->x2, shape->y2, shape->x3, shape->y3);
        }
    }
}

int main() {
    int choice;

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            addObject();
        } else if (choice == 2) {
            deleteObject();
        } else if (choice == 3) {
            modifyObject();
        } else if (choice == 4) {
            renderObjects();
            displayPicture();
        } else if (choice == 5) {
            listObjects();
        } else if (choice == 0) {
            printf("Goodbye.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
