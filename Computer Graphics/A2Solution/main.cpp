#include "solution.h"

Solution solution;
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    solution.Create();
    glutMainLoop();
    return EXIT_SUCCESS;
}